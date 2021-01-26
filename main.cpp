#include <bits/stdc++.h>
#include "UserInput.h"

using namespace std;

int main(int argc, char* argv[]) {
  UserInput *user_buffer = new UserInput;
  while(1){
    cout<<"db > ";
    user_buffer->read_input(user_buffer);

    user_buffer->check_user_input(user_buffer);
  }
  return 0;
}
