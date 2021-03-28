#include <bits/stdc++.h>
#include "UserInput.h"
#include "table.h"
#include <stdio.h>
#include <sys/types.h>

// Hello World

using namespace std;

int main(int argc, char* argv[]) {
  Table* table = new Table;
  UserInput *user_buffer = new UserInput;
  if (argc < 2) {
    printf("Supply a database filename as an argument.\n");
    exit(EXIT_FAILURE);
  }

  char* filename = argv[1];
  Table* t = table->db_open(filename);
  while(1){
    cout<<"db > ";
    user_buffer->read_input(user_buffer);

    user_buffer->check_user_input(user_buffer,t);
  }
  return 0;
}
