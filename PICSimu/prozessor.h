#ifndef PROZESSOR_H
#define PROZESSOR_H

#include "speicher.h"

class Prozessor
{
public:
    Speicher speicher;

    Prozessor(void);
    ~Prozessor(void);

    void execute(int);

    void bsf(int);
};

#endif // PROZESSOR_H
