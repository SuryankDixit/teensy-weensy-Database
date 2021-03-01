#include <bits/stdc++.h>
#include <unistd.h>
#include "table.h"
#include "commands.h"

using namespace std;


MetaCommandResults Command:: check_meta_command(string &input,Table* table){
    if(input == ".exit"){
        db_close(table);
        return META_COMMAND_SUCCESS;
    }else{
        return META_COMMAND_INVALID;
    }
}

MetaCommandResults meta_command(string &s,Command* c,Table* table){
    return c->check_meta_command(s,table);
}

PrepareCommand Command:: prepare_insert_command(string &s,Command* c){
    c->type = INSERT_COMMAND;

    vector <string> tokens;
      
    // stringstream class check1 
    stringstream check1(s); 
      
    string intermediate; 
      
    // Tokenizing w.r.t. space ' ' 
    while(getline(check1, intermediate, ' ')) 
    { 
        tokens.push_back(intermediate); 
    }
    int n = tokens.size();
    if(n!=4){
        return COMMAND_SYNTAX_ERROR;
    }
    string id_string = tokens[n-3];
    string username = tokens[n-2];
    string email = tokens[n-1];

    int id=0;
    try{
        id = stoi(id_string);
    }catch(exception &err){
        cout << "Invalid query format "<<endl;
        cout<<"Table Structure is : int>0     string(32)      string(255)\n";
        cout<<"Exiting..."<<endl;
        usleep(10000);
        exit(0);
    }

    if(username.size()> NAME_SIZE)
        return COMMAND_TOO_LONG;
    if(email.size()> MAIL_SIZE) 
        return COMMAND_TOO_LONG;

    c->row.id = (uint32_t)id;
    // c_str() function is converting string to char*;
    // contents pointed by char* are inserted into row;
    strcpy(c->row.username,username.c_str());
    strcpy(c->row.email,email.c_str());

    // cout<<c->row.id<<" "<<c->row.username<<" "<<c->row.email<<endl;

    return COMMAND_SUCCESS;
}

PrepareCommand Command:: prepare_select_command(string &s,Command* c){
    c->type = SELECT_COMMAND;

    return COMMAND_SUCCESS;
}


PrepareCommand prepare_db_command(string &input, Command* command){

    if(input.substr(0,6)=="insert"){
        return command->prepare_insert_command(input,command);
    }
    if(input.substr(0,6)=="select"){
        return command->prepare_select_command(input,command);
    }

    return COMMAND_UNRECOGNIZED;
}

CommandType Command :: check_command_type(Command* c){
    return c->type;
}


ExecuteCommand Command :: execute_insert_command(Command* c,Table* t){
    if(t->get_num_rows() >= TABLE_MAX_ROWS){
        return EXECUTE_TABLE_FULL;
    }
    Row *row = &(c->row);
    // Serialization is the process of turning an object in memory into a stream of bytes so you can do stuff like store it on disk or send it over the network.
    c->row.serialize_data(row, t->allocate_row(t , t->get_num_rows()));
    t->set_num_rows();

    return EXECUTE_SUCCESS;
}

ExecuteCommand Command :: execute_select_command(Command* c,Table* t){
    
    Row row;
    for (uint32_t i = 0; i < t->get_num_rows(); i++) {
        c->row.deserialize_data(t->allocate_row(t,i),&row);
        c->row.print_row(&row);
    }
    return EXECUTE_SUCCESS;
}


ExecuteCommand execute_command(Command* command, Table* table){
    switch(command->check_command_type(command)){
        case(INSERT_COMMAND):
            // cout<<"In insert1\n";
            return command->execute_insert_command(command,table);
            break;
        case(SELECT_COMMAND):
            return command->execute_select_command(command,table);
            break;
    }
}


void Command::test(){
    Command c;
}