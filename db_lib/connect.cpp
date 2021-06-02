#include <bits/stdc++.h>
#include "UserInput.h"
#include "connect.h"
#include <stdio.h>
#include <sys/types.h>


using namespace std;


void open_db(int argc,char** argv){
    if (argc < 2) {
    printf("Supply a database filename as an argument.\n");
    exit(EXIT_FAILURE);
  }
}


void database::connect_db_console(int argc, char** argv,database* db) {
  
    open_db(argc,argv);
    UserInput *user_buffer = new UserInput;
    Table* table = new Table;
    char* filename = argv[1];
    Table* t = table->db_open(filename);
    db->table = t;
    while(1){
        cout<<"db > ";
        user_buffer->read_input(user_buffer);
        user_buffer->check_user_input(user_buffer,db->table);
    }
    return;
}

void database::connect_db(int argc, char** argv,database* db){

    open_db(argc,argv);
    Table* table = new Table;
    char* filename = argv[1];
    Table* t = table->db_open(filename);
    db->table = t;
}

void database::insert(int id, string name, string email,database* db){
    UserInput *user_buffer = new UserInput;
    string buffer = "insert " ;
    buffer+=to_string(id);
    buffer+=" ";
    buffer+=name;
    buffer+=" ";
    buffer+=email;
    // cout<<buffer<<endl;
    user_buffer->fill_buffer(user_buffer,buffer);
    user_buffer->check_user_input(user_buffer,db->table);
}

void database::print_db(database* db){
    UserInput *user_buffer = new UserInput;
    string buffer ="select";
    user_buffer->fill_buffer(user_buffer,buffer);
    user_buffer->check_user_input(user_buffer,db->table);
}

void database::close_db(database* db){
    UserInput *user_buffer = new UserInput;
    string buffer =".exit";
    user_buffer->fill_buffer(user_buffer,buffer);
    user_buffer->check_user_input(user_buffer,db->table);
}

void database::print_tree(database* db){
    UserInput *user_buffer = new UserInput;
    string buffer =".btree";
    user_buffer->fill_buffer(user_buffer,buffer);
    user_buffer->check_user_input(user_buffer,db->table);
}