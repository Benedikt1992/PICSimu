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
		if(!sW->executeStep())
		{
			emit slotGoClicked();
			return;
		}


        //Stop falls breakpoint erreicht
        if(sW->pc->breakpoint)
        {
            emit slotGoClicked();
            return;
        }

        //Pause
        int v=50; // falls ds Signal nicht schnell genug verarbeitet wird -> mindestverzögerung 50ms
        emit getVerzoegerung(&v);
        QThread::msleep(50); // warte darauf, dass signal abgearbeitet wurde
        //std::cout << v << std::endl;
        QThread::msleep(v-50);
    }

}
