#ifndef COMMANDS
#define COMMANDS

#include <bits/stdc++.h>
#include "UserInput.h"
#include "table.h"
using namespace std;

// Command = Statement;

typedef enum{
    META_COMMAND_SUCCESS,
    META_COMMAND_INVALID
} MetaCommandResults;

typedef enum{
    COMMAND_SUCCESS,
    COMMAND_UNRECOGNIZED,
    COMMAND_TOO_LONG,
    COMMAND_SYNTAX_ERROR,
} PrepareCommand;

typedef enum{
    EXECUTE_SUCCESS,
    EXECUTE_TABLE_FULL,
    EXECUTE_INVALID_COMMAND,
    EXECUTE_KEY_MATCH,
    EXECUTE_KEY_MISMATCH,
    EXECUTE_DUPLICATE_KEY
} ExecuteCommand;

typedef enum{
    INSERT_COMMAND,
    SELECT_COMMAND,
    SEARCH_COMMAND
} CommandType;

class Command {
    CommandType type;
    Row row;

public:
    
    MetaCommandResults check_meta_command(string &s,Table*);
    PrepareCommand prepare_insert_command(string &s,Command* c);
    PrepareCommand prepare_select_command(string &s,Command* c);
    PrepareCommand prepare_search_command(string &s,Command* c);
    ExecuteCommand execute_insert_command(Command* c,Table* t);
    ExecuteCommand execute_select_command(Command* c,Table* t);
    ExecuteCommand execute_search_command(Command* c,Table* t);
    CommandType check_command_type(Command* c);
    void test();
};

MetaCommandResults meta_command(string &s,Command* c,Table*);

PrepareCommand prepare_db_command(string &s,Command* c);      // prepare result

ExecuteCommand execute_command(Command* c,Table* t);

#endif