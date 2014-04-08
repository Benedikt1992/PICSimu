#ifndef STEUERWERK_H
#define STEUERWERK_H

#include "prozessor.h"
#include "codeline.h"
#include <vector>
#include "mainwindow.h"

using namespace std;

class Steuerwerk
{
public:
    MainWindow* mainWindow;
    Prozessor* alu;
	//Benedikt: geändert
	list<QString> lstFile;
    vector<Codeline> maschinencode;
    vector<Codeline>::iterator pc;

    Steuerwerk(MainWindow*);
    ~Steuerwerk(void);


    bool loadFile(string);
    bool executeStep(void);
    void run(void);
    void execute(int);
	bool toggleBreakpoint(int);
    bool programmEndeErreicht(void);

    int* getBank(int);
};



#endif // STEUERWERK_H
#ifndef STEUERWERK_H
#define STEUERWERK_H

#endif // STEUERWERK_H
