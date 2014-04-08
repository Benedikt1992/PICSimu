#ifndef PROZESSOR_H
#define PROZESSOR_H

#include "speicher.h"

class Steuerwerk;

class Prozessor
{
public:
    Speicher speicher;

    Prozessor(void);
    ~Prozessor(void);

    void execute(int);

    void bsf(int);
    void go_to(int, Steuerwerk*);

};

#endif // PROZESSOR_H
