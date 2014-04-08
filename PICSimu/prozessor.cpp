#include "prozessor.h"
#include "steuerwerk.h"
#include <iostream>
#include <vector>

using namespace std;

Prozessor::Prozessor(void)
{
}


Prozessor::~Prozessor(void)
{
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

void Prozessor::go_to(int command, Steuerwerk* steuerwerk)
{
    int sprungAdresse = command & 0x7FF;

    steuerwerk->pc = steuerwerk->maschinencode.begin() + sprungAdresse - 1;
}
