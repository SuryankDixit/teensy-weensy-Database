#ifndef _TABLE_
#define _TABLE_

#include<bits/stdc++.h>
using namespace std;

#define NAME_SIZE 31
#define MAIL_SIZE 255


#define size_of_attribute(class,member) sizeof(((class*)0)->member)

typedef enum { 
    NODE_INTERNAL, 
    NODE_LEAF 
} NodeType;


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
// const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;       // 14 row per page
// const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;    // 1400 max rows in one table;


//Common Node Header Layout
const uint32_t NODE_TYPE_SIZE = sizeof(uint8_t);
const uint32_t NODE_TYPE_OFFSET = 0;
const uint32_t IS_ROOT_SIZE = sizeof(uint8_t);
const uint32_t IS_ROOT_OFFSET = NODE_TYPE_SIZE;
const uint32_t PARENT_POINTER_SIZE = sizeof(uint32_t);
const uint32_t PARENT_POINTER_OFFSET = IS_ROOT_OFFSET + IS_ROOT_SIZE;
const uint8_t COMMON_NODE_HEADER_SIZE = NODE_TYPE_SIZE + IS_ROOT_SIZE + PARENT_POINTER_SIZE;

//Leaf Node Header Layout
const uint32_t LEAF_NODE_NUM_CELLS_SIZE = sizeof(uint32_t);
const uint32_t LEAF_NODE_NUM_CELLS_OFFSET = COMMON_NODE_HEADER_SIZE;
const uint32_t LEAF_NODE_NEXT_LEAF_SIZE = sizeof(uint32_t);
const uint32_t LEAF_NODE_NEXT_LEAF_OFFSET =
    LEAF_NODE_NUM_CELLS_OFFSET + LEAF_NODE_NUM_CELLS_SIZE;
const uint32_t LEAF_NODE_HEADER_SIZE = COMMON_NODE_HEADER_SIZE + LEAF_NODE_NUM_CELLS_SIZE+LEAF_NODE_NEXT_LEAF_SIZE;


//Leaf Node Body Layout
const uint32_t LEAF_NODE_KEY_SIZE = sizeof(uint32_t);
const uint32_t LEAF_NODE_KEY_OFFSET = 0;
const uint32_t LEAF_NODE_VALUE_SIZE = ROW_SIZE;
const uint32_t LEAF_NODE_VALUE_OFFSET = LEAF_NODE_KEY_OFFSET + LEAF_NODE_KEY_SIZE;
const uint32_t LEAF_NODE_CELL_SIZE = LEAF_NODE_KEY_SIZE + LEAF_NODE_VALUE_SIZE;
const uint32_t LEAF_NODE_SPACE_FOR_CELLS = PAGE_SIZE - LEAF_NODE_HEADER_SIZE;
const uint32_t LEAF_NODE_MAX_CELLS = LEAF_NODE_SPACE_FOR_CELLS / LEAF_NODE_CELL_SIZE;


typedef struct {
  int file_descriptor;
  uint32_t file_length;
  uint32_t num_pages;
  void* pages[TABLE_MAX_PAGES];
} Pager;



class Table{
    uint32_t root_page_num;
    Pager* pager;
public:
        Table(){}

       ~Table(){}
    

    // Rows should not cross page boundaries. Since pages probably won’t exist next to each other in memory
    // this function will return the base address of the row where we will append our row in table
    Pager* get_pager();
    uint32_t get_root();
    void set_root(Table* &t, uint32_t);
    Table* db_open(const char*);
};


class Cursor{
    
public:

    Table* table;
    uint32_t page_num;
    uint32_t cell_num;

    Cursor(){}
    ~Cursor(){}
    
    bool end_of_table;      // position just after the last element

    void* cursor_position(Cursor* cursor);

    void advance_cursor(Cursor* cursor);
};

void db_close(Table*);
void pager_flush(Pager* pager, uint32_t page_num);
void* get_page(Pager* pager, uint32_t page_num);
Pager* pager_open(const char* filename);


void leaf_node_insert(Cursor* cursor, uint32_t key, Row* value);
Cursor* leaf_node_find(Table* table, uint32_t page_num, uint32_t key);
Cursor* table_find(Table* table, uint32_t key);
NodeType get_node_type(void* node);
void set_node_type(void* node, NodeType type);



static uint32_t* leaf_node_num_cells(void* node) {
  void* ptr = node + LEAF_NODE_NUM_CELLS_OFFSET;
  return (uint32_t*)ptr;
}

static void* leaf_node_cell(void* node, uint32_t cell_num) {
  return node + LEAF_NODE_HEADER_SIZE + cell_num * LEAF_NODE_CELL_SIZE;
}

static uint32_t* leaf_node_key(void* node, uint32_t cell_num) {
  void* ptr = leaf_node_cell(node, cell_num);
  return (uint32_t*)ptr;
}

static void* leaf_node_value(void* node, uint32_t cell_num) {
  return leaf_node_cell(node, cell_num) + LEAF_NODE_KEY_SIZE;
}

static void initialize_leaf_node(void* node) {
  set_node_type(node, NODE_LEAF);
  *leaf_node_num_cells(node) = 0;
}

static uint32_t* leaf_node_next_leaf(void* node) {
  void* ptr = node + LEAF_NODE_NEXT_LEAF_OFFSET;
  return (uint32_t*)ptr;
}




// creates new cursors;
static Cursor* table_start(Table* table){
    Cursor* cursor = new Cursor();
    cursor->table = table;
    cursor->page_num = table->get_root();

    void* root = get_page(table->get_pager(),table->get_root());
    uint32_t num_cells = *leaf_node_num_cells(root);
    cursor->end_of_table = (num_cells==0);

    return cursor;
}

// static Cursor* table_end(Table* table){
//     Cursor* cursor = new Cursor();
//     cursor->table = table;
//     cursor->page_num = table->get_root();

//     void* root = get_page(table->get_pager(),table->get_root());
//     uint32_t cell_num = *leaf_node_num_cells(root);
//     cursor->cell_num = cell_num;
//     cursor->end_of_table = true;
//     return cursor;
// }

static void print_leaf_node(void* node) {
  uint32_t num_cells = *leaf_node_num_cells(node);
  printf("leaf (size %d)\n", num_cells);
  for (uint32_t i = 0; i < num_cells; i++) {
    uint32_t key = *leaf_node_key(node, i);
    printf("  - %d : %d\n", i, key);
  }
}



static void print_constants() {
  printf("ROW_SIZE: %d\n", ROW_SIZE);
  printf("COMMON_NODE_HEADER_SIZE: %d\n", COMMON_NODE_HEADER_SIZE);
  printf("LEAF_NODE_HEADER_SIZE: %d\n", LEAF_NODE_HEADER_SIZE);
  printf("LEAF_NODE_CELL_SIZE: %d\n", LEAF_NODE_CELL_SIZE);
  printf("LEAF_NODE_SPACE_FOR_CELLS: %d\n", LEAF_NODE_SPACE_FOR_CELLS);
  printf("LEAF_NODE_MAX_CELLS: %d\n", LEAF_NODE_MAX_CELLS);
}

#endif
