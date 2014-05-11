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

    string line;
    ifstream lesestream(filename.c_str());

    if(!lesestream)
        return false;

    while(!lesestream.eof())
    {
        getline(lesestream, line, '\n');


		//Benedikt: gändert
		lstDatei->push_back((QString::fromStdString(line)));


        if(line[0] != ' ' && line[0] != 0)	// Codezeile erkennen!
        {
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

            stream << std::dec << line.substr(21, 4);
            stream >> textZeile;
            stream.clear();

            Codeline* newLine = new Codeline(textZeile, command);

            steuerwerk->maschinencode.push_back(*newLine);
        }


    }

    lesestream.close();

    return true;
}




