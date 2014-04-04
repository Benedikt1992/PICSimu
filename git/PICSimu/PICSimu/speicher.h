#ifndef SPEICHER_H
#define SPEICHER_H

class Speicher
{
public:
    int bank0[47];
    int bank1[47];

    int* refBank[2][47];

    Speicher(void);
    ~Speicher(void);

    int* getFile(int);
    void ausgeben(int);
};

#endif // SPEICHER_H
