#include "prozessor.h"
#include <iostream>

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


void Prozessor::goto_alu(int command, std::vector<Codeline>::iterator* pc, std::vector<Codeline> maschinencode)
{
    cout << " GOTO" << endl;

    int sprungmarke;

    //      10 1kkk kkkk kkkk
    //  &   00 0111 1111 1111 = 0x7FF

    sprungmarke = command & 0x7FF;

    cout << " pc wird geändert: Adresse: " << sprungmarke << endl;

    cout << "cmd vor GOTO: " << (*pc)->command << endl;

    *pc = maschinencode.begin() + sprungmarke - 1;

    cout << "cmd nach GOTO: " << (*pc)->command << endl;

    cout << "geändert" << endl;
}
