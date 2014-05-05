#ifndef SPEICHER_H
#define SPEICHER_H

#define n_register 48
#define EEPROM_SIZE (0x003f)

class Speicher
{
public:
    int address_2007h;


    Speicher(void);
    ~Speicher(void);

    int* getFileReference(int);
    void ausgeben(int);
	bool clearSpeicher(void);
    bool write(int,int);
    int read(int);
    int readOnBank(int bank, int file);
    void writeOnBank(int bank,int file,int wert);
    int readW(void);
    void writeW(int value);
    void setZBit(void);
    void clearZBit(void);
    void setDCBit(void);
    void clearDCBit(void);
    void setCBit(void);
    void clearCBit(void);
	void writePC(int value);

    bool terminateEEPROM;

private:
    int* refBank[2][n_register];
    int workingregister;
    int bank0[n_register];
    int bank1[n_register];
	int latchA;
	int latchB;

    int eecon2;
    int* eecon1;
    int* eedata;
    int* eeadr;
    int eeprom[EEPROM_SIZE+1];
    void eepromRead();
    void eepromWrite();


};

#endif // SPEICHER_H
