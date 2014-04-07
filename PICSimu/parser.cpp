#include <iostream>
#include <math.h>
#include "parser.h"

using namespace std;

Parser::Parser()
{
}

bool Parser::auslesen(list<string>* lstDatei, string filename, Steuerwerk* pic)
{
    string line;
    ifstream lesestream(filename.c_str());

    if(!lesestream)
        return false;

    while(!lesestream.eof())
    {

        getline(lesestream, line, '\n');

        lstDatei->push_back(line);

        if(line[0] != ' ' && line[0] != 0)	// Codezeile erkennen!
        {

            int address = 0;
            int command = 0;

            /*
             *
             * stringstream
             * korrekt Hexzahlen einlesen und konvertieren!
             *
             */

            for(int i=0; i<4; i++)		// Wert einlesen und in int speichern
                address += (line[i]-48)*pow(16,(3-i));

            for(int i=0; i<4; i++)
                command += (line[i+5]-48)*pow(16,(3-i));

            Codeline* newLine = new Codeline(address, command);

            pic->sourcecode.push_back(*newLine);
        }

        pic->pc = pic->sourcecode.begin();
    }

    lesestream.close();

    return true;
}


// für Testzwecke auf Console
void Parser::ausgeben(list<string> lstDatei)
{
    for(list<string>::iterator i=lstDatei.begin(); i != lstDatei.end(); i++)
        cout << *i << endl;
}

