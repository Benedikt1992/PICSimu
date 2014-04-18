#include "goklasse.h"
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
        if(sW->programmEndeErreicht()) // return falls noch kein Programm existiert
        {

            emit slotGoClicked();
            return ;
        }

        //aktuelle Zeile in urpsungszustand färben
        if(sW->pc->breakpoint)
            emit setLineColorRed(sW->getCurrentLineNumber()-1);
        else
            emit setLineColorWhite(sW->getCurrentLineNumber()-1);

        //führe aktuellen Befehl aus und erhöhe programmcounter
        if(sW->pc != sW->maschinencode.end())
        {
            sW->execute(sW->pc->command);
            sW->pc++;
            //neuen PC Wert in das PCL Register schreiben
            sW->alu->speicher.writePC(sW->pc - sW->maschinencode.begin());
        }
        else
        {
            emit slotGoClicked();
            return ;
        }

        //anzeige erneuern
        emit slotRefreshSpeicher();
        emit refreshSFRWidget();

        //Go beenden wenn programmende erreicht ist
        if(sW->programmEndeErreicht())
        {
            emit slotGoClicked();
            return ;
        }

        //nächste Zeile einfärben und anzeigen
        emit setLineColorGreen(sW->getCurrentLineNumber()-1);
        emit gotoLineNumber(sW->getCurrentLineNumber()-1);


        //Stop falls breakpoint erreicht
        if(sW->pc->breakpoint)
            emit slotGoClicked();

        //Pause
        QThread::msleep(100);
    }
}
