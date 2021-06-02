#include <bits/stdc++.h>
#include "db_lib/connect.h"

using namespace std;

class student{
  public:
    int id;
    string name;
    string email;
    student(int id,string name,string email){
      this->id = id;
      this->name = name;
      this->email = email;
    }
};

int main(int argc, char* argv[]) {

  database* db = new database;
  db->connect_db_console(argc,argv,db);
  // db->connect_db(argc,argv,db);
  // student obj[] = {
  //         student(1,"suryank","sk@gmail.com"),
  //         student(2,"suryank","sk@gmail.com"),
  //         student(3,"suryank","sk@gmail.com"),
  //         student(4,"suryank","sk@gmail.com"),
  //         student(5,"suryank","sk@gmail.com"),
  //         student(6,"suryank","sk@gmail.com"),
  //         student(7,"suryank","sk@gmail.com"),
  //         student(8,"suryank","sk@gmail.com"),
  //         student(9,"suryank","sk@gmail.com"),
  //         student(10,"suryank","sk@gmail.com"),
  //         student(12,"suryank","sk@gmail.com"),
  //         student(13,"suryank","sk@gmail.com"),
  //         student(14,"suryank","sk@gmail.com"),
  //         student(15,"suryank","sk@gmail.com"),
  //         student(16,"suryank","sk@gmail.com"),
  //         student(17,"suryank","sk@gmail.com"),
  //         student(18,"suryank","sk@gmail.com"),
  //         student(19,"suryank","sk@gmail.com"),
  // };

  // for(int i=0;i<18;i++){
  //   db->insert(obj[i].id,obj[i].name,obj[i].email,db);
  // }

  // db->print_db(db);
  // db->print_tree(db);
  // db->close_db(db);
  return 0;
}