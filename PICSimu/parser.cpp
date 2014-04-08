#include <iostream>
#include <math.h>
#include <sstream>
#include "parser.h"

using namespace std;

Parser::Parser()
{
}

bool Parser::auslesen(list<QString>* lstDatei, string filename, Steuerwerk* steuerwerk)
// lstDatei nicht notwendig... Zugriff über steuerwerk
{
    // cout << "auslesen() gestartet" << endl;

    string line;
    ifstream lesestream(filename.c_str());

    if(!lesestream)
        return false;

    while(!lesestream.eof())
    {
        // cout << "while-Schleife Anfang" << endl;
        getline(lesestream, line, '\n');
        //cout << line << endl;
        // cout << "Zeile an lst-Liste anfügen" << endl;


		//Benedikt: gändert
		lstDatei->push_back((QString::fromStdString(line)));

        // cout << "push_back() erfolgreich" << endl;

        if(line[0] != ' ' && line[0] != 0)	// Codezeile erkennen!
        {
            // cout << "if-Bedingung erfüllt" << endl;
            int textZeile = 0;
            int command = 0;

            /*
             *
             * stringstream
             * korrekt Hexzahlen einlesen und konvertieren!
             *
             */

            stringstream stream;
            stream << std::hex << line.substr(5, 4);
            stream >> command;
            stream.clear();

            cout << command << " - ";

            stream << std::dec << line.substr(21, 4);
            stream >> textZeile;
            stream.clear();

            cout << textZeile << endl;

            Codeline* newLine = new Codeline(textZeile, command);

            // cout << "Zeile zu Maschinencode hinzufügen" << endl;
            steuerwerk->maschinencode.push_back(*newLine);
        }

        // cout << "Programmcounter gesetzt" << endl;
        steuerwerk->pc = steuerwerk->maschinencode.begin();
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

