#include "sleepklasse.h"
#include "prozessor.h"
#include <QThread>
#include <iostream>
using namespace std;

SleepKlasse::SleepKlasse(QObject *parent) :
    QObject(parent)
{
}
SleepKlasse::SleepKlasse(Steuerwerk * sW)
{
    this->sW = sW;
}

void SleepKlasse::run()
{
    sW->iAmSleeping = true;
    sW->wdt = 0;
    int currentStatus=sW->alu->speicher.readOnBank(0,3); //Lese Status register
    currentStatus = (currentStatus | 0x0010) & 0xfff7;  //clear PD and set TO bit in Status register
    sW->alu->speicher.writeOnBank(0,3,currentStatus);
cout << sW->wdt << endl;
    while(sW->isSleeping())
    {
        sW->alu->vorherigeCycles = sW->alu->cycles;

        sW->alu->nop();
        sW->countWDT();
        cout << sW->wdt << endl;
        emit sW->refreshRuntime();

        if(sW->isWDTTimeOut())
        {
            sW->iAmSleeping=false;
            currentStatus=sW->alu->speicher.readOnBank(0,3); //Lese Status register
            currentStatus = currentStatus & 0xffef;  //clear TO bit in Status register
            sW->alu->speicher.writeOnBank(0,3,currentStatus);
            emit sW->slotRefreshSpeicher();
            emit sW->refreshRuntime();
            emit sW->refreshSFRWidget();
            return;
        }
        QThread::msleep(50); //verzögerung um GUI nicht zu überlasten

    }



}
