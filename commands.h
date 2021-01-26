#ifndef COMMANDS
#define COMMANDS

#include <iostream>
#include "UserInput.h"
using namespace std;


// Command = Statement;

typedef enum{
    META_COMMAND_SUCCESS,
    META_COMMAND_INVALID
} MetaCommandResults;

typedef enum{
    COMMAND_SUCCESS,
    COMMAND_UNRECOGNIZED
} PrepareCommand;

typedef enum{
    INSERT_COMMAND,
    SELECT_COMMAND
} CommandType;

typedef struct {
    CommandType type;
}Command;

MetaCommandResults check_meta_command(string &s);

PrepareCommand prepare_db_command(string &s,Command* c);      // prepare result

void execute_command(Command* c);

#endif