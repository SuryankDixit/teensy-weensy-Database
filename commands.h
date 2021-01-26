#ifndef COMMANDS
#define COMMANDS

#include <iostream>
#include "UserInput.h"
using namespace std;


// Command = Statement;

typedef enum{
    META_COMMAND_SUCCESS,
    META_INVALID_COMMAND
} MetaCommandResults;

MetaCommandResults check_meta_command(string &s);

#endif