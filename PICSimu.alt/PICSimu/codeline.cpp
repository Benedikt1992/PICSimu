#include "codeline.h"


Codeline::Codeline(void)
{
    address = -1;
    command = -1;
}

Codeline::Codeline(int p_address, int p_command)
{
    address = p_address;
    command = p_command;
}

Codeline::~Codeline(void)
{
}
