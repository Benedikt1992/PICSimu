#ifndef EEPROMKLASSE_H
#define EEPROMKLASSE_H

#include <QObject>
#include "speicher.h"
#define EEPROM_SIZE (0x003f)
class EEPROMKlasse : public QObject
{
    Q_OBJECT
public:
    explicit EEPROMKlasse(QObject *parent = 0);
    EEPROMKlasse(int* eecon1, int* eedata, int* eeadr);

    int eecon2;
    int eeprom[EEPROM_SIZE+1];
private:
    int* eecon1;
    int* eedata;
    int* eeadr;

signals:

public slots:
    void eepromRead();
    void eepromWrite();

};

#endif // EEPROMKLASSE_H
