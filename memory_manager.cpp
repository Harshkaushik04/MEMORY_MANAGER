#include<iostream>
#include "config.h"

using namespace std;
//page table implementation

class page{
public:
    int page_size;
    unsigned int page_address;
    unsigned int inner_pagetable_address;
public:
    page(int page_size,unsigned int page_address,unsigned int inner_pagetable_address){
        this->page_size=page_size;
        this->page_address=page_address;
        this->inner_pagetable_address=inner_pagetable_address;
    }
};

class inner_page_table{
public:
    int inner_page_table_size;
    unsigned int inner_pagetable_address;
    unsigned int outer_pagertable_address;
public:
    inner_page_table(int inner_page_table_size,unsigned int inner_pagetable_address,unsigned int outer_pagertable_address){
        this->inner_page_table_size=inner_page_table_size;
        this->inner_pagetable_address=inner_pagetable_address;
        this->outer_pagertable_address=outer_pagertable_address;
    }
};

class outer_page_table{
public:
    int outer_page_table_size;
    unsigned int outer_page_table_address;
public:
    outer_page_table(int outer_page_table_size,unsigned int outer_page_table_address){
        this->outer_page_table_size=outer_page_table_size;
        this->outer_page_table_address=outer_page_table_address;
    }
};

int main(){
    return 0;
}