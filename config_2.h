#pragma once

#include <iostream>
using namespace std;

const unsigned long long VIRTUAL_MEMORY_SIZE = 0x400000000;  // 16GB
const unsigned long long PHYSICAL_MEMORY_SIZE = 0x200000000;  // 8GB
const unsigned long long PAGE_SIZE = 0x400;  // 1KB
const unsigned long long VIRTUAL_MEMORY_START = 0x0;
const unsigned long long VIRTUAL_MEMORY_LAST = 0x3FFFFFFFF;  // For 16GB virtual memory range
const unsigned long long PHYSICAL_MEMORY_START = 0x0;
const unsigned long long PHYSICAL_MEMORY_LAST = 0x1FFFFFFFF;  // For 8GB physical memory range

// Number of pages for Virtual Memory and Physical Memory
const unsigned long long NUM_VIRTUAL_PAGES = VIRTUAL_MEMORY_SIZE / PAGE_SIZE;
const unsigned long long NUM_PHYSICAL_PAGES = PHYSICAL_MEMORY_SIZE / PAGE_SIZE;

// Memory Sections: Start and End Addresses
const unsigned long long TEXT_START_ADDRESS = 0x00400000;      // Example start address for text section
const unsigned long long TEXT_END_ADDRESS = 0x00FFFFFF;        // Example end address for text section
const unsigned long long DATA_START_ADDRESS = 0x10000000;      // Example start address for data section
const unsigned long long DATA_END_ADDRESS = 0x1FFFFFFF;        // Example end address for data section
const unsigned long long STACK_START_ADDRESS = 0x7FFFFFFF;     // Example start address for stack (typically grows downwards)
const unsigned long long STACK_END_ADDRESS = 0x70000000;       // Example end address for stack section
const unsigned long long SHARED_LIB_START_ADDRESS = 0x80000000;// Example start address for shared libraries
const unsigned long long SHARED_LIB_END_ADDRESS = 0x8FFFFFFF;  // Example end address for shared libraries section
const unsigned long long HEAP_START_ADDRESS = 0x60000000;      // Example start address for heap (typically grows upwards)
const unsigned long long HEAP_END_ADDRESS = 0x6FFFFFFF;        // Example end address for heap section

