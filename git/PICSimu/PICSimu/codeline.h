#ifndef CODELINE_H
#define CODELINE_H

class Codeline
{
public:
    int address;
    int command;

    Codeline(void);
    Codeline(int, int);
    ~Codeline(void);
};

#endif // CODELINE_H
