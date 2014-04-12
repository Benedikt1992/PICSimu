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

    void clrf(int);
    void clrw();
    void nop (void);
    void bcf(int command);
    void bsf(int);
    void btfsc(int command, Steuerwerk* steuerwerk);
    void btfss(int command, Steuerwerk* steuerwerk);
    void go_to(int, Steuerwerk*);
    void xorlw(int command);

};

#endif // PROZESSOR_H
