#ifndef CODELINE_H
#define CODELINE_H

class Codeline
{
public:
    int textzeile;
    int command;
	bool breakpoint;

    Codeline(void);
    Codeline(int, int);
    ~Codeline(void);
};

#endif // CODELINE_H
