#ifndef PROZESSOR_H
#define PROZESSOR_H

#include "speicher.h"
#include <vector>
#include "codeline.h"

class Prozessor
{
public:
    Speicher speicher;

    Prozessor(void);
    ~Prozessor(void);

    void execute(int);

    void bsf(int);
    void goto_alu(int, std::vector<Codeline>::iterator*, std::vector<Codeline>);

};

#endif // PROZESSOR_H
