#include <bits/stdc++.h>
#include<stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include "commands.h"
#include "table.h"

using namespace std;


MetaCommandResults Command:: check_meta_command(string &input,Table* table){
    
    if(input == ".btree"){
        cout<<"Tree:\n";
        print_tree(table->get_pager(),0,0);
        return META_COMMAND_SUCCESS;
    }else if(input.size()==0){
        return META_COMMAND_SUCCESS;
    }else if(input == ".constants"){
        cout<<"Constants:\n";
        print_constants();
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
        // cout<<intermediate<<" ";
        tokens.push_back(intermediate); 
    }
    // cout<<endl;

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
      /*  cout << "Invalid query format "<<endl;
        cout<<"Table Structure is : int>0     string(32)      string(255)\n";
        cout<<"Exiting..."<<endl;
        usleep(10000);
        exit(0);
        */
       return COMMAND_SYNTAX_ERROR;
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
    if(input.substr(0,6)=="search"){
        return command->prepare_search_command(input,command);
    }

    return COMMAND_UNRECOGNIZED;
}

CommandType Command :: check_command_type(Command* c){
    return c->type;
}


ExecuteCommand Command :: execute_insert_command(Command* c,Table* t){

    Row *row = &(c->row);
    Cursor* cursor = new Cursor();
    uint32_t new_key = row->id;
    cursor = table_find(t,new_key);

    void* node = get_page(t->get_pager(), cursor->page_num);
    uint32_t num_cells = *leaf_node_num_cells(node);    // number of cells in node

    // if (num_cells >= LEAF_NODE_MAX_CELLS) {
    //     return EXECUTE_TABLE_FULL;
    // }

    // cout<<cursor->cell_num<<"   "<<num_cells<<endl;
    if(cursor->cell_num < num_cells){
        uint32_t key_at_index = *leaf_node_key(node, cursor->cell_num);
        // cout<<key_at_index<<"   "<<new_key<<endl;
        if (key_at_index == new_key) {
        return EXECUTE_DUPLICATE_KEY;
     }
    }
    leaf_node_insert(cursor,row->id,row);
    free(cursor);

    return EXECUTE_SUCCESS;
}

ExecuteCommand Command :: execute_select_command(Command* c,Table* t){
    
    Row row;
    Cursor* cursor = new Cursor();
    cursor = table_start(t);

    while(!cursor->end_of_table) {
        c->row.deserialize_data(cursor->cursor_position(cursor),&row);
        c->row.print_row(&row);
        cursor->advance_cursor(cursor);
    }
    free(cursor);
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
        case(SEARCH_COMMAND):
            return command->execute_search_command(command,table);
            break;
    }
}


ExecuteCommand Command :: execute_search_command(Command* c,Table* t){

    Row *row = &(c->row);
    Cursor* cursor = new Cursor();
    uint32_t new_key = row->id;
    cursor = table_find(t,new_key);

    void* node = get_page(t->get_pager(), cursor->page_num);
    uint32_t num_cells = *leaf_node_num_cells(node);    // number of cells in node

    if(cursor->cell_num < num_cells){
        uint32_t key_at_index = *leaf_node_key(node, cursor->cell_num);
        if (key_at_index == new_key) {
            // return EXECUTE_DUPLICATE_KEY;
            Row row_;
            // Cursor* cursor = new Cursor();
            // cursor = table_start(t);

            // while(!cursor->end_of_table) {
            c->row.deserialize_data(cursor->cursor_position(cursor),&row_);
            c->row.print_row(&row_);
            return EXECUTE_KEY_MATCH;
            // cursor->advance_cursor(cursor);
            // }
        }
    }
    free(cursor);

    return EXECUTE_KEY_MISMATCH;
}

PrepareCommand Command:: prepare_search_command(string &s,Command* c){
    c->type = SEARCH_COMMAND;

    vector <string> tokens;
      
    // stringstream class check1 
    stringstream check1(s); 
      
    string intermediate; 
      
    // Tokenizing w.r.t. space ' ' 
    while(getline(check1, intermediate, ' ')) 
    { 
        // cout<<intermediate<<" ";
        tokens.push_back(intermediate); 
    }
    // for(auto x:tokens){
        // cout<<x<<" ";
    // }
    // cout<<endl;

    int n = tokens.size();
    if(n!=4){
        // cout<<"Hi1";
        return COMMAND_SYNTAX_ERROR;
    }
    string id_string = tokens[n-1];
    string equal = tokens[n-2];
    string id_ = tokens[n-3];

    int id=0;
    try{
        id = stoi(id_string);
    }catch(exception &err){
      /*  cout << "Invalid query format "<<endl;
        cout<<"Table Structure is : int>0     string(32)      string(255)\n";
        cout<<"Exiting..."<<endl;
        usleep(10000);
        exit(0);
        */
    //    cout<<"Hi2";
       return COMMAND_SYNTAX_ERROR;
    }

    if(equal != "equals"){
        cout<<"Hi3";
        return COMMAND_SYNTAX_ERROR;
    }
    if(id_ != "id"){
        // cout<<"Hi4";
        return COMMAND_SYNTAX_ERROR;
    }

    c->row.id = (uint32_t)id;

    return COMMAND_SUCCESS;
}


void Command::test(){
    Command c;
}