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
    cout << "vor while" << endl;
    while(!fin.eof())
    {
        cout << "while anfang" << endl;
        getline(fin, line, '\n');
        cout << line << endl;
        cout << "nach get line" << endl;
        lstDatei->push_back(line);
        cout << "nach push back" << endl;
        if(line[0] != ' ' && line[0] != 0)	// Codezeile erkennen!
        {
            cout << "if-Bedingung erfüllt" << endl;
            int address = 0;
            int command= 0;

            for(int i=0; i<4; i++)		// Wert einlesen und in int speichern
                address += (line[i]-48)*pow(16,(3-i));

            for(int i=0; i<4; i++)
                command += (line[i+5]-48)*pow(16,(3-i));

            Codeline* newLine = new Codeline(address, command);

            pic->sourcecode.push_back(*newLine);
        }
        cout << "nach if" << endl;

        pic->pc = pic->sourcecode.begin();
        cout << "nach begin()" << endl;
    }
    cout << "nach while" << endl;
    fin.close();
    cout << "filestream closed" << endl;

}

void Parser::ausgeben(list<string> lstDatei)
{
    for(list<string>::iterator i=lstDatei.begin(); i != lstDatei.end(); i++)
        cout << *i << endl;
}

