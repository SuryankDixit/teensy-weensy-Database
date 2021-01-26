#include <bits/stdc++.h>
#include "commands.h"
#include "UserInput.h"

using namespace std;

MetaCommandResults check_meta_command(string &input){
    if(input == ".exit"){
        return META_COMMAND_SUCCESS;
    }else{
        return META_INVALID_COMMAND;
    }
}