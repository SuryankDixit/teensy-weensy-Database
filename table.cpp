#include<bits/stdc++.h>
#include "table.h"

using namespace std;


void* Table :: allocate_row(Table* table ,uint32_t row_num){
    uint32_t page_num = row_num / ROWS_PER_PAGE;           // find page number to add row;
    void *page = table->pages[page_num];
    if(page == nullptr){
        page = table->pages[page_num] = malloc(PAGE_SIZE);
    }
    uint32_t row_offset = row_num % ROWS_PER_PAGE;
    uint32_t byte_offset = row_offset * ROW_SIZE;
    return page+byte_offset;
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


void Table :: set_num_rows(){
    this->num_rows++;
    return;
}

uint32_t Table :: get_num_rows(){
    return this->num_rows;
}

void* Table :: get_pages(){
    return this->pages;
}

