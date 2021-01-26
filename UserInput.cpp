#include <bits/stdc++.h>
#include "UserInput.h"

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
  if(obj->buffer == ".exit"){
    free_object(obj);
    cout<<"Exiting..."<<endl;
    exit(EXIT_SUCCESS);
  }else if(buffer_len>0){
    cout<<"Invalid Command "<<obj->buffer<<endl;
  }
  return ;
}
