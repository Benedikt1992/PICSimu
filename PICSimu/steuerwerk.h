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

    std::stack<vector<Codeline>::iterator> picStack; // genius...
    bool isRunning;
    bool iAmSleeping;

    bool RA4alt;
    int externalClockCycles;
    int wdt;

    Steuerwerk(MainWindow*);
    ~Steuerwerk(void);

    bool isSleeping(void);
    void countWDT(void);
    bool isWDTTimeOut(void);
    bool loadFile(string);
    bool executeStep(void);
    void run(void);
    void analyzeAndExecute(int);
	bool toggleBreakpoint(int);
    bool programmEndeErreicht(void);
	bool clearSteuerwerk(void);
	bool clearProgrammspeicher(void);
    bool clearRuntime(void);
    int getCurrentLineNumber(void);
    int readForGUI(int bank,int file);
    int readWForGUI(void);
    void writeRAFromGUI(int value);
    void writeRBFromGUI(int value);
    int getPCInt(void);
    int getStackInt(void);
    void setTimePerCycle(double);
    double computeRuntime(void);

    void testForInterrupt(void);
    void callInterrupt(void);

    void checkTimer0(void);
    bool incrementTimerAndCheckOverflow(int timer);
    void setTimer0InterruptFlag(int intcon);

signals:
    void connectSteuerwerk(Steuerwerk*);
    void setLineColorRed (int);
    void setLineColorWhite (int);
    void setLineColorGreen(int);
    void slotRefreshSpeicher();
    void refreshSFRWidget();
    void refreshRuntime();
    void gotoLineNumber(int);
    void refreshStack(void);
    void reset(void);
};



#endif // STEUERWERK_H
#ifndef STEUERWERK_H
#define STEUERWERK_H

#endif // STEUERWERK_H
