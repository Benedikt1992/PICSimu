#ifndef SPEICHER_H
#define SPEICHER_H

#define n_register 48

class Speicher
{
public:
    int bank0[n_register];
    int bank1[n_register];

    int workingregister;

    int* refBank[2][n_register];

    Speicher(void);
    ~Speicher(void);

    int* getFileReference(int);
    void ausgeben(int);
	bool clearSpeicher(void);
};

#endif // SPEICHER_H
