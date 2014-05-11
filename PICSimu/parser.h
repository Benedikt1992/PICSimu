#ifndef PARSER_H
#define PARSER_H

#include <list>
#include <string>
#include <fstream>
#include "codeline.h"
#include "steuerwerk.h"

using namespace std;

class Parser
{
public:
    Parser();


	static bool auslesen(list<QString>* lstDatei, string filename, Steuerwerk* pic);

};

#endif // PARSER_H
