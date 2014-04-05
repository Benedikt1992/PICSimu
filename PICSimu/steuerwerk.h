#ifndef STEUERWERK_H
#define STEUERWERK_H

#include "prozessor.h"
#include "codeline.h"
#include <list>

using namespace std;

class Steuerwerk
{
public:
    Prozessor mainProcessor;
    list<Codeline> sourcecode;
    list<Codeline>::iterator pc;

    Steuerwerk(void);
    ~Steuerwerk(void);

    bool executeStep(void);
    void run(void);
};



#endif // STEUERWERK_H
#ifndef STEUERWERK_H
#define STEUERWERK_H

#endif // STEUERWERK_H
