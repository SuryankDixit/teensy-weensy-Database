#include <bits/stdc++.h>
#include "UserInput.h"
#include "commands.h"
#include <stdio.h>
#include <sys/types.h>

using namespace std;


void free_object(UserInput *obj){
  delete(obj);
}

void UserInput:: read_input(UserInput *obj){
  getline(cin,obj->buffer);
  
  ssize_t len = obj->buffer.size();
  obj->buffer_len = len;
  return ;
}

void UserInput:: fill_buffer(UserInput *obj,string &command){
  obj->buffer = command;
  // cout<<obj->buffer<<endl;
  ssize_t len = obj->buffer.size();
  obj->buffer_len = len;
  return;
}

void UserInput:: check_user_input(UserInput* obj,Table* table){
  string input = obj->buffer;
  int command_parsed_successfully=0;
  Command command;
  // condition for meta commands that start with a '.' symbol;
  if(input == ".exit"){
        db_close(table);
        free_object(obj);
        exit(EXIT_SUCCESS);
  }
  if(input[0] == '.'){
    switch(meta_command(input,&command,table)){
      case(META_COMMAND_SUCCESS):
        break;
      case(META_COMMAND_INVALID):
        cout<<"Unrecognized Command Entered: "<<input<<endl;
    }
  }
  // Parsing of Commands:
  else{
    switch(prepare_db_command(input,&command)){
      case(COMMAND_SUCCESS):
        command_parsed_successfully=1;
        break;
      case(COMMAND_UNRECOGNIZED):
           cout<<"Unrecognized Command Entered: "<<input<<endl;
        break;
      case(COMMAND_SYNTAX_ERROR):
        cout<<"Syntax Error"<<endl;
        break;
      case(COMMAND_TOO_LONG):
        cout<<"Command too long/short"<<input<<endl;
        break;
      default: break;
    }

    // execute_command(&command);
    if(command_parsed_successfully){
      switch(execute_command(&command,table)){
        case (EXECUTE_SUCCESS):
          cout<<"Executed\n";
          break;
        case (EXECUTE_TABLE_FULL):
          cout<<"Table Full Error\n";
          break;
        case (EXECUTE_DUPLICATE_KEY):
          cout<<"Duplicate Key Insertion\n";
          break;
      }
    }
  }
  return ;
}
