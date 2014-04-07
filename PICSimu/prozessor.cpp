#include "prozessor.h"
#include <iostream>

using namespace std;

Prozessor::Prozessor(void)
{
}


Prozessor::~Prozessor(void)
{
}

// execute in Steuerwerk verschieben
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
    //      01 01bb bfff ffff = 0x1400
    //  &   11 1100 0000 0000 = 0x3C00
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

    ref = speicher.getFileReference(file);

    if(ref == 0)
        return;

    *ref |= (1 << bit);
}

