#pragma once

#include<iostream>
using namespace std;

const unsigned long long VIRTUAL_MEMORY_SIZE=0x1000000000; //64GB
const unsigned long long PHYSICAL_MEMORY_SIZE=0x80000000;  //2GB
const unsigned long long PAGE_SIZE=0x1000; //4KB
const unsigned long long VIRTUAL_MEMORY_START=0x0;
const unsigned long long VIRTUAL_MEMORY_LAST=0xFFFFFFFFF;
const unsigned long long PHYSICAL_MEMORY_START=0x0;
const unsigned long long PHYSICAL_MEMORY_LAST=0x7FFFFFFF;