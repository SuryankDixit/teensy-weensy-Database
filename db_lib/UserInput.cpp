#include <bits/stdc++.h>
#include "UserInput.h"
#include "commands.h"
#include "../css.h"
#include <stdio.h>
#include <sys/types.h>

using namespace std;


void free_object(UserInput *obj){
  delete(obj);
}

void UserInput:: read_input(UserInput *obj){

  getline(cin,obj->buffer);
  // if(obj->buffer.size()==0){
  //   // getline(cin,obj->buffer);
  //   return;
  // }
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
  if(input[0] == '.' || input.size()==0){
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
           cout<<ANSI_COLOR_RED<<"Unrecognized Command Entered: "<<input<<ANSI_COLOR_RESET<<endl;
        break;
      case(COMMAND_SYNTAX_ERROR):
        cout<<ANSI_COLOR_RED<<"Syntax Error"<<ANSI_COLOR_RESET<<endl;
        break;
      case(COMMAND_TOO_LONG):
        cout<<ANSI_COLOR_RED<<"Command too long/short"<<input<<ANSI_COLOR_RESET<<endl;
        break;
      default: break;
    }

    // execute_command(&command);
    if(command_parsed_successfully){
      switch(execute_command(&command,table)){
        case (EXECUTE_SUCCESS):
          cout<<ANSI_COLOR_GREEN<<"Executed\n"<<ANSI_COLOR_RESET;
          break;
        case (EXECUTE_TABLE_FULL):
          cout<<ANSI_COLOR_RED<<"Table Full Error\n"<<ANSI_COLOR_RESET;
          break;
        case (EXECUTE_DUPLICATE_KEY):
          cout<<ANSI_COLOR_RED<<"Duplicate Key Insertion. Record couldn't be inserted\n"<<ANSI_COLOR_RESET;
          break;
        case (EXECUTE_KEY_MATCH):
          cout<<ANSI_COLOR_GREEN<<"Record Found.\n"<<ANSI_COLOR_RESET;
          break;
        case (EXECUTE_KEY_MISMATCH):
          cout<<ANSI_COLOR_RED<<"Record Doesn't Exist.\n"<<ANSI_COLOR_RESET;
          break;
      }
    }
  }
  return ;
}