#ifndef SLEEPKLASSE_H
#define SLEEPKLASSE_H

#include <QObject>
#include "steuerwerk.h"

class SleepKlasse : public QObject
{
    Q_OBJECT
public:
    explicit SleepKlasse(QObject *parent = 0);
    SleepKlasse(Steuerwerk*);
private:
    Steuerwerk* sW;

signals:

public slots:
    void run (void);

};

#endif // SLEEPKLASSE_H
