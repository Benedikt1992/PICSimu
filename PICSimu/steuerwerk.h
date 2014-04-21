#ifndef STEUERWERK_H
#define STEUERWERK_H


#include "codeline.h"
#include <vector>
#include <stack>
#include "mainwindow.h"
#include "steuerwerk.h"
#include "prozessor.h"

using namespace std;

class Steuerwerk : public QObject
{
    Q_OBJECT
public:

    Prozessor* alu;
	//Benedikt: geändert
	list<QString> lstFile;
    vector<Codeline> maschinencode;
    vector<Codeline>::iterator pc;

    std::stack<vector<Codeline>::iterator> stack; // genius...
    bool isRunning;

    Steuerwerk(MainWindow*);
    ~Steuerwerk(void);


    bool loadFile(string);
    bool executeStep(void);
    void run(void);
    void execute(int);
	bool toggleBreakpoint(int);
    bool programmEndeErreicht(void);
	bool clearSteuerwerk(void);
	bool clearProgrammspeicher(void);
    bool clearRuntime(void);
    int getCurrentLineNumber(void);
    int readForGUI(int,int);
    int readWForGUI(void);
    int getPCInt(void);
    void setTimePerCycle(double);
    double computeRuntime(void);

    void testForInterrupt(void);
    void callInterrupt(void);

signals:
    void connectSteuerwerk(Steuerwerk*);
    void setLineColorRed (int);
    void setLineColorWhite (int);
    void setLineColorGreen(int);
    void slotRefreshSpeicher();
    void refreshSFRWidget();
    void refreshRuntime();
    void gotoLineNumber(int);
};



#endif // STEUERWERK_H
#ifndef STEUERWERK_H
#define STEUERWERK_H

#endif // STEUERWERK_H
