#include <bits/stdc++.h>
#include "commands.h"
#include "UserInput.h"

using namespace std;

MetaCommandResults check_meta_command(string &input){
    if(input == ".exit"){
        return META_COMMAND_SUCCESS;
    }else{
        return META_COMMAND_INVALID;
    }
}

PrepareCommand prepare_db_command(string &input, Command* command){
    if(input.substr(0,6)=="insert"){
        command->type = INSERT_COMMAND;
        return COMMAND_SUCCESS;
    }
    if(input.substr(0,6)=="select"){
        command->type = SELECT_COMMAND;
        return COMMAND_SUCCESS;
    }

    return COMMAND_UNRECOGNIZED;
}

void execute_command(Command* command){
    switch(command->type){
        case(INSERT_COMMAND):
            cout<<"Insert Command will be processed here."<<endl;
            break;
        case(SELECT_COMMAND):
            cout<<"Select Command will be processed here."<<endl;
            break;
    }
}