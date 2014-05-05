#include "speicher.h"
#include <iostream>
#include <QtConcurrent/QtConcurrentRun>

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

using namespace std;

Speicher::Speicher(void)
{
    terminateEEPROM = false;

    // Mapping
    // ungemappte Register (z.B. 00h oder 07h) noch nicht berücksichtigt!!! - mit null-Zeiger abfangen?)
    // aktuell 2x47 Register als Speicherzellen verfügbar... Bank1 nutzt nur 5
    for(int i=0; i < n_register; i++)             // mapping Bank 0
        refBank[0][i] = &(bank0[i]);

    for(int i=0; i < n_register; i++)             // mapping Bank 1
        refBank[1][i] = &(bank0[i]);

    // undefinierte Register erhalten Null-Zeiger
    refBank[0][0] = NULL;
    refBank[1][0] = NULL;
    refBank[0][7] = NULL;
    refBank[1][7] = NULL;

    refBank[1][1] = &(bank1[1]);    // OPTION-Register
    refBank[1][5] = &(bank1[5]);    // TRISA-Register
    refBank[1][6] = &(bank1[6]);    // TRISB-Register
    refBank[1][8] = &(bank1[8]);    // EECON1-Register
    refBank[1][9] = &(bank1[9]);    // EECON2-Register

    eecon1 = refBank[1][8];
    eedata = refBank[0][8];
    eeadr = refBank[0][9];

	// Registerinhalte mit 0 initialisieren
	clearSpeicher();
	workingregister =0;

    QtConcurrent::run(this,&Speicher::eepromRead);
    QtConcurrent::run(this,&Speicher::eepromWrite);

}
Speicher::~Speicher(void)
{
}

bool Speicher::clearSpeicher()
{
	latchA = latchB = 0;
    workingregister =0;
    address_2007h = 0xffff;
    eecon2=0;
	for(int i = 0; i < n_register; i++)
		bank0[i] = bank1[i] = 0;

    for(int j= 0; j <= EEPROM_SIZE;j++)
    {
        eeprom[j]=0;
    }

	//Register mit vorwerten belegen
	*refBank[0][3] = 0x0018; //STATUS Register TO und PD sind 1 (S.14)
	*refBank[1][1] = 0x00ff; //OPTION Register alles ist 1 (S.15)
	*refBank[0][11] = 0x0000; //INCON Register alles 0 (S.16)
	*refBank[1][5] = 0x00ff; //TRISA untere 5 Bit sind 1 (es existieren nur 5) S.13
	*refBank[1][6] = 0x00ff; //TRISB alle bit sind 1 S.13

	return true;
}

// nur zum Testen
void Speicher::ausgeben(int bank)
{
    cout << "Bank" << bank << endl;
    cout << "Zelle\t" << "Wert" << endl;
    for(int i=0; i < n_register; i++)
        if(refBank[bank][i] != 0)
            cout << i << "\t" << *(refBank[bank][i]) << endl;
        else
            cout << i << "\t" << "N/A" << endl;

    cout << "Bank Content Ende!" << endl;
}

int* Speicher::getFileReference(int file)
{
    int bank;
    if(CHECK_BIT(bank0[3],5))
        bank = 1;
    else
        bank = 0;

    return refBank[bank][file];
}

//TODO Hier wird in den Speicher geschrieben und fehler abgefangen; z.B. nicht Schreibbare Bits
bool Speicher::write(int file, int wert)
{
	int* FileReference;
	if((FileReference=getFileReference(file))==NULL)
		return false; // kein gültiges file...

    if(CHECK_BIT(bank0[3],5))
    {
        cout << "writing on Bank 1" << endl;
        //Bank 1
        //Indirect addr. adressiert die Adresse die im FSR Register hinterlegt ist
        //-> siehe abschnitt FSR für implementierung
		//Option Register ist voll schreibbar
		//PCL ist voll schreibbar
		//Status Register: bit 3 und 4 sind nicht schreibbar
		if(file == 3)
		{
			*FileReference= (wert & 0x00E7) + (*FileReference & 0x0018);
			return true;
		}
        //FSR ist voll schreibbar; der Wert wird als adresse aufgefasst und durch Adrees 0x00 Adressiert
        //-> refBank[0][0] und refBank[1][0] müssen auf diese Adresse zeigen
        if(file==4)
        {
            int indFile = wert & 0x007f; //die durch 0x00 zu referenzierende Adresse
            int indBank = (wert & 0x0080) >> 7; //die durch 0x00 zu referenzierende Bank
            refBank[0][0]= refBank[indBank][indFile];
            refBank[1][0]= refBank[indBank][indFile];
            if(indFile==0) //wenn fsr wieder zurück auf 0 gesetzt wird, würde die vorherige adresse erhalten bleiben -> daher manuell NULL
            {
                refBank[0][0]= NULL;
                refBank[1][0]= NULL;
            }
        }
		//TRISA ist voll schreibbar, Latch muss in den speicher übernommen werden
		if(file==5)
		{
			*FileReference=wert;
			writeOnBank(0,5,(latchA & (~bank1[5])) + (readOnBank(0,5) & bank1[5]));
			return true;
		}
		//TRISB ist voll schreibbar, latch muss in speicher geschrieben werden
		if(file==6)
		{
			*FileReference=wert;
			writeOnBank(0,6,(latchB & (~bank1[6])) + (readOnBank(0,6) & bank1[6]));
			return true;
		}
        //EECON1 ist teilweise voll schreibbar. RD und WR dürfen nur gesetzt werden
        if(file==8)
        {
            if((wert&0x0001) != 0) //RD Bit gesetzt?
                *FileReference |= 0x0001;//->RD bit setzen
            if((wert&0x0002) != 0) //WR bit gesetzt?
                *FileReference |= 0x0002; //->WR bit setzen

            *FileReference = (wert & 0xfffc) + (*FileReference & 0x0003);
        }
        //EECON2 ist nicht schreibbar. Daten müssen an EEPROM weitergereicht werden
        if(file==9)
        {
            eecon2 = wert;
            return true;
        }
		//PCLATH ist voll schreibbar
		//INTCON ist voll schreibbar
	}
	else
    {
        cout << "writing on Bank 0" << endl;
        //Bank 0
        //Indirect addr. adressiert die Adresse die im FSR Register hinterlegt ist
        //-> siehe abschnitt FSR für implementierung
		//TMR0 ist voll schreibbar
		//PCL ist voll schreibbar
		//Status Register: bit 3 und 4 sind nicht schreibbar
		if(file == 3)
		{
			*FileReference= (wert & 0x00E7) + (*FileReference & 0x0018);
			return true;
		}
        //FSR ist voll schreibbar; der Wert wird als adresse aufgefasst und durch Adrees 0x00 Adressiert
        //-> refBank[0][0] und refBank[1][0] müssen auf diese Adresse zeigen
        if(file==4)
        {
            int indFile = wert & 0x007f; //die durch 0x00 zu referenzierende Adresse
            int indBank = (wert & 0x0080) >> 7; //die durch 0x00 zu referenzierende Bank
            refBank[0][0]= refBank[indBank][indFile];
            refBank[1][0]= refBank[indBank][indFile];
            if(indFile==0) //wenn fsr wieder zurück auf 0 gesetzt wird, würde die vorherige adresse erhalten bleiben -> daher manuell NULL
            {
                refBank[0][0]= NULL;
                refBank[1][0]= NULL;
            }
        }
		//PortA nur die Bit bei denen TRISA = 0 können geschrieben werden
		if(file==5)
		{
			latchA = wert;
			*FileReference = (wert & (~bank1[5])) + (*FileReference & bank1[5]);
			return true;
		}
		//PortB nur die Bit bei denen TRISA = 0 können geschrieben werden
		if(file==6)
		{
			latchB = wert;
			*FileReference = (wert & (~bank1[6])) + (*FileReference & bank1[6]);
			return true;
		}
		//EEDATA ist voll schreibbar
		//EEADR ist voll schreibbar
		//PCLATH ist voll schreibbar
		//INTCON ist voll schreibbar
	}
	*FileReference=wert;
    return true;
}

int Speicher::read( int file)
{ //TODO Fehler abfangen. Bisher zeigt Adresse 0 z.B. auf NULL
    /*
     * Fehlerwerte:
     * 0x0100 == Speicher existiert nicht
     *
     */

    if(getFileReference(file)==NULL)
            return 0x0100;
    return *getFileReference(file);
}

int Speicher::readOnBank (int bank, int file)
{
   // cout << "Speicher read" << endl;

	if(refBank[bank][file] == NULL)
			return 0;
	return *(refBank[bank][file]);

}

void Speicher::writeOnBank(int bank,int file,int wert)
{
    if(refBank[bank][file] == NULL)
        return;

    *refBank[bank][file] = wert;
}

int Speicher::readW()
{
    return workingregister;
}

void Speicher::writeW(int value)
{
    workingregister=value;
}

void Speicher::setZBit()
{
    bank0[3]= bank0[3] | (1<<2);
}

void Speicher::clearZBit()
{
    bank0[3]= bank0[3] & (~(1<<2));
}

void Speicher::setDCBit()
{
    bank0[3]= bank0[3] | (1<<1);
}

void Speicher::clearDCBit()
{
    bank0[3]= bank0[3] & (~(1<<1));
}

void Speicher::setCBit()
{
    bank0[3]= bank0[3] | (1);
}
void Speicher::clearCBit()
{
    bank0[3]= bank0[3] & (~1);
}

//Den Wert des PC (intern) in die Register des PIC schreiben
void Speicher::writePC(int value)
{
	// PCL = value & 0x00FF
	bank0[2]= value & 0x00FF;

	//PCLATH = (value &1F00) >> 8
	bank0[0xa]= (value&0x1f00)>>8;
}

int Speicher::read2007()
{
	return address_2007h;
}

void Speicher::write2007(int value)
{
	address_2007h = value | 0xffe0;
}



//EEPROM
void Speicher::eepromRead()
{
    while(!terminateEEPROM)
    {
        if(*eecon1&0x0001)// RD Bit ist 1
        {
            if(*eeadr > EEPROM_SIZE)
                *eedata=0;
            else
                *eedata = eeprom[*eeadr];
            *eecon1 = *eecon1 & 0xfffe; //lösche RD bit
        }
    }
}

void Speicher::eepromWrite()
{
    while(!terminateEEPROM)
    {
        if(eecon2==0x0055)
        {
            while(!terminateEEPROM)
            {
                if(eecon2==0x00aa)
                {
                    if((*eecon1&0x0006)==0x0006) //WREN und WR bit gesetzt?
                    {
                        if(*eeadr <=EEPROM_SIZE)
                        {
                            eeprom[*eeadr]= *eedata;
                        }

                        //clear WR
                        *eecon1 &=  0xfffd;

                        //set EEIF
                        *eecon1 |= 0x0010;

                    }

                    break;
                }
            }
        }

    }
}
