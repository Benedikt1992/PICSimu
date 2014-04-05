#include "prozessor.h"
#include <iostream>

using namespace std;

Prozessor::Prozessor(void)
{
}


Prozessor::~Prozessor(void)
{
}

void Prozessor::execute(int command)
{
    command = command & 0x3FFF;

    // BYTE-ORIENTED FILE REGISTER OPERATIONS

    // ADDWF
    // ANDWF
    // ...
    // NOP
    if( command == 0 )
        cout << " NOP";
    // ...
    // SWAPF
    // XORWF


    // BIT-ORIENTED FILE REGISTER OPERATIONS

    // BCF
    // BSF
    if( (command & 0x3C00) == 0x1400 )
        bsf(command);
    // BTFSC
    // BTFSS


    // LITERAL AND CONTROL OPERATIONS

    // ADDLW
    // ANDLW
    // CALL
    // CLRWDT
    // GOTO
    if( (command & 0x3800) == 0x2800 )
        cout << " GOTO";
    // ...
    // SUBLW
    // XORLW
}


void Prozessor::bsf(int command)
{
    cout << " BSF ";

    int bit;
    int file;

    int* ref;

    bit = command & 0x380;
    bit = bit >> 7;

    file = command & 0x7F;

    ref= speicher.getFileReference(file);
    *ref |= 1 << bit;
}

