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

    static void ausgeben(list<string> lstDatei);
    static void auslesen(list<Codeline> code, list<string> lstDatei, string line, string filename, Steuerwerk* pic);

};

#endif // PARSER_H
