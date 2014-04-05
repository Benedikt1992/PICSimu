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
    Steuerwerk* pic = new Steuerwerk();
    string filename;

    if(argc == 2)
        filename = argv[1];
    else
        return -1;

    cout << "Filename: " << filename << endl;

    list<string> lstDatei;

    Parser::auslesen(&lstDatei, filename, pic);
    Parser::ausgeben(lstDatei);

    getchar();

    pic->run();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();





    return a.exec();
}
