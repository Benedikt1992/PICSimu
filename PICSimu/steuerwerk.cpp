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

    mainWindow->setLineColorWhite(getCurrentLineNumber()-1);

    if(pc != maschinencode.end())
    {
        execute(pc->command);
        pc++;
    }
    else
        return false;
    mainWindow->slotRefreshSpeicher();

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
    // ANDWF
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
    // NOP
    if( (command & 0x0f9f) == 0 )
        alu->nop();
    // ...
    // SWAPF
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
    // BTFSS


    // LITERAL AND CONTROL OPERATIONS

    // ADDLW
    // ANDLW
    // CALL
    // CLRWDT
    // GOTO
    if( (command & 0x3800) == 0x2800 )
        alu->go_to(command, this);
    // ...
    // SUBLW
    // XORLW
}


// Mario
int Steuerwerk::readForGUI(int bank,int file){
    //cout << "steuerwerk read" << endl;
    return alu->speicher.readForGUI(bank,file);
}
// Mario ende

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
