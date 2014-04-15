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
    void decfsz(int);   // unimplemented
    void incf(int);     // unimplemented
    void incfsz(int);   // unimplemented
    void iorwf(int);    // unimplemented
    void movf(int);     // unimplemented
    void movwf(int);    // unimplemented
    void nop (void);
    void rlf(int);      // unimplemented
    void rrf(int);      // unimplemented
    void subwf(int);    // unimplemented
    void swapf(int);
    void xorwf(int);    // unimplemented

    void bcf(int command);
    void bsf(int);
    void btfsc(int command, Steuerwerk* steuerwerk);
    void btfss(int command, Steuerwerk* steuerwerk);

    void go_to(int, Steuerwerk*);
    void xorlw(int command);

    void checkCarryFlag(int);
	void checkDigitCarryFlagAddition(int, int);
    void checkDigitCarryFlagSubtraktion(int, int);
    void checkZeroFlag(int);

    void writeBack(int, int, bool);
};

#endif // PROZESSOR_H
