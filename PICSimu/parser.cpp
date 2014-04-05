#include <iostream>
#include <math.h>
#include "parser.h"

using namespace std;

Parser::Parser()
{
}

void Parser::auslesen(list<string>* lstDatei, string filename, Steuerwerk* pic)
{
    string line;
    ifstream fin(filename.c_str());

    while(!fin.eof())
    {
        getline(fin, line, '\n');

        lstDatei->push_back(line);

        if(line[0] != ' ' && line[0] != 0)	// Codezeile erkennen!
        {
            int address = 0;
            int command= 0;

            for(int i=0; i<4; i++)		// Wert einlesen und in int speichern
                address += (line[i]-48)*pow(16,(3-i));

            for(int i=0; i<4; i++)
                command += (line[i+5]-48)*pow(16,(3-i));

            Codeline* newLine = new Codeline(address, command);

            pic->sourcecode.push_back(*newLine);
        }

        pic->pc = pic->sourcecode.begin();
    }

    fin.close();
}

void Parser::ausgeben(list<string> lstDatei)
{
    for(list<string>::iterator i=lstDatei.begin(); i != lstDatei.end(); i++)
        cout << *i << endl;
}

