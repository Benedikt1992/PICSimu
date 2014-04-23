#include "goklasse.h"
#include <QThread>
#include "steuerwerk.h"
#include <iostream>

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
        sW->testForInterrupt();

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
        emit refreshRuntime();
        emit refreshStack();

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
        int v=50; // falls ds Signal nicht schnell genug verarbeitet wird -> mindestverzögerung 50ms
        emit getVerzoegerung(&v);
        QThread::msleep(50); // warte darauf, dass signal abgearbeitet wurde
        //std::cout << v << std::endl;
        QThread::msleep(v-50);
    }
}
