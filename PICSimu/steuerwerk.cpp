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

bool Steuerwerk::loadFile(string filename)
{
    return Parser::auslesen(&lstFile, filename, this);
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
    if(pc != maschinencode.end())
    {
        execute(pc->command);
        pc++;
    }
    else
        return false;

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
    // ...
    // NOP
    if( command == 0 )
        cout << " NOP" << endl;
    // ...
    // SWAPF
    // XORWF


    // BIT-ORIENTED FILE REGISTER OPERATIONS

    // BCF
    // BSF
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
int* Steuerwerk::getBank(int bank){
    if(bank == 0)
        return alu->speicher.bank0;

    if(bank == 1)
        return alu->speicher.bank1;

    return 0;
}
// Mario ende

bool Steuerwerk::programmEndeErreicht()
{
    if(pc == maschinencode.end())
        return true;

    return false;
}
