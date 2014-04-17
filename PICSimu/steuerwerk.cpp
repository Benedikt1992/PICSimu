#include "steuerwerk.h"
#include "parser.h"
#include <iostream>
#include <stdio.h>
#include "codeline.h"

Steuerwerk::Steuerwerk(MainWindow* mainWindow)
{
    this->mainWindow = mainWindow;
    mainWindow->connectSteuerwerk(this);
    alu = new Prozessor();
}


Steuerwerk::~Steuerwerk(void)
{
}

bool Steuerwerk::clearSteuerwerk()
{
	if(!alu->clearProzessor()) // Prozessor resetten
		return false;
	if(0!=maschinencode.size())
		pc = maschinencode.begin(); // PC auf beginn setzen
	return true;

}

bool Steuerwerk::clearProgrammspeicher()
{
	lstFile.clear();
	maschinencode.clear();
	return true;
}

bool Steuerwerk::loadFile(string filename)
{
    if(Parser::auslesen(&lstFile, filename, this))
    {
        pc = maschinencode.begin();
        return true;
    }
    return false;

}


/*
 * Rückgabewert ist der neue Breakpointwert
 * true -> breakpoint wurde gesetzt
 * false -> breakpoint wurde entfernt
 */
bool Steuerwerk::toggleBreakpoint(int textzeile)
{
	for(vector<Codeline>::iterator it = maschinencode.begin(); it!=maschinencode.end(); it++)
	{
		if(it->textzeile==textzeile)
			return it->breakpoint = !it->breakpoint;
	}
	return false;
}

/*
 *  Gibt false zurück, wenn PC nicht mehr auf Codezeile zeigt,
 *  d.h. wenn das Programm am Ende angelangt ist.
 *
 *  Zeigt der PC auf einen gültigen Befehl, wird true zurückgegeben.
 */
bool Steuerwerk::executeStep(void)
{
    if(programmEndeErreicht())
        return true;
    if(pc->breakpoint)
        mainWindow->setLineColorRed(getCurrentLineNumber()-1);
    else
        mainWindow->setLineColorWhite(getCurrentLineNumber()-1);

    if(pc != maschinencode.end())
    {
        execute(pc->command);
        pc++;
		alu->speicher.writePC(pc - maschinencode.begin());
    }
    else
        return false;
    mainWindow->slotRefreshSpeicher();
    mainWindow->refreshSFRWidget();

    if(programmEndeErreicht())
        return true;

    mainWindow->setLineColorGreen(getCurrentLineNumber()-1);
    mainWindow->gotoLineNumber(getCurrentLineNumber()-1);
    return true;
}

void Steuerwerk::run(void)
{
    alu->speicher.ausgeben(0);
    getchar();
    alu->speicher.ausgeben(1);
    getchar();

    while(!executeStep())
        getchar();

    alu->speicher.ausgeben(0);
    getchar();
    alu->speicher.ausgeben(1);
    getchar();
}

void Steuerwerk::execute(int command)
{
    command = command & 0x3FFF;

    // BYTE-ORIENTED FILE REGISTER OPERATIONS

    // ADDWF
	//	00 0111 dfff ffff == 0x0700
	//&	11 1111 0000 0000 == 0x3f00
	if((command&0x3f00)==0x0700)
		alu->addwf(command);

    // ANDWF
	//	00 0101 dfff ffff = 0x0500
	//&	11 1111 0000 0000 = 0x3f00
	if((command&0x3f00)==0x0500)
		alu->andwf(command);

    // CLRF
    //  00 0001 1fff ffff == 0c0180
    //& 11 1111 1000 0000 == 0x3f80
    if( (command&0x3f80) == 0x0180)
            alu->clrf(command);
    //CLRW
    //  00 0001 0xxx xxxx = 0x0100
	//& 11 1111 1000 0000 = 0x3F80
    if((command & 0x3f80)== 0x0100)
        alu->clrw();
	//COMF
	//	00 1001 dfff ffff = 0x0900
	//&	11 1111 0000 0000 = 0x3f00
	if((command&0x3f00)==0x0900)
			alu->comf(command);

	//DECF
	//	00 0011 dfff ffff = 0x0300
	//&	11 1111 0000 0000 = 0x3f00
	if((command&0x3f00)==0x0300)
		alu->decf(command);

	//DECFSZ
	//	00 1011 dfff ffff
	//& 11 1111 0000 0000

	//INCF
	//	00 1010 dfff ffff
	//	11 1111 0000 0000

	//INCFSZ
	//	00 1111 dfff ffff
	//	11 1111 0000 0000

	//IORWF
	//	00 0100 dfff ffff
	//	11 1111 0000 0000

	//MOVF
	//	00 1000 dfff ffff
	//	11 1111 0000 0000

	//MOVWF
	//	00 0000 1fff ffff
	//	11 1111 1000 0000
    if( (command & 0x3F80) == 0x0080)
        cout << "MOVWF" << endl;
    // NOP
    if( (command & 0x0f9f) == 0 )
        alu->nop();
	//RLF
	//	00 1101 dfff ffff
	//	11 1111 0000 0000

	//RRF
	//	00 1100 dfff ffff
	//	11 1111 0000 0000

	//SUBWF
	//	00 0010 dfff ffff
	//	11 1111 0000 0000

    // SWAPF
    //  00 1110 dfff ffff = 0xE00
    //  11 1111 0000 0000 = 0x3F00
    if((command & 0x3F00) == 0xE00)
        alu->swapf(command);

    // XORWF


    // BIT-ORIENTED FILE REGISTER OPERATIONS

    // BCF
    //  01 00bb bfff ffff = 0x1000
    //& 11 1100 0000 0000 = 0x3C00
    if ((command&0x3C00)==0x1000)
        alu->bcf(command);

    //BSF
    //      01 01bb bfff ffff = 0x1400
    //  &   11 1100 0000 0000 = 0x3C00
    if( (command & 0x3C00) == 0x1400 )
        alu->bsf(command);

    // BTFSC
    //  01 10bb bfff ffff = 0x1800
    //  11 1100 0000 0000 = 0x3C00
    if((command & 0x3c00)==0x1800)
            alu->btfsc(command, this);

    // BTFSS
    //  01 11bb bfff ffff = 0x1C00
    //  11 1100 0000 0000 = 0x3C00
    if((command & 0x3c00)==0x1c00)
            alu->btfss(command, this);


    // LITERAL AND CONTROL OPERATIONS

    // ADDLW
	//	11 111x kkkk kkkk
	//	11 1110 0000 0000

    // ANDLW
	//	11 1001 kkkk kkkk
	//	11 1111 0000 0000

    // CALL
	//	10 0kkk kkkk kkkk = 0x2000
	//	11 1000 0000 0000 = 0x3800
	if((command&0x3800)== 0x2000)
		alu->call(command,this);
    // CLRWDT
    // GOTO
    if( (command & 0x3800) == 0x2800 )
        alu->go_to(command, this);

    // ...
    // SUBLW
    // XORLW
    //  11 1010 kkkk kkkk = 0x3A00
    //& 11 1111 0000 0000 = 0x3F00
    if((command&0x3f00)==0x3a00)
        alu->xorlw(command);
}


// Mario
int Steuerwerk::readForGUI(int bank,int file){
    //cout << "steuerwerk read" << endl;
    return alu->speicher.readForGUI(bank,file);
}
// Mario ende

int Steuerwerk::readWForGUI()
{
    return alu->speicher.readW();
}

bool Steuerwerk::programmEndeErreicht()
{
    if(pc == maschinencode.end())
        return true;

    return false;
}

int Steuerwerk::getCurrentLineNumber()
{
    return pc->textzeile;
}

int Steuerwerk::getPCInt()
{
    return pc - maschinencode.begin();
}
