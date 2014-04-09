#include "speicher.h"
#include <iostream>

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

using namespace std;

Speicher::Speicher(void)
{
    // Registerinhalte mit 0 initialisieren
    for(int i = 0; i < n_register; i++)
        bank0[i] = bank1[i] = 0;

    // Mapping
    // ungemappte Register (z.B. 00h oder 07h) noch nicht berücksichtigt!!! - mit null-Zeiger abfangen?)
    // aktuell 2x47 Register als Speicherzellen verfügbar... Bank1 nutzt nur 5
    for(int i=0; i < n_register; i++)             // mapping Bank 0
        refBank[0][i] = &(bank0[i]);

    for(int i=0; i < n_register; i++)             // mapping Bank 1
        refBank[1][i] = &(bank0[i]);

    // undefinierte Register erhalten Null-Zeiger
    refBank[0][0] = 0;
    refBank[1][0] = 0;
    refBank[0][7] = 0;
    refBank[1][7] = 0;

    refBank[1][1] = &(bank1[1]);    // OPTION-Register
    refBank[1][5] = &(bank1[5]);    // TRISA-Register
    refBank[1][6] = &(bank1[6]);    // TRISB-Register
    refBank[1][8] = &(bank1[8]);    // EECON1-Register
    refBank[1][9] = &(bank1[9]);    // EECON2-Register
}

Speicher::~Speicher(void)
{
}

bool Speicher::clearSpeicher()
{
	//TODO muss noch angepasst werden (welche Ur-Zustände haben die SFRs?)
	for(int i = 0; i < n_register; i++)
		bank0[i] = bank1[i] = 0;
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

