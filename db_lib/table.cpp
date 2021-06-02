#include<bits/stdc++.h>
#include<stdio.h>
#include <unistd.h>
#include<errno.h>
#include<fcntl.h>
#include <sys/types.h>
#include<sys/stat.h>
#include "table.h"

using namespace std;

Pager* pager_open(const char* filename)
{
    int fd = open(filename,
                O_RDWR |      // Read/Write mode
                    O_CREAT,  // Create file if it does not exist
                S_IWUSR |     // User write permission
                    S_IRUSR   // User read permission
                );


  if (fd == -1) {
    printf("Unable to open file\n");
    exit(EXIT_FAILURE);
  }

  off_t file_length = lseek(fd, 0, SEEK_END);       // sets cursor at end of file.
//   cout<< file_length<<endl;

  //cout<<file_length<<endl;

  Pager* pager = (Pager*)malloc(sizeof(Pager));
  pager->file_descriptor = fd;
  pager->file_length = file_length;
  pager->num_pages = (file_length / PAGE_SIZE);


//   cout<<fd<<" "<<file_length<<" "<<pager->num_pages<<" ";

  if (file_length % PAGE_SIZE != 0) {
    printf("Db file is not a whole number of pages. Corrupt file.\n");
    exit(EXIT_FAILURE);
   }

  for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
    pager->pages[i] = NULL;
  }

  return pager;
} 
 

Table* Table :: db_open(const char* filename)
{
    Pager* pager = pager_open(filename);
    // uint32_t num_rows = pager->file_length / ROW_SIZE;

    Table* table = new Table;
    table->pager = pager;
    // table->num_rows = num_rows;
    // table->set_root(table,0);
    if(pager->num_pages==0){
        //new db file.
        // page 0 = leaf node;
        void* root = get_page(pager,0);
        initialize_leaf_node(root);
        set_node_root(root, true);
    }
    return table;
}


void* get_page(Pager* pager, uint32_t page_num) {
  if (page_num > TABLE_MAX_PAGES) {
     printf("Tried to fetch page number out of bounds. %d > %d\n", page_num,
     	TABLE_MAX_PAGES);
     exit(EXIT_FAILURE);
  }

  if (pager->pages[page_num] == NULL) {
     // Cache miss. Allocate memory and load from file.
     void* page = malloc(PAGE_SIZE);

     // total nuber of filled pages in pager right now.
     uint32_t num_pages = pager->file_length / PAGE_SIZE;


     // We might save a partial page at the end of the file
     if (pager->file_length % PAGE_SIZE) {
         num_pages += 1;
     }

     if (page_num <= num_pages) {
         lseek(pager->file_descriptor, page_num * PAGE_SIZE, SEEK_SET);
        //  cout<<lseek(pager->file_descriptor, page_num * PAGE_SIZE, SEEK_END)<<endl;
         ssize_t bytes_read = read(pager->file_descriptor, page, PAGE_SIZE);
        //  cout<<endl<<bytes_read<<endl;
         if (bytes_read == -1) {
            printf("Error reading file: %d\n", errno);
            exit(EXIT_FAILURE);
         }
     }

    pager->pages[page_num] = page;
    if (page_num >= pager->num_pages) {
        pager->num_pages = page_num + 1;
    }

  }

//   cout<<pager->num_pages<<" "<<page_num<<endl;
  return pager->pages[page_num];
}

void pager_flush(Pager* pager, uint32_t page_num) {
  if (pager->pages[page_num] == NULL) {
     printf("Tried to flush null page\n");
     exit(EXIT_FAILURE);
  }

  off_t offset = lseek(pager->file_descriptor, page_num * PAGE_SIZE,
     		 SEEK_SET);

  if (offset == -1) {
     printf("Error seeking: %d\n", errno);
     exit(EXIT_FAILURE);
  }

  ssize_t bytes_written = write(
     pager->file_descriptor, pager->pages[page_num], PAGE_SIZE
     );

  if (bytes_written == -1) {
     printf("Error writing: %d\n", errno);
     exit(EXIT_FAILURE);
  }
}

void db_close(Table* table) {
  Pager* pager = table->get_pager();
//   uint32_t num_full_pages = table->get_num_rows() / ROWS_PER_PAGE;

  for (uint32_t i = 0; i < pager->num_pages; i++) {
     if (pager->pages[i] == NULL) {
         continue;
     }
     pager_flush(pager, i);
     free(pager->pages[i]);
     pager->pages[i] = NULL;
  }

  // There may be a partial page to write to the end of the file
  // This should not be needed after we switch to a B-tree
  
//   uint32_t num_additional_rows = table->get_num_rows() % ROWS_PER_PAGE;
//   if (num_additional_rows > 0) {
//      uint32_t page_num = num_full_pages;
//      if (pager->pages[page_num] != NULL) {
//          pager_flush(pager, page_num, num_additional_rows * ROW_SIZE);
//          free(pager->pages[page_num]);
//          pager->pages[page_num] = NULL;
//      }
//   }

  int result = close(pager->file_descriptor);
  if (result == -1) {
     printf("Error closing db file.\n");
     exit(EXIT_FAILURE);
  }
  for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
     void* page = pager->pages[i];
     if (page) {
         free(page);
         pager->pages[i] = NULL;
     }
  }

  free(pager);
  free(table);
}


void* Cursor :: cursor_position(Cursor* cursor){
    // uint32_t row_num = cusror->get_num_rows();
    // uint32_t page_num = row_num / ROWS_PER_PAGE;           // find page number to add row;
    // Table* table = cusror->get_table();
    // void *page = get_page(table->get_pager(), page_num);
    // uint32_t row_offset = row_num % ROWS_PER_PAGE;
    // uint32_t byte_offset = row_offset * ROW_SIZE;
    uint32_t page_num = cursor->page_num;
    void* page = get_page(cursor->table->get_pager(), page_num);
    return leaf_node_value(page, cursor->cell_num);
    // return page+byte_offset;
}


void Cursor :: advance_cursor(Cursor* cursor){
    uint32_t page_num = cursor->page_num;
    void* node = get_page(cursor->table->get_pager(),page_num);
    cursor->cell_num+=1;
    if(cursor->cell_num>=(*leaf_node_num_cells(node))){
        uint32_t next_page_num = *leaf_node_next_leaf(node);
        if (next_page_num == 0) {
        /* This was rightmost leaf */
            cursor->end_of_table = true;
        } else {
            cursor->page_num = next_page_num;
            cursor->cell_num = 0;
        }
    }
}


Pager* Table :: get_pager(){
    return this->pager;
}

uint32_t Table :: get_root(){
    return this->root_page_num;
}

void Table :: set_root(Table* &t,uint32_t page){
    t->root_page_num = page;
    return;
}

void Row :: serialize_data(Row* source, void* destination){
    memcpy(destination+ID_OFFSET ,&(source->id), ID_SIZE);
    memcpy(destination + USERNAME_OFFSET, &(source->username), USERNAME_SIZE);
    memcpy(destination + EMAIL_OFFSET, &(source->email), EMAIL_SIZE);
}
  
void Row :: deserialize_data(void* source,Row* destination){
    memcpy(&(destination->id), source + ID_OFFSET, ID_SIZE);
    memcpy(&(destination->username), source + USERNAME_OFFSET, USERNAME_SIZE);
    memcpy(&(destination->email), source + EMAIL_OFFSET, EMAIL_SIZE);
}


void Row :: print_row(Row* row){
    printf("(%d, %s, %s)\n", row->id, row->username, row->email);
}