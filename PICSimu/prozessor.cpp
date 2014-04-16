#include "prozessor.h"
#include "steuerwerk.h"
#include <iostream>
#include <vector>

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))
#define WRITE_TO_FILE_REGISTER 1
#define WRITE_TO_WORKING_REGISTER 0

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

	//      00 0111 dfff ffff
    //  &   00 0000 0111 1111  = 0x7F
    //      00 0000 0kkk kkkk
    int file = command & 0x7F;

    // Register laden
    int currentValue = speicher.read(file);
    if(currentValue== 0x0100) //die Speicheradresse ist nicht belegt!!
        return;

    int workingRegisterValue = speicher.readW();

    // Rechenoperation
    int newValue = currentValue + workingRegisterValue;

    // betroffene Flags setzen/löschen
    checkCarryFlag(newValue);
    checkZeroFlag(newValue);
	checkDigitCarryFlagAddition(currentValue, workingRegisterValue);

    writeBack(file, newValue, storeInFileRegister);

    cycles++;
}

void Prozessor::andwf(int command)
{
    bool storeInFileRegister = (CHECK_BIT(command,7));

    //      00 0101 dkkk kkkk
    //  &   00 0000 0111 1111  = 0x7F
    //      00 0000 0kkk kkkk
    int file = command & 0x7F;

    // Register laden
    int currentValue = speicher.read(file);
    if(currentValue== 0x0100) //die Speicheradresse ist nicht belegt!!
        return;

    int workingRegisterValue = speicher.readW();

    // Rechenoperation
    int newValue = currentValue & workingRegisterValue;

    // betroffene Flags setzen/löschen
    checkZeroFlag(newValue);

    writeBack(file, newValue, storeInFileRegister);

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

void Prozessor::comf(int command)
{
    bool storeInFileRegister = (CHECK_BIT(command,7));

    //      01 1001 dfff ffff
    //  &   00 0000 0111 1111  = 0x7F
    //      00 0000 0fff ffff
    int file = (command & 0x7F);

    // Register laden
    int currentValue = speicher.read(file);
    if(currentValue== 0x0100) //die Speicheradresse ist nicht belegt!!
        return;

    // Rechenoperation
    int newValue = ~currentValue;

    // betroffene Flags setzen/löschen
    checkZeroFlag(newValue);

    writeBack(file, newValue, storeInFileRegister);

    cycles++;
}

void Prozessor::decf(int command)
{
    bool storeInFileRegister = (CHECK_BIT(command,7));

	//      00 0011 dfff ffff
    //  &   00 0000 0111 1111  = 0x7F
    //      00 0000 0fff ffff
    int file = (command & 0x7F);

    int currentValue = speicher.read(file);
    if(currentValue== 0x0100) //die Speicheradresse ist nicht belegt!!
        return;

    // Rechenoperation
    int newValue = currentValue - 1;

    // betroffene Flags prüfen
    checkZeroFlag(newValue);

    writeBack(file, newValue, storeInFileRegister);

    cycles++;
}

void Prozessor::decfsz(int command, Steuerwerk *steuerwerk)
{
    bool storeInFileRegister = (CHECK_BIT(command,7));

    //      00 1011 dfff ffff
    //  &   00 0000 0111 1111  = 0x7F
    //      00 0000 0fff ffff
    int file = (command & 0x7F);

    int currentValue = speicher.read(file);
    if(currentValue== 0x0100) //die Speicheradresse ist nicht belegt!!
        return;

    // Rechenoperation
    int newValue = currentValue - 1;

    // keine Flags im Status-Register betroffen

    writeBack(file, newValue, storeInFileRegister);

    if((newValue & 0xFF) == 0)
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

void Prozessor::incf(int command)
{
    bool storeInFileRegister = (CHECK_BIT(command,7));

    //      00 1010 dfff ffff
    //  &   00 0000 0111 1111  = 0x7F
    //      00 0000 0fff ffff
    int file = command & 0x7F;

    // Register laden
    int currentValue = speicher.read(file);
    if(currentValue== 0x0100) //die Speicheradresse ist nicht belegt!!
        return;

    // Rechenoperation
    int newValue = currentValue + 1;

    // betroffene Flags prüfen und setzen/löschen
    checkZeroFlag(newValue);

    writeBack(file, newValue, storeInFileRegister);
    cycles++;
}

void Prozessor::incfsz(int command, Steuerwerk *steuerwerk)
{
    bool storeInFileRegister = (CHECK_BIT(command,7));

    //      00 1011 dfff ffff
    //  &   00 0000 0111 1111  = 0x7F
    //      00 0000 0fff ffff
    int file = (command & 0x7F);

    int currentValue = speicher.read(file);
    if(currentValue== 0x0100) //die Speicheradresse ist nicht belegt!!
        return;

    // Rechenoperation
    int newValue = currentValue + 1;

    // keine Flags im Status-Register betroffen

    writeBack(file, newValue, storeInFileRegister);

    if((newValue & 0xFF) == 0)
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

void Prozessor::movf(int command)
{
    bool storeInFileRegister = (CHECK_BIT(command,7));

    //      00 1000 dfff ffff
    //  &   00 0000 0111 1111  = 0x7F
    //      00 0000 0fff ffff
    int file = command & 0x7F;

    // Register laden
    int currentValue = speicher.read(file);
    if(currentValue== 0x0100) //die Speicheradresse ist nicht belegt!!
        return;

    // Operation
    int newValue = currentValue;

    writeBack(file, newValue, storeInFileRegister);

    cycles++;
}

void Prozessor::movwf(int command)
{
    //      00 0000 1fff ffff
    //  &   00 0000 0111 1111  = 0x7F
    //      00 0000 0fff ffff
    int file = command & 0x7F;

    int currentValue = speicher.read(file);
    if(currentValue== 0x0100) //die Speicheradresse ist nicht belegt!!
        return;

    // Operation
    int newValue = currentValue;

    writeBack(file, newValue, WRITE_TO_FILE_REGISTER);

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

    // Register laden
    int currentValue = speicher.read(file);
    if(currentValue== 0x0100) //die Speicheradresse ist nicht belegt!!
        return;

    // Rechenoperation
    int upperNibble = currentValue & 0xF0;
    int lowerNibble = currentValue & 0x0F;

    upperNibble = upperNibble >> 4;
    lowerNibble = lowerNibble << 4;

    int newValue = upperNibble + lowerNibble;
    // Rechenoperation - ENDE

    writeBack(file, newValue, storeInFileRegister);

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

    // Befehl laden
    bit = command & 0x380;
    bit = bit >> 7;


    //      01 01bb bfff ffff
    //  &   00 0000 0111 1111  = 0x7F
    //      00 0000 0fff ffff

    // Register laden
    file = command & 0x7F;

    currentValue = speicher.read(file);
    if(currentValue== 0x0100) //die Speicheradresse ist nicht belegt!!
        return;

    // Rechenoperation
    int newValue = currentValue & (~(1 << bit));

    writeBack(file, newValue, WRITE_TO_FILE_REGISTER);

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

void Prozessor::addlw(int command)
{
    //      11 111x kkkk kkkk
    //  &   00 0000 1111 1111  = 0x00FF
    //      00 0000 kkkk kkkk
    int literal = command & 0xFF;

    // Register laden
    int valueW = speicher.readW();

    // Rechenoperation
    int newValue = valueW + literal;

    // Betroffene Flags setzen/löschen
    checkCarryFlag(newValue);
    checkDigitCarryFlagAddition(literal, valueW);
    checkZeroFlag(newValue);

    writeBackToW(newValue);

    cycles++;
}

void Prozessor::andlw(int command)
{
    //      11 1001 kkkk kkkk
    //  &   00 0000 1111 1111  = 0x00FF
    //      00 0000 kkkk kkkk
    int literal = command & 0xFF;

    // Register laden
    int valueW = speicher.readW();

    // Rechenoperation
    int newValue = valueW & literal;

    // Betroffene Flags setzen/löschen
    checkZeroFlag(newValue);

    writeBackToW(newValue);

    cycles++;
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
    cycles += 2;
}

void Prozessor::xorlw(int command)
{

    //  11 1010 kkkk kkkk
    //& 00 0000 1111 1111 = 0x00ff
    //= 00 0000 kkkk kkkk
    int newValue= speicher.readW() ^ (command & 0x00ff);
    newValue &= 0x00ff; // der Speicher ist nur 8 Bit breit
    speicher.writeW(newValue);

    checkZeroFlag(newValue);

    cycles++;
}

void Prozessor::checkCarryFlag(int result)
{
    if(CHECK_BIT(result, 8))
        speicher.setCBit();
    else
        speicher.clearCBit();
}

void Prozessor::checkDigitCarryFlagAddition(int x, int y)
{
    int lowerNibbleResult = (x & 0x0F) + (y & 0x0F);
    if(lowerNibbleResult > 0x0F)
        speicher.setDCBit();
    else
        speicher.clearDCBit();
}

void Prozessor::checkDigitCarryFlagSubtraktion(int x, int y)
{
    int lowerNibbleResult = (x & 0x0F) - (y & 0x0F);
    if(lowerNibbleResult > 0x0F)
        speicher.clearDCBit();
    else
        speicher.setDCBit();
}

void Prozessor::checkZeroFlag(int result)
{
    if((result & 0xFF) == 0)
        speicher.setZBit();
    else
        speicher.clearZBit();
}

void Prozessor::writeBack(int file, int result, bool storeInFileRegister)
{
    result &= 0xFF;

    if(storeInFileRegister)
        speicher.write(file, result);
    else
        speicher.writeW(result);
}

void Prozessor::writeBackToW(int result)
{
    result &= 0xFF;
    speicher.writeW(result);
}
