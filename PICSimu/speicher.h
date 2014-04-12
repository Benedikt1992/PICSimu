#ifndef SPEICHER_H
#define SPEICHER_H

#define n_register 48

class Speicher
{
public:



    Speicher(void);
    ~Speicher(void);

    int* getFileReference(int);
    void ausgeben(int);
	bool clearSpeicher(void);
    bool write(int,int);
    int read(int);
    int readForGUI(int,int);
    int readW(void);
    void writeW(int value);
    void setZBit(void);
    void clearZBit(void);
    void setDCBit(void);
    void clearDCBit(void);
    void setCBit(void);
    void clearCBit(void);


private:
    int* refBank[2][n_register];
    int workingregister;
    int bank0[n_register];
    int bank1[n_register];
};

#endif // SPEICHER_H
