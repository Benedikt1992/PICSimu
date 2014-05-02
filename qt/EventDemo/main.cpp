#include <QCoreApplication>
#include "myobject.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    MyObject mo;

    return a.exec();
}
