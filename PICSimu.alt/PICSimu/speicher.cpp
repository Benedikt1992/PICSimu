#include "speicher.h"
#include <iostream>

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

using namespace std;

Speicher::Speicher(void)
{
    // Dummy-Werte initialisiert, später mit 0 initialisieren!!!
    for(int i=0; i<47; i++)
        bank0[i] = i;

    for(int i=0; i<47; i++)
        bank1[i] = i+100;

    // Mapping
    // ungemappte Register (z.B. 00h oder 07h) noch nicht berücksichtigt!!! - mit null-Zeiger abfangen?)
    // aktuell 2x47 Register als Speicherzellen verfügbar... Bank1 nutzt nur 5
    for(int i=0; i<47; i++)
        refBank[0][i] = &(bank0[i]);

    for(int i=0; i<47; i++)
        refBank[0][i] = &(bank0[i]);

    for(int i=0; i<47; i++)
        refBank[1][i] = &(bank0[i]);

    refBank[0][0] = 0;
    refBank[1][0] = 0;
    refBank[0][7] = 0;
    refBank[1][7] = 0;

    refBank[1][1] = &(bank1[1]);

    refBank[1][5] = &(bank1[5]);

    refBank[1][6] = &(bank1[6]);

    refBank[1][8] = &(bank1[8]);

    refBank[1][9] = &(bank1[9]);
}

Speicher::~Speicher(void)
{
}

// nur zum Testen
void Speicher::ausgeben(int bank)
{
    cout << "Bank" << bank << endl;
    cout << "Zelle\t" << "Wert" << endl;
    for(int i=0; i<47; i++)
        if(refBank[bank][i] != 0)
            cout << i << "\t" << *(refBank[bank][i]) << endl;
        else
            cout << i << "\t" << "N/A" << endl;

    cout << "Bank COntent Ende!" << endl;
}

int* Speicher::getFileReference(int file)
{
    int bank;
    if(CHECK_BIT(bank1[3],5))
        bank = 1;
    else
        bank = 0;

    return refBank[bank][file];
}
