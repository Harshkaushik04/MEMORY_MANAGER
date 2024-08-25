#pragma once

#include<iostream>
using namespace std;

const unsigned long long VIRTUAL_MEMORY_SIZE = 0x400000000;  // 16GB
const unsigned long long PHYSICAL_MEMORY_SIZE = 0x200000000;  // 8GB
const unsigned long long PAGE_SIZE = 0x400;                  // 1KB
const unsigned long long VIRTUAL_MEMORY_START = 0x0;
const unsigned long long VIRTUAL_MEMORY_LAST = 0x3FFFFFFFF;  // For 16GB virtual memory range
const unsigned long long PHYSICAL_MEMORY_START = 0x0;
const unsigned long long PHYSICAL_MEMORY_LAST = 0x1FFFFFFFF;  // For 8GB physical memory range
const unsigned long long NUM_SECOND_LEVEL_PAGES=VIRTUAL_MEMORY_SIZE/PAGE_SIZE;
const unsigned long long SECOND_LEVEL_PAGE_TABLE_SIZE=4*NUM_SECOND_LEVEL_PAGES; // 19 bits would be the optimal amount of memory needed but to make it 2 divisible, we keep 32 bits=>4 bytes
const unsigned long long SECOND_LEVEL_PAGE_TABLE_SECTION_START=0x80000001;  //64MB
const unsigned long long SECOND_LEVEL_PAGE_TABLE_SECTION_END=0x84000001;
// const unsigned long long FIRST_LEVEL_PAGE_TABLE_SECTION_START=0x84000002;  //64MB
// const unsigned long long FIRST_LEVEL_PAGE_TABLE_SECTION_END=0x84000002;