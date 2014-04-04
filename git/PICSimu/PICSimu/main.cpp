#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <math.h>
#include "codeline.h"
#include "steuerwerk.h"
#include "parser.h"

using namespace std;

int main(int argc, char* argv[])
{
    Steuerwerk* pic = new Steuerwerk();
    string filename;

    if(argc == 2)
        filename = argv[1];
    else
        return -1;

    cout << "Filename: " << filename << endl;

    list<Codeline> sourcecode;
    list<string> lstDatei;
    string line;

    Parser::auslesen(sourcecode, lstDatei, line, filename, pic);
    Parser::ausgeben(lstDatei);

    getchar();

    pic->run();


    return 0;
}
