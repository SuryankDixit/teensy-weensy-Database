#ifndef _TABLE_
#define _TABLE_

#include<bits/stdc++.h>
using namespace std;

#define NAME_SIZE 31
#define MAIL_SIZE 255


#define size_of_attribute(class,member) sizeof(((class*)0)->member)

class Row{
public:
    uint32_t id;
    char username[NAME_SIZE+1];
    char email[MAIL_SIZE+1];
    Row(){
        username[NAME_SIZE] = '\0';
        email[MAIL_SIZE] = '\0';
    }
    // Serialization is the process of turning an object in memory into a stream of bytes so you can do stuff like store it on disk or send it over the network.
    void serialize_data(Row* source, void* data);
    void deserialize_data(void* source,Row* destination);

    void print_row(Row*);
};

const uint32_t ID_SIZE = size_of_attribute(Row, id);
const uint32_t USERNAME_SIZE = size_of_attribute(Row, username);
const uint32_t EMAIL_SIZE = size_of_attribute(Row, email);
const uint32_t ID_OFFSET = 0;
const uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const uint32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
const uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

#define TABLE_MAX_PAGES 100
const uint32_t PAGE_SIZE = 4096;                        // page size 4 kilobytes because it’s the same size as a page used in the virtual memory systems of most computer architectures
const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;       // 14 row per page
const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;    // 1400 max rows in one table;


typedef struct {
  int file_descriptor;
  uint32_t file_length;
  void* pages[TABLE_MAX_PAGES];
} Pager;



class Table{
    uint32_t num_rows;
    Pager* pager;
public:
        Table(){}

       ~Table(){}
    

    // Rows should not cross page boundaries. Since pages probably won’t exist next to each other in memory
    // this function will return the base address of the row where we will append our row in table
    Pager* get_pager();
    Table* db_open(const char*);
    uint32_t get_num_rows();
    void set_num_rows();
};


class Cursor{
    Table* table;
    uint32_t num_rows;
    
public:

    Cursor(){}
    Cursor(Table* table , int num_rows,bool status){
        this->table = table;
        this->num_rows = num_rows;
        this->end_of_table = status;
    }
    ~Cursor(){}
    
    bool end_of_table;      // position just after the last element

    void* cursor_position(Cursor* cursor);

    void advance_cursor(Cursor* cursor);

    uint32_t get_num_rows();
    void set_num_rows();
    Table* get_table();
};

// creates new cursors;
static Cursor* table_start(Table* table){
    Cursor* cursor = new Cursor(table,0,table->get_num_rows()==0);
    return cursor;
}

static Cursor* table_end(Table* table){
    Cursor* cursor = new Cursor(table,table->get_num_rows(),true);
    return cursor;
}

void db_close(Table*);
void pager_flush(Pager* pager, uint32_t page_num, uint32_t size);
void* get_page(Pager* pager, uint32_t page_num);
Pager* pager_open(const char* filename);

#endif