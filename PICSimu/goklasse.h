#ifndef GOTHREAD_H
#define GOTHREAD_H
#include <QThread>
#include <QObject>
#include "steuerwerk.h"

class GoKlasse : public QObject
{
    Q_OBJECT
public:
    GoKlasse(Steuerwerk* sW);
public slots:
    void run (void);
private:
    Steuerwerk* sW;
signals:
    void setLineColorWhite(int);
    void setLineColorGreen(int);
    void setLineColorRed(int);
    void gotoLineNumber(int);
    void refreshSFRWidget(void);
    void slotGoClicked(void);
    void slotRefreshSpeicher(void);
    void getVerzoegerung(int*);
    void refreshRuntime(void);

};

#endif // GOTHREAD_H
