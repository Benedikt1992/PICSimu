#include "prozessor.h"
#include "steuerwerk.h"
#include <iostream>
#include <vector>

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

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

void Prozessor::addwf(int command)
{
    bool storeInFileRegister = (CHECK_BIT(command,7));

    //      00 0111 dkkk kkkk
    //  &   00 0000 0111 1111  = 0x7F
    //      00 0000 0kkk kkkk
    int file = command & 0x7F;

    int currentValue = speicher.read(file);
    int workingRegisterValue = speicher.readW();

    int newValue = currentValue + workingRegisterValue;

    // Überlauf prüfen und entsprechend Carry setzen/löschen
    if(CHECK_BIT(newValue, 8))
        speicher.setCBit();
    else
        speicher.clearCBit();

    // Ergebnis auf 0 prüfen und entsprechend ZERO setzen/löschen
    if((newValue & 0xFF) == 0)
        speicher.setZBit();
    else
        speicher.clearZBit();

    // Überlauf durch unteres Nibble prüfen, entpsrechend DC setzen/löschen
    int lowerNibbleResult = (currentValue & 0x0F) + (workingRegisterValue & 0x0F);
    if(lowerNibbleResult > 0x0F)
        speicher.setDCBit();
    else
        speicher.clearDCBit();

    if(storeInFileRegister)
        speicher.write(file, newValue);
    else
        speicher.writeW(newValue);

    cycles++;
}

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

void Prozessor::swapf(int command)
{
    bool storeInFileRegister = (CHECK_BIT(command,7));

    //      01 01bb bfff ffff
    //  &   00 0000 0111 1111  = 0x7F
    //      00 0000 0fff ffff
    int file = command & 0x7F;

    int currentValue = speicher.read(file);

    if(currentValue == 0x0100)
        return;

    int upperNibble = currentValue & 0xF0;
    int lowerNibble = currentValue & 0x0F;

    upperNibble >> 4;
    lowerNibble << 4;

    int newValue = upperNibble + lowerNibble;

    if(storeInFileRegister)
        speicher.write(file, newValue);
    else
        speicher.writeW(newValue);

    cycles++;
}

void Prozessor::bcf(int command)
{
    //cout << " BCF ";

    int bit;
    int file;

    int currentValue;

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

    currentValue = speicher.read(file);
    if(currentValue== 0x0100) //die Speicheradresse ist nicht belegt!!
        return;

    int newValue = currentValue & (~(1 << bit));
    speicher.write(file,newValue);
    cycles++;
}

void Prozessor::bsf(int command)
{
    //cout << " BSF ";

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

void Prozessor::btfss(int command, Steuerwerk* steuerwerk)
{
    int bit, file, actualValue;
    //      01 11bb bfff ffff
    //  &   00 0011 1000 0000  = 0x380
    //      00 00bb b000 0000
    //  >>  00 0000 0000 0bbb

    bit = command & 0x380;
    bit = bit >> 7;

    //      01 11bb bfff ffff
    //  &   00 0000 0111 1111  = 0x7F
    //      00 0000 0fff ffff

    file = command & 0x7F;

    actualValue = speicher.read(file);
    if(actualValue== 0x0100) //die Speicheradresse ist nicht belegt!!
        return;

    if(actualValue & (1<<bit))
    {
        //Bit ist 1 -> nächster Befehl wird übersprungen
        steuerwerk->pc++;
        cycles+=2;
    }
    else
    {
        //Bit ist 0
        cycles++;
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

void Prozessor::xorlw(int command)
{

    //  11 1010 kkkk kkkk
    //& 00 0000 1111 1111 = 0x00ff
    //= 00 0000 kkkk kkkk
    int newValue= speicher.readW() ^ (command & 0x00ff);
    newValue &= 0x00ff; // der Speicher ist nur 8 Bit breit
    speicher.writeW(newValue);
    if(!newValue)
        speicher.setZBit();
    else
        speicher.clearZBit();
    cycles++;
}


