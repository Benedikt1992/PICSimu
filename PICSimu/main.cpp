#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <math.h>
#include "codeline.h"
#include "steuerwerk.h"
#include "parser.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    Steuerwerk steuerwerk(&w);
    w.show();





    return a.exec();
}
