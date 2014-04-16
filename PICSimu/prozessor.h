#ifndef PROZESSOR_H
#define PROZESSOR_H

#include "speicher.h"

class Steuerwerk;

class Prozessor
{
public:
    Speicher speicher;
    int cycles;

    Prozessor(void);
    ~Prozessor(void);

    void execute(int);
	bool clearProzessor(void);

    void addwf(int);
    void andwf(int);
    void clrf(int);
    void clrw();
    void comf(int);
	void decf(int);
    void decfsz(int command, Steuerwerk* steuerwerk);
    void incf(int);
    void incfsz(int command, Steuerwerk* steuerwerk);
    void iorwf(int);
    void movf(int);
    void movwf(int);
    void nop (void);
    void rlf(int);
    void rrf(int);
    void subwf(int);
    void swapf(int);
    void xorwf(int);    // unimplemented

    void bcf(int command);
    void bsf(int);
    void btfsc(int command, Steuerwerk* steuerwerk);
    void btfss(int command, Steuerwerk* steuerwerk);

    void addlw(int command);
    void andlw(int command);
    void call(int command, Steuerwerk*);    // unimplemented
    void clrwdt(void);          // unimplemented
    void go_to(int, Steuerwerk*);
    void iorlw(int command);
    void movlw(int command);
    void retfie(Steuerwerk* steuerwerk);          // unimplemented
    void retlw(int command, Steuerwerk* steuerwerk);    // unimplemented
    void preturn(Steuerwerk* steuerwerk);             // unimplemented
    void psleep();              // unimplemented
    void sublw(int command);
    void xorlw(int command);

    void checkCarryFlag(int result);
    void checkCarryFlagRRF(int result);
    void checkDigitCarryFlagAddition(int x, int y);
    void checkDigitCarryFlagSubtraktion(int x, int y);
    void checkZeroFlag(int result);

    void writeBack(int file, int result, bool storeInFileRegister);
    void writeBackToW(int result);
};

#endif // PROZESSOR_H
