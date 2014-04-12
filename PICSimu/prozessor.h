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

    void bcf(int command);
    void bsf(int);
    void go_to(int, Steuerwerk*);
    void nop (void);
    void clrf(int);

};

#endif // PROZESSOR_H
