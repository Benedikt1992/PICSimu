#include "prozessor.h"
#include "steuerwerk.h"
#include <iostream>
#include <vector>

using namespace std;

Prozessor::Prozessor(void)
{
    cycles =0;
}


Prozessor::~Prozessor(void)
{
}

bool Prozessor::clearProzessor()
{
    cycles =0;
	return speicher.clearSpeicher();

}

/*
 *
 * Befehlsimplementierungen
 *
 */

void Prozessor::clrf(int command)
{

    // 00 0001 1fff ffff
    int file = command & 0x007f;
    speicher.write(file,0);

    speicher.setZBit();
    cycles++;

}
void Prozessor::clrw()
{
    speicher.writeW(0);
    speicher.setZBit();
    cycles++;
}

void Prozessor::nop()
{
    cycles++;
}
void Prozessor::bcf(int command)
{
    cout << " BCF ";

    int bit;
    int file;

    int actualValue;

    //      01 00bb bfff ffff
    //  &   00 0011 1000 0000  = 0x380
    //      00 00bb b000 0000
    //  >>  00 0000 0000 0bbb

    bit = command & 0x380;
    bit = bit >> 7;


    //      01 01bb bfff ffff
    //  &   00 0000 0111 1111  = 0x7F
    //      00 0000 0fff ffff

    file = command & 0x7F;

    actualValue = speicher.read(file);
    if(actualValue== 0x0100) //die Speicheradresse ist nicht belegt!!
        return;

    int newValue = actualValue & (~(1 << bit));
    speicher.write(file,newValue);
    cycles++;
}

void Prozessor::bsf(int command)
{
    cout << " BSF ";

    int bit;
    int file;

    int actualValue;

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

    actualValue = speicher.read(file);
    if(actualValue== 0x0100) //die Speicheradresse ist nicht belegt!!
        return;

    int newValue = actualValue | (1 << bit);
    speicher.write(file,newValue);
    cycles++;
}

void Prozessor::btfsc(int command, Steuerwerk* steuerwerk)
{
    int bit, file, actualValue;
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

    actualValue = speicher.read(file);
    if(actualValue== 0x0100) //die Speicheradresse ist nicht belegt!!
        return;

    if(actualValue & (1<<bit))
    {
        //Bit ist 1
        cycles++;
    }
    else
    {
        //Bit ist 0 -> nächster Befehl wird übersprungen
        steuerwerk->pc++;
        cycles+=2;
    }
}

void Prozessor::go_to(int command, Steuerwerk* steuerwerk)
{
    //      10 1fff ffff ffff
    //  &   00 0111 1111 1111  = 0x7FF
    //      00 0fff ffff ffff

    int sprungAdresse = command & 0x7FF;

    /*
     *  Da in Steuerwerk::executeStep() der PC nach dem Ausführen inkrementiert wird,
     *  muss die neue Adresse im PC eine Stelle vor die gewünschte gesetzt werden.
     */

    steuerwerk->pc = steuerwerk->maschinencode.begin() + sprungAdresse - 1;
    cycles +=2;
}


