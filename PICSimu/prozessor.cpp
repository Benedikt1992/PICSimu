#include "prozessor.h"
#include "steuerwerk.h"
#include "sleepklasse.h"
#include <iostream>
#include <vector>
#include <QThread>

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

using namespace std;

Prozessor::Prozessor(void)
{
    cycles =0;
}


Prozessor::~Prozessor(void)
{
}

bool Prozessor::clearProzessor()
{
    cycles =0;
	return speicher.clearSpeicher();

}
bool Prozessor::clearRuntime()
{
    cycles = 0;
    return true;
}

/*
 *
 * Befehlsimplementierungen
 *
 */

void Prozessor::addwf(int command)
{
    bool storeInFileRegister = (CHECK_BIT(command,7));

	//      00 0111 dfff ffff
    //  &   00 0000 0111 1111  = 0x7F
    //      00 0000 0kkk kkkk
    int file = command & 0x7F;

    // Register laden
    int currentValue = speicher.read(file);
    if(currentValue== 0x0100) //die Speicheradresse ist nicht belegt!!
    {
        cycles++;
        return;
    }

    int workingRegisterValue = speicher.readW();

    // Rechenoperation
    int newValue = currentValue + workingRegisterValue;

    // betroffene Flags setzen/löschen
    checkCarryFlag(newValue);
    checkZeroFlag(newValue);
	checkDigitCarryFlagAddition(currentValue, workingRegisterValue);

    writeBack(file, newValue, storeInFileRegister);

    cycles++;
}

void Prozessor::andwf(int command)
{
    bool storeInFileRegister = (CHECK_BIT(command,7));

    //      00 0101 dkkk kkkk
    //  &   00 0000 0111 1111  = 0x7F
    //      00 0000 0kkk kkkk
    int file = command & 0x7F;

    // Register laden
    int currentValue = speicher.read(file);
    if(currentValue== 0x0100) //die Speicheradresse ist nicht belegt!!
    {
        cycles++;
        return;
    }

    int workingRegisterValue = speicher.readW();

    // Rechenoperation
    int newValue = currentValue & workingRegisterValue;

    // betroffene Flags setzen/löschen
    checkZeroFlag(newValue);

    writeBack(file, newValue, storeInFileRegister);

    cycles++;
}

void Prozessor::clrf(int command)
{
    // 00 0001 1fff ffff
    int file = command & 0x007f;
    writeBack(file,0,WRITE_TO_FILE_REGISTER);
    //speicher.write(file,0);

    speicher.setZBit();

    cycles++;
}

void Prozessor::clrw()
{
    speicher.writeW(0);
    speicher.setZBit();
    cycles++;
}

void Prozessor::comf(int command)
{
    bool storeInFileRegister = (CHECK_BIT(command,7));

    //      01 1001 dfff ffff
    //  &   00 0000 0111 1111  = 0x7F
    //      00 0000 0fff ffff
    int file = (command & 0x7F);

    // Register laden
    int currentValue = speicher.read(file);
    if(currentValue== 0x0100) //die Speicheradresse ist nicht belegt!!
    {
        cycles++;
        return;
    }

    // Rechenoperation
    int newValue = ~currentValue;

    // betroffene Flags setzen/löschen
    checkZeroFlag(newValue);

    writeBack(file, newValue, storeInFileRegister);

    cycles++;
}

void Prozessor::decf(int command)
{
    bool storeInFileRegister = (CHECK_BIT(command,7));

	//      00 0011 dfff ffff
    //  &   00 0000 0111 1111  = 0x7F
    //      00 0000 0fff ffff
    int file = (command & 0x7F);

    int currentValue = speicher.read(file);
    if(currentValue== 0x0100) //die Speicheradresse ist nicht belegt!!
    {
        cycles++;
        return;
    }

    // Rechenoperation
    int newValue = currentValue - 1;

    // betroffene Flags prüfen
    checkZeroFlag(newValue);

    writeBack(file, newValue, storeInFileRegister);

    cycles++;
}

void Prozessor::decfsz(int command, Steuerwerk *steuerwerk)
{
    bool storeInFileRegister = (CHECK_BIT(command,7));

    //      00 1011 dfff ffff
    //  &   00 0000 0111 1111  = 0x7F
    //      00 0000 0fff ffff
    int file = (command & 0x7F);

    int currentValue = speicher.read(file);
    if(currentValue== 0x0100) //die Speicheradresse ist nicht belegt!!
    {
        cycles++;
        return;
    }

    // Rechenoperation
    int newValue = currentValue - 1;

    // keine Flags im Status-Register betroffen

    writeBack(file, newValue, storeInFileRegister);

    if((newValue & 0xFF) == 0)
    {
        // Bit ist 0 -> nächster Befehl wird übersprungen
        steuerwerk->pc++;
        cycles+=2;
    }
    else
    {
        cycles++;
    }
}

void Prozessor::incf(int command)
{
    bool storeInFileRegister = (CHECK_BIT(command,7));

    //      00 1010 dfff ffff
    //  &   00 0000 0111 1111  = 0x7F
    //      00 0000 0fff ffff
    int file = command & 0x7F;

    // Register laden
    int currentValue = speicher.read(file);
    if(currentValue== 0x0100) //die Speicheradresse ist nicht belegt!!
    {
        cycles++;
        return;
    }

    // Rechenoperation
    int newValue = currentValue + 1;

    // betroffene Flags prüfen und setzen/löschen
    checkZeroFlag(newValue);

    writeBack(file, newValue, storeInFileRegister);
    cycles++;
}

void Prozessor::incfsz(int command, Steuerwerk *steuerwerk)
{
    bool storeInFileRegister = (CHECK_BIT(command,7));

    //      00 1011 dfff ffff
    //  &   00 0000 0111 1111  = 0x7F
    //      00 0000 0fff ffff
    int file = (command & 0x7F);

    int currentValue = speicher.read(file);
    if(currentValue== 0x0100) //die Speicheradresse ist nicht belegt!!
    {
        cycles++;
        return;
    }

    // Rechenoperation
    int newValue = currentValue + 1;

    // keine Flags im Status-Register betroffen

    writeBack(file, newValue, storeInFileRegister);

    if((newValue & 0xFF) == 0)
    {
        // Bit ist 0 -> nächster Befehl wird übersprungen
        steuerwerk->pc++;
        cycles+=2;
    }
    else
    {
        // Bit ist 1
        cycles++;
    }
}

void Prozessor::iorwf(int command)
{
    bool storeInFileRegister = (CHECK_BIT(command,7));

    //      00 0100 dfff ffff
    //  &   00 0000 0111 1111  = 0x7F
    //      00 0000 0fff ffff
    int file = command & 0x7F;

    // Register laden
    int currentValue = speicher.read(file);
    int workingRegisterValue = speicher.readW();
    if(currentValue== 0x0100) //die Speicheradresse ist nicht belegt!!
    {
        cycles++;
        return;
    }

    // Rechenoperation
    int newValue = currentValue | workingRegisterValue;

    // betroffene Flags prüfen und setzen/löschen
    checkZeroFlag(newValue);    // prüfen warum !Z ??? s. Datenblatt

    writeBack(file, newValue, storeInFileRegister);

    cycles++;
}

void Prozessor::movf(int command)
{
    bool storeInFileRegister = (CHECK_BIT(command,7));

    //      00 1000 dfff ffff
    //  &   00 0000 0111 1111  = 0x7F
    //      00 0000 0fff ffff
    int file = command & 0x7F;

    // Register laden
    int currentValue = speicher.read(file);
    if(currentValue== 0x0100) //die Speicheradresse ist nicht belegt!!
    {
        cycles++;
        return;
    }

    // Operation
    int newValue = currentValue;

    // betroffene Flags prüfen und setzen/löschen
    checkZeroFlag(newValue);

    writeBack(file, newValue, storeInFileRegister);

    cycles++;
}

void Prozessor::movwf(int command)
{
	cout << "MOVWF" <<endl;
    //      00 0000 1fff ffff
    //  &   00 0000 0111 1111  = 0x7F
    //      00 0000 0fff ffff
    int file = command & 0x7F;

    // Register laden
    int currentValue = speicher.readW();

    cout << "W=" << currentValue << endl;
    cout << "file= " << file << endl;

    if(currentValue== 0x0100) //die Speicheradresse ist nicht belegt!!
    {
        cycles++;
        return;
    }

    writeBack(file, currentValue, WRITE_TO_FILE_REGISTER);

    cycles++;
}

void Prozessor::nop()
{
    cycles++;
}

void Prozessor::rlf(int command)
{
    bool storeInFileRegister = (CHECK_BIT(command,7));

    //      01 1101 dfff ffff
    //  &   00 0000 0111 1111  = 0x7F
    //      00 0000 0fff ffff
    int file = command & 0x7F;

    // Register laden
    int currentValue = speicher.read(file);
    if(currentValue== 0x0100) //die Speicheradresse ist nicht belegt!!
    {
        cycles++;
        return;
    }

    // Operation
    int newValue = currentValue <<  1;

    // betroffene Flags prüfen und setzen/löschen
    checkCarryFlag(newValue);

    writeBack(file, newValue, storeInFileRegister);

    cycles++;
}

void Prozessor::rrf(int command)
{
    bool storeInFileRegister = (CHECK_BIT(command,7));

    //      01 1100 dfff ffff
    //  &   00 0000 0111 1111  = 0x7F
    //      00 0000 0fff ffff
    int file = command & 0x7F;

    // Register laden
    int currentValue = speicher.read(file);
    if(currentValue== 0x0100) //die Speicheradresse ist nicht belegt!!
    {
        cycles++;
        return;
    }

    // Operation
    int newValue = currentValue >>  1;

    // betroffene Flags prüfen und setzen/löschen
    checkCarryFlagRRF(currentValue);

    writeBack(file, newValue, storeInFileRegister);

    cycles++;
}

void Prozessor::subwf(int command)
{
    bool storeInFileRegister = (CHECK_BIT(command,7));

    //      00 0010 dfff ffff
    //  &   00 0000 0111 1111  = 0x7F
    //      00 0000 0fff ffff
    int file = command & 0x7F;

    // Register laden
    int currentValue = speicher.read(file);
    int workingRegisterValue = speicher.readW();
    if(currentValue== 0x0100) //die Speicheradresse ist nicht belegt!!
    {
        cycles++;
        return;
    }

    // Rechenoperation
    int newValue = currentValue - workingRegisterValue;

    // betroffene Flags prüfen und setzen/löschen
    checkCarryFlag(newValue);
    checkDigitCarryFlagSubtraktion(currentValue, workingRegisterValue);
    checkZeroFlag(newValue);

    writeBack(file, newValue, storeInFileRegister);

    cycles++;
}

void Prozessor::swapf(int command)
{
    bool storeInFileRegister = (CHECK_BIT(command,7));

    //      01 01bb bfff ffff
    //  &   00 0000 0111 1111  = 0x7F
    //      00 0000 0fff ffff
    int file = command & 0x7F;

    // Register laden
    int currentValue = speicher.read(file);
    if(currentValue== 0x0100) //die Speicheradresse ist nicht belegt!!
    {
        cycles++;
        return;
    }

    // Rechenoperation
    int upperNibble = currentValue & 0xF0;
    int lowerNibble = currentValue & 0x0F;

    upperNibble = upperNibble >> 4;
    lowerNibble = lowerNibble << 4;

    int newValue = upperNibble + lowerNibble;
    // Rechenoperation - ENDE

    writeBack(file, newValue, storeInFileRegister);

    cycles++;
}

void Prozessor::xorwf(int command)
{
    bool storeInFileRegister = (CHECK_BIT(command,7));

    //      00 0110 dfff ffff
    //  &   00 0000 0111 1111  = 0x7F
    //      00 0000 0fff ffff
    int file = command & 0x7F;

    // Register laden
    int currentValue = speicher.read(file);
    int workingRegisterValue = speicher.readW();
    if(currentValue== 0x0100) //die Speicheradresse ist nicht belegt!!
    {
        cycles++;
        return;
    }

    // Rechenoperation
    int newValue = currentValue ^ workingRegisterValue;

    // betroffene Flags prüfen und setzen/löschen
    checkZeroFlag(newValue);

    writeBack(file, newValue, storeInFileRegister);

    cycles++;
}


void Prozessor::bcf(int command)
{
    //cout << " BCF ";

    int bit;
    int file;

    int currentValue;

    //      01 00bb bfff ffff
    //  &   00 0011 1000 0000  = 0x380
    //      00 00bb b000 0000
    //  >>  00 0000 0000 0bbb

    // Befehl laden
    bit = command & 0x380;
    bit = bit >> 7;


    //      01 01bb bfff ffff
    //  &   00 0000 0111 1111  = 0x7F
    //      00 0000 0fff ffff

    // Register laden
    file = command & 0x7F;

    currentValue = speicher.read(file);
    if(currentValue== 0x0100) //die Speicheradresse ist nicht belegt!!
    {
        cycles++;
        return;
    }

    // Rechenoperation
    int newValue = currentValue & (~(1 << bit));

    writeBack(file, newValue, WRITE_TO_FILE_REGISTER);

    cycles++;
}

void Prozessor::bsf(int command)
{
    //cout << " BSF ";

    int bit;
    int file;

    int actualValue;

    //      01 01bb bfff ffff
    //  &   00 0011 1000 0000  = 0x380
    //      00 00bb b000 0000
    //  >>  00 0000 0000 0bbb

    bit = command & 0x380;
    bit = bit >> 7;


    //      01 01bb bfff ffff
    //  &   00 0000 0111 1111  = 0x7F
    //      00 0000 0fff ffff

    file = command & 0x7F;

    actualValue = speicher.read(file);
    if(actualValue== 0x0100) //die Speicheradresse ist nicht belegt!!
    {
        cycles++;
        return;
    }

    int newValue = actualValue | (1 << bit);
    writeBack(file,newValue,WRITE_TO_FILE_REGISTER);
    //speicher.write(file,newValue);
    cycles++;
}

void Prozessor::btfsc(int command, Steuerwerk* steuerwerk)
{
    int bit, file, actualValue;
    //      01 01bb bfff ffff
    //  &   00 0011 1000 0000  = 0x380
    //      00 00bb b000 0000
    //  >>  00 0000 0000 0bbb

    bit = command & 0x380;
    bit = bit >> 7;

    //      01 01bb bfff ffff
    //  &   00 0000 0111 1111  = 0x7F
    //      00 0000 0fff ffff

    file = command & 0x7F;

    actualValue = speicher.read(file);
    if(actualValue== 0x0100) //die Speicheradresse ist nicht belegt!!
    {
        cycles++;
        return;
    }

    if(actualValue & (1<<bit))
    {
        //Bit ist 1
        cycles++;
    }
    else
    {
        //Bit ist 0 -> nächster Befehl wird übersprungen
        steuerwerk->pc++;
        cycles+=2;
    }
}

void Prozessor::btfss(int command, Steuerwerk* steuerwerk)
{
    int bit, file, actualValue;
    //      01 11bb bfff ffff
    //  &   00 0011 1000 0000  = 0x380
    //      00 00bb b000 0000
    //  >>  00 0000 0000 0bbb

    bit = command & 0x380;
    bit = bit >> 7;

    //      01 11bb bfff ffff
    //  &   00 0000 0111 1111  = 0x7F
    //      00 0000 0fff ffff

    file = command & 0x7F;

    actualValue = speicher.read(file);
    if(actualValue== 0x0100) //die Speicheradresse ist nicht belegt!!
    {
        cycles++;
        return;
    }

    if(actualValue & (1<<bit))
    {
        //Bit ist 1 -> nächster Befehl wird übersprungen
        steuerwerk->pc++;
        cycles+=2;
    }
    else
    {
        //Bit ist 0
        cycles++;
    }
}

void Prozessor::addlw(int command)
{
    //      11 111x kkkk kkkk
    //  &   00 0000 1111 1111  = 0x00FF
    //      00 0000 kkkk kkkk
    int literal = command & 0xFF;

    // Register laden
    int valueW = speicher.readW();

    // Rechenoperation
    int newValue = valueW + literal;

    // Betroffene Flags setzen/löschen
    checkCarryFlag(newValue);
    checkDigitCarryFlagAddition(literal, valueW);
    checkZeroFlag(newValue);

    writeBackToW(newValue);

    cycles++;
}

void Prozessor::andlw(int command)
{
    //      11 1001 kkkk kkkk
    //  &   00 0000 1111 1111  = 0x00FF
    //      00 0000 kkkk kkkk
    int literal = command & 0xFF;

    // Register laden
    int valueW = speicher.readW();

    // Rechenoperation
    int newValue = valueW & literal;

    // Betroffene Flags setzen/löschen
    checkZeroFlag(newValue);

    writeBackToW(newValue);

    cycles++;
}


void Prozessor::call(int command, Steuerwerk* steuerwerk)
{
    //      10 0kkk kkkk kkkk
    //  &   00 0111 1111 1111  = 0x07FF
    //      00 0000 kkkk kkkk
	int address = (command & 0x7FF)+(  (int)(steuerwerk->pc -steuerwerk->maschinencode.begin())   &  0x1800) ; //Der Programmcounter hat 13 BIT die 2 fehlenden oberen Bits werden aus dem aktuellen PC extrahiert
	//PCLATH ist jetz in Adresse integriert

    // Push PC+1 to stack
	vector<Codeline>::iterator stackAddress = steuerwerk->pc + 1;
    steuerwerk->picStack.push(stackAddress);



	vector<Codeline>::iterator subroutineAddress = steuerwerk->maschinencode.begin() + address - 1 ;//PC wird noch um inkrementiert

    steuerwerk->pc = subroutineAddress;

    cycles+=2;
}

void Prozessor::clrwdt(Steuerwerk* steuerwerk)
{
    steuerwerk->wdt=0;
    cycles++;
    speicher.writeOnBank(0,3,speicher.readOnBank(0,3)|0x0018); //set TO und PD Bit in Status register
}

void Prozessor::go_to(int command, Steuerwerk* steuerwerk)
{
    //      10 1fff ffff ffff
    //  &   00 0111 1111 1111  = 0x7FF
    //      00 0fff ffff ffff

	int sprungAdresse = (command & 0x7FF)+(  (int)(steuerwerk->pc -steuerwerk->maschinencode.begin())   &  0x1800) ; //Der Programmcounter hat 13 BIT die 2 fehlenden oberen Bits werden aus dem aktuellen PC extrahiert

    /*
     *  Da in Steuerwerk::executeStep() der PC nach dem Ausführen inkrementiert wird,
     *  muss die neue Adresse im PC eine Stelle vor die gewünschte gesetzt werden.
     */

    steuerwerk->pc = steuerwerk->maschinencode.begin() + sprungAdresse - 1;

    /*
     *  Es muss nach jedem Befehlszyklus geprüft werden,
     *  ob der Timer gesetzt werden soll, da sonst Inkrements
     *  von TMR0 verpasst werden können.
     */
    cycles++;
    steuerwerk->checkTimer0();
    cycles++;
}

void Prozessor::iorlw(int command)
{
    //  11 1000 kkkk kkkk
    //& 00 0000 1111 1111 = 0x00ff
    //= 00 0000 kkkk kkkk
    int literal = command & 0xFF;

    int workingRegisterValue = speicher.readW();

    // Operation
    int newValue = workingRegisterValue | literal;

    // betroffene Flags prüfen und setzen/löschen
    checkZeroFlag(newValue);

    writeBackToW(newValue);
    cycles++;
}

void Prozessor::movlw(int command)
{
    //  11 00xx kkkk kkkk
    //& 00 0000 1111 1111 = 0x00ff
    //= 00 0000 kkkk kkkk
    int literal = command & 0xFF;

    // Operation
    int newValue = literal;

    writeBackToW(newValue);

    cycles++;
}

void Prozessor::retfie(Steuerwerk* steuerwerk)
{
    //GIE aktivieren
    int newValue = speicher.readOnBank(0,0xb) | 0x0080;
    speicher.writeOnBank(0,0xb,newValue);
    if(steuerwerk->picStack.empty())
    {
        cycles+=2;
        return;
    }
    steuerwerk->pc = (steuerwerk->picStack.top()) -1;
    steuerwerk->picStack.pop();
    cycles += 2;
}

void Prozessor::retlw(int command, Steuerwerk* steuerwerk)
{


    if(steuerwerk->picStack.empty())
		return;
	// 11 01xx kkkk kkkk
    writeBackToW(command & 0x00ff);
    //speicher.writeW(command & 0x00ff);

    steuerwerk->pc = (steuerwerk->picStack.top()) -1;
    steuerwerk->picStack.pop();

	cycles += 2;
}

void Prozessor::preturn(Steuerwerk* steuerwerk)
{
    if(steuerwerk->picStack.empty())
    {
        cycles+=2;
		return;
    }
    steuerwerk->pc = (steuerwerk->picStack.top()) -1;
    steuerwerk->picStack.pop();
	cycles += 2;
}

void Prozessor::psleep(Steuerwerk* steuerwerk)
{
    QThread* workerThread = new QThread();
    SleepKlasse* worker = new SleepKlasse(steuerwerk);
    //worker object in Threadkontext verschieben
    worker->moveToThread(workerThread);
    //arbeit starten
    workerThread->start();
    QMetaObject::invokeMethod(worker, "run", Qt::QueuedConnection);

}

void Prozessor::sublw(int command)
{
    //  11 110x kkkk kkkk
    //& 00 0000 1111 1111 = 0x00ff
    //= 00 0000 kkkk kkkk
    int literal = command & 0xFF;

    // Register laden
    int workingRegisterValue = speicher.readW();

    int newValue = literal - workingRegisterValue;

    // betroffene Flags prüfen und setzen/löschen
    checkCarryFlag(newValue);
    checkDigitCarryFlagSubtraktion(literal, workingRegisterValue);  // ?? prüfen!
    checkZeroFlag(newValue);

    writeBackToW(newValue);

    cycles++;
}

void Prozessor::xorlw(int command)
{
    //  11 1010 kkkk kkkk
    //& 00 0000 1111 1111 = 0x00ff
    //= 00 0000 kkkk kkkk
    int literal = command & 0xFF;

    // Register laden
    int workingRegisterValue = speicher.readW();

    // Operation
    int newValue = workingRegisterValue ^ literal;

    // betroffene Flags prüfen und setzen/löschen
    checkZeroFlag(newValue);

    writeBackToW(newValue);

    cycles++;
}

void Prozessor::checkCarryFlag(int result)
{
    if(CHECK_BIT(result, 8))
        speicher.setCBit();
    else
        speicher.clearCBit();
}

void Prozessor::checkCarryFlagRRF(int result)
{
    if(CHECK_BIT(result, 0))
        speicher.setCBit();
    else
        speicher.clearCBit();
}

void Prozessor::checkDigitCarryFlagAddition(int x, int y)
{
    int lowerNibbleResult = (x & 0x0F) + (y & 0x0F);
    if(lowerNibbleResult > 0x0F)
        speicher.setDCBit();
    else
        speicher.clearDCBit();
}

void Prozessor::checkDigitCarryFlagSubtraktion(int x, int y)
{
    int lowerNibbleResult = (x & 0x0F) - (y & 0x0F);
    if(lowerNibbleResult > 0x0F)
        speicher.clearDCBit();
    else
        speicher.setDCBit();
}

void Prozessor::checkZeroFlag(int result)
{
    if((result & 0xFF) == 0)
        speicher.setZBit();
    else
        speicher.clearZBit();
}

void Prozessor::writeBack(int file, int result, bool storeInFileRegister)
{
    result &= 0xFF;

    if(storeInFileRegister)
        speicher.write(file, result);
    else
        speicher.writeW(result);
}

void Prozessor::writeBackToW(int result)
{
    result &= 0xFF;
    speicher.writeW(result);
}

void Prozessor::setTimePerCycle(double value)
{
    timePerCycle = value;
}

double Prozessor::computeRuntime(void)
{
    //cout << "cycles: " << cycles << " timePerCycle: "<<timePerCycle<<endl;
    return cycles * timePerCycle;
}

int Prozessor::getCycles()
{
    return cycles;
}
