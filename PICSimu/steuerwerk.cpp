#include "steuerwerk.h"
#include "parser.h"
#include <iostream>
#include <stdio.h>

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

bool Steuerwerk::executeStep(void)
{
    if(pc != maschinencode.end())
    {
        cout << pc->command << " execute" << endl;
        alu->execute(pc->command);
        pc++;
    }
    else
        return true;
    return false;
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
