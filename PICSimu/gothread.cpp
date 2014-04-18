#include "gothread.h"
#include <QThread>
#include "steuerwerk.h"

GoKlasse::GoKlasse(Steuerwerk* sW)
{
    this->sW = sW;
}

void GoKlasse::run()
{
    while(sW->isRunning)
    {
        if(sW->programmEndeErreicht())
        {

            emit slotGoClicked();
            return ;
        }
        if(sW->pc->breakpoint)
            emit setLineColorRed(sW->getCurrentLineNumber()-1);
        else
            emit setLineColorWhite(sW->getCurrentLineNumber()-1);

        if(sW->pc != sW->maschinencode.end())
        {
            sW->execute(sW->pc->command);
            sW->pc++;
            sW->alu->speicher.writePC(sW->pc - sW->maschinencode.begin());
        }
        else
        {
            emit slotGoClicked();
            return ;
        }
        emit slotRefreshSpeicher();
        emit refreshSFRWidget();

        if(sW->programmEndeErreicht())
        {
            emit slotGoClicked();
            return ;
        }

        emit setLineColorGreen(sW->getCurrentLineNumber()-1);
        emit gotoLineNumber(sW->getCurrentLineNumber()-1);

        if(sW->pc->breakpoint)
            emit slotGoClicked();
        QThread::msleep(100);
    }
}
