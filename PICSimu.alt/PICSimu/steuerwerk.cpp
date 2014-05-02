#include "steuerwerk.h"
#include <iostream>
#include <stdio.h>

Steuerwerk::Steuerwerk(void)
{
}


Steuerwerk::~Steuerwerk(void)
{
}

bool Steuerwerk::executeStep(void)
{
    if(pc != sourcecode.end())
    {
        cout << pc->command << " execute" << endl;
        mainProcessor.execute(pc->command);
        pc++;
    }
    else
        return true;
    return false;
}

void Steuerwerk::run(void)
{
    mainProcessor.speicher.ausgeben(0);
    getchar();
    mainProcessor.speicher.ausgeben(1);
    getchar();

    while(!executeStep())
        getchar();

    mainProcessor.speicher.ausgeben(0);
    getchar();
    mainProcessor.speicher.ausgeben(1);
    getchar();
}
