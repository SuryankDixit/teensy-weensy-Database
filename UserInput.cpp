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

  // condition for meta commands that start with a '.' symbol;
  if(input[0] == '.'){
    switch(check_meta_command(input)){
      case(META_COMMAND_SUCCESS):
        free_object(obj);
        exit(EXIT_SUCCESS);
        break;
      case(META_INVALID_COMMAND):
        cout<<"Unrecognized Command Entered: "<<input<<endl;
        break;
    }
  }

  // condition for proper sql commands;
  

  return ;
}
