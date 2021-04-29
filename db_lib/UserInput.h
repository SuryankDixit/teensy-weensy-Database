#ifndef USER_INPUT
#define USER_INPUT

#include <bits/stdc++.h>
#include "table.h"
using namespace std;

class UserInput{
  string buffer;
  ssize_t buffer_len;

public:

  UserInput(){
    this->buffer_len = 0;
  }

  ~UserInput(){}

  void fill_buffer(UserInput*, string&);
  void read_input(UserInput*);
  void check_user_input(UserInput*,Table*);
};

void free_object(UserInput*);

#endif