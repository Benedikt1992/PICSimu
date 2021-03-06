#ifndef PROZESSOR_H
#define PROZESSOR_H

#include "speicher.h"
#define WRITE_TO_FILE_REGISTER 1
#define WRITE_TO_WORKING_REGISTER 0

class Steuerwerk;

class Prozessor
{
public:
	Speicher speicher;
    int cycles;
    int vorherigeCycles;
    double timePerCycle; //in mikro sekunden
    void setTimePerCycle(double);
    double computeRuntime(void);

	Prozessor(Steuerwerk* steuerwerk);
    ~Prozessor(void);

    void execute(int);
	bool clearProzessor(void);
    bool clearRuntime(void);

    void addwf(int);
    void andwf(int);
    void clrf(int);
    void clrw();
    void comf(int);
	void decf(int);
    void decfsz(int command, Steuerwerk* steuerwerk);
    void incf(int);
    void incfsz(int command, Steuerwerk* steuerwerk);
    void iorwf(int);
    void movf(int);
    void movwf(int);
    void nop (void);
    void rlf(int);
    void rrf(int);
    void subwf(int);
    void swapf(int);
	void xorwf(int);

    void bcf(int command);
    void bsf(int);
    void btfsc(int command, Steuerwerk* steuerwerk);
    void btfss(int command, Steuerwerk* steuerwerk);

    void addlw(int command);
    void andlw(int command);
	void call(int command, Steuerwerk*);
    void clrwdt(Steuerwerk* steuerwerk);
    void go_to(int, Steuerwerk*);
    void iorlw(int command);
    void movlw(int command);
    void retfie(Steuerwerk* steuerwerk);
	void retlw(int command, Steuerwerk* steuerwerk);
	void preturn(Steuerwerk* steuerwerk);
    void psleep(Steuerwerk* steuerwerk);
    void sublw(int command);
    void xorlw(int command);

    void checkCarryFlag(int result);
    void checkCarryFlagRRF(int result);
    void checkDigitCarryFlagAddition(int x, int y);
    void checkDigitCarryFlagSubtraktion(int x, int y);
    void checkZeroFlag(int result);

    void writeBack(int file, int result, bool storeInFileRegister);
    void writeBackToW(int result);

    int getCycles(void);
};

#endif // PROZESSOR_H
