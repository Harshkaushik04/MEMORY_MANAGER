#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "config_2.h"

using namespace std;

// Function to align the generated address to the page size boundary
unsigned long long alignToPageSize(unsigned long long address) {
    return (address / PAGE_SIZE) * PAGE_SIZE;
}

// Function to generate a random address within a given range, aligned to the page size
unsigned long long generateRandomAddress(unsigned long long start, unsigned long long end) {
    unsigned long long range = (end - start) / PAGE_SIZE;
    unsigned long long randomPage = rand() % range;
    return alignToPageSize(start + (randomPage * PAGE_SIZE));
}

// Function to generate a random size (in KB) for each task
unsigned long long generateRandomSize() {
    return (rand() % 16 + 1);  // Random size between 1KB and 16KB
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    unsigned long long numTasks;
    cout << "Enter the number of tasks: ";
    cin >> numTasks;

    ofstream traceFile("trace_new.txt");

    for (unsigned long long i = 0; i < numTasks; ++i) {
        unsigned long long taskID = rand() % (numTasks/1000) + 1;
        unsigned long long sectionChoice = rand() % 5;
        unsigned long long startAddress, endAddress;

        // Choose a section based on sectionChoice
        switch (sectionChoice) {
            case 0:  // Text Section
                startAddress = TEXT_START_ADDRESS;
                endAddress = TEXT_END_ADDRESS;
                break;
            case 1:  // Data Section
                startAddress = DATA_START_ADDRESS;
                endAddress = DATA_END_ADDRESS;
                break;
            case 2:  // Stack Section
                startAddress = STACK_END_ADDRESS;  // Stack grows downwards
                endAddress = STACK_START_ADDRESS;
                break;
            case 3:  // Shared Library Section
                startAddress = SHARED_LIB_START_ADDRESS;
                endAddress = SHARED_LIB_END_ADDRESS;
                break;
            case 4:  // Heap Section
                startAddress = HEAP_START_ADDRESS;
                endAddress = HEAP_END_ADDRESS;
                break;
            default:
                continue;
        }

        // Generate random address within the selected section
        unsigned long long address = generateRandomAddress(startAddress, endAddress);
        unsigned long long size = generateRandomSize();

        // Write to trace file in the required format
        traceFile << "T" << taskID << ":0x" << uppercase << hex << address << ":" << dec << size << "KB" << endl;
    }

    traceFile.close();
    cout << "Trace file generated successfully." << endl;

    return 0;
}

