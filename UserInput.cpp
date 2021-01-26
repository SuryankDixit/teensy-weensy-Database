#include <bits/stdc++.h>
#include "UserInput.h"
#include "commands.h"

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

void UserInput:: check_user_input(UserInput* obj){
  string input = obj->buffer;
  int command_executed=0;

  // condition for meta commands that start with a '.' symbol;
  if(input[0] == '.'){
    switch(check_meta_command(input)){
      case(META_COMMAND_SUCCESS):
        free_object(obj);
        exit(EXIT_SUCCESS);
        break;
      case(META_COMMAND_INVALID):
        cout<<"Unrecognized Command Entered: "<<input<<endl;
    }
  }
  // condition for proper sql commands;
  else{
    Command command;
    switch(prepare_db_command(input,&command)){
      case(COMMAND_SUCCESS):
        command_executed=1;
        break;
      case(COMMAND_UNRECOGNIZED):
        cout<<"Unrecognized keyword at start of "<<input<<endl;
        break;
      default: break;
    }

    execute_command(&command);
    if(command_executed==1)
      cout<<"Executed\n";
  }
  return ;
}
