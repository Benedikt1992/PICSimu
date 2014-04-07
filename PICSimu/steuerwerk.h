#ifndef STEUERWERK_H
#define STEUERWERK_H

#include "prozessor.h"
#include "codeline.h"
#include <list>
#include "mainwindow.h"

using namespace std;

class Steuerwerk
{
public:
    MainWindow* mainWindow;
    Prozessor* alu;
    list<string> lstFile;
    list<Codeline> maschinencode;
    list<Codeline>::iterator pc;

    Steuerwerk(MainWindow*);
    ~Steuerwerk(void);


    bool loadFile(string);
    bool executeStep(void);
    void run(void);
};



#endif // STEUERWERK_H
#ifndef STEUERWERK_H
#define STEUERWERK_H

#endif // STEUERWERK_H
