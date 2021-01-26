#ifndef USER_INPUT
#define USER_INPUT

#include <bits/stdc++.h>
using namespace std;

class UserInput{
  string buffer;
  ssize_t buffer_len;

public:

  UserInput(){
    this->buffer_len = 0;
  }

  ~UserInput(){}

  void read_input(UserInput*);
  void check_user_input(UserInput*);
};

void free_object(UserInput*);

#endif