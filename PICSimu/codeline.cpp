#include "codeline.h"


Codeline::Codeline(void)
{
    textzeile = -1;
    command = -1;
	breakpoint =false;
}

Codeline::Codeline(int p_textzeile, int p_command)
{
    textzeile = p_textzeile;
    command = p_command;
	breakpoint = false;
}

Codeline::~Codeline(void)
{
}

