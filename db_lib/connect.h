#ifndef __CONNECT__
#define __CONNECT__

#include "table.h"

class database{
    public:
        Table* table;
        void connect_db_console(int,char**,database*);
        void connect_db(int,char**,database* db);
        void insert(int,string,string,database* db);
        void print_db(database* db);
        void close_db(database* db);
        void print_tree(database* db);
};

#endif