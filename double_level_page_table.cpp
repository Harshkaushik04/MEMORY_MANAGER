#include<iostream>
#include<unordered_map>
#include <fstream>
#include <sstream>
#include <string>
#include<vector>
#include "config_1.h"
using namespace std;


// //page table implementation

// class page{
// public:
//     int page_size;
//     unsigned int page_address;
//     unsigned int inner_pagetable_address;
// public:
//     page(int page_size,unsigned int page_address,unsigned int inner_pagetable_address){
//         this->page_size=page_size;
//         this->page_address=page_address;
//         this->inner_pagetable_address=inner_pagetable_address;
//     }
// };
// //second level page table
// class inner_page_table{ 
// public:
//     int inner_page_table_size;
//     unsigned int inner_pagetable_address;
//     unsigned int outer_pagertable_address;
// public:
//     inner_page_table(int inner_page_table_size,unsigned int inner_pagetable_address,unsigned int outer_pagertable_address){
//         this->inner_page_table_size=inner_page_table_size;
//         this->inner_pagetable_address=inner_pagetable_address;
//         this->outer_pagertable_address=outer_pagertable_address;
//     }
// };
// //first level page table
// class outer_page_table{
// public:
//     int outer_page_table_size;
//     unsigned int outer_page_table_address;
// public:
//     outer_page_table(int outer_page_table_size,unsigned int outer_page_table_address){
//         this->outer_page_table_size=outer_page_table_size;
//         this->outer_page_table_address=outer_page_table_address;
//     }
// };

bool string_keys_checker(unordered_map<string,unsigned long long**> map,string to_check){
    bool flag=false;
    for (pair<string,unsigned long long**> pair : map) {
        if(pair.first==to_check){
            flag=true;
            return flag;
        }
    }
    return flag;
}
bool string_keys_checker_2(unordered_map<string,unsigned long long> map,string to_check){
    bool flag=false;
    for (pair<string,unsigned long long> pair : map) {
        if(pair.first==to_check){
            flag=true;
            return flag;
        }
    }
    return flag;
}


bool vec_checker(vector<unsigned long long> vec,unsigned long long to_check){
    bool flag=false;
    int vec_length=vec.end()-vec.begin();
    for(int i=0;i<vec_length;i++) {
        if(vec[i]==to_check){
            flag=true;
            return flag;
        }
    }
    return flag;
}

vector<unsigned long long> physical_frames_numbers(unsigned long long memory_start,unsigned long long memory_end,unsigned long long page_size){
    vector<unsigned long long> frame_numbers={};
    long long num_pages=(memory_end-memory_start)/page_size;
    for(long long i=0;i<num_pages;i++){
        frame_numbers.push_back(i);
    }
    return frame_numbers;
}
unsigned long long frames_allocator(vector<unsigned long long>* free_frame_numbers,vector<unsigned long long>* allocated_frame_numbers,unsigned long long memory_to_allocate,unsigned long long frame_size,unsigned long long physical_offset){
    long long num_frames=memory_to_allocate/frame_size;
    unsigned long long physical_address;
    // vector<pair<unsigned long long,unsigned long long>> frames_to_allocate; //vector of frames
    // vector<pair<unsigned long long,unsigned long long>> actual_free_frames=*free_frames;
    // vector<pair<unsigned long long,unsigned long long>> actual_allocated_frames=*allocated_frames;
    // for(int i=0;i<num_frames;i++){
    //     frames_to_allocate.push_back(actual_free_frames[i]);
    // }
    physical_address=free_frame_numbers->at(0)*frame_size+physical_offset;
    for(long long i=0;i<num_frames;i++){
        allocated_frame_numbers->push_back(free_frame_numbers->at(0));
        free_frame_numbers->erase(free_frame_numbers->begin());
    }
    return physical_address;
}

void IOmanager(string trace_file_path){
    //mapping
    unordered_map<string,unsigned long long> memory_map;
    unsigned long long total_memory_allocated=0;
    int num_page_table_hits=0;
    int num_page_table_miss=0;
    int num_keys=0;
    vector<string> memory_keys;
    vector<unsigned long long> free_frame_numbers=physical_frames_numbers(PHYSICAL_MEMORY_START,PHYSICAL_MEMORY_LAST,PAGE_SIZE);
    vector<unsigned long long> allocated_frame_numbers={};
    unordered_map<string,unsigned long long**> program_to_first_level_page_table; //mapping from program to respective page table
    ifstream traceFile(trace_file_path);
    if (!traceFile) {
        std::cerr << "Error: Could not open the file!" << std::endl;
        return;
    }

    string line;
    int count=0;
    while (getline(traceFile, line)) {
        count+=1;
        vector<unsigned long long>* free_frame__numbers_address=&free_frame_numbers;
        vector<unsigned long long>* allocated_frame_numbers_address=&allocated_frame_numbers;
        stringstream ss(line);
        string task, address, size;

        // Parse the line
        getline(ss, task, ':');    // Task ID
        getline(ss, address, ':'); // Memory address
        getline(ss, size, ':');    // Size
        string size_type=size.substr(size.length()-2,2);
        unsigned long long size_allocate=stoull(size.substr(0,size.length()-2));
        unsigned long long memory_to_allocate;
        if(size_type=="KB"){
            memory_to_allocate=size_allocate<<10;
        }
        else if(size_type=="MB"){
            memory_to_allocate=size_allocate<<20;
        }
        else{
            cout<<"wtf??"<<endl;
            return;
        }
        unsigned long long actual_address=stoull(address,nullptr,16);
        unsigned long long frame_offset=0x3FF&actual_address; //for page size=1kb
        unsigned long long total_page_number=actual_address>>10; //for page size=10kb //10+12+12=34->2^23=16gb=virtual memory size
        unsigned long long page_table_number=total_page_number>>12; //first 12 digits of the segmented portion
        unsigned long long page_number=total_page_number&0xFFF; //last 12 digits of the segmented portion0xFFF
        cout<<task<<" "<<address<<" "<<size<<":";
        if(string_keys_checker(program_to_first_level_page_table,task)==true){
            if(program_to_first_level_page_table[task][page_table_number][page_number]!=0){ //page table hit
                // cout<<task<<" "<<actual_address<<" "<<address<<" "<<size_allocate<<" "<<size<<endl;
                num_page_table_hits+=1; 
                cout<<"Page tabel hit"<<endl;
            }
            else{ //page miss
                num_page_table_miss+=1;
                total_memory_allocated+=memory_to_allocate;
                program_to_first_level_page_table[task][page_table_number][page_number]=frames_allocator(free_frame__numbers_address,allocated_frame_numbers_address,memory_to_allocate,PAGE_SIZE,frame_offset);
                if(string_keys_checker_2(memory_map,task)==true){
                    memory_map[task]+=memory_to_allocate;
                }
                else{
                    memory_map[task]=memory_to_allocate;
                    memory_keys.push_back(task);
                }
                std::stringstream ss;
                ss<<hex <<program_to_first_level_page_table[task][page_table_number][page_number];
                string hex_string = ss.str();
                cout<<hex_string<<endl;
            }
        }
        else{
            num_keys+=1;
            num_page_table_miss+=1;
            program_to_first_level_page_table[task]=new unsigned long long*[0x1000];
            for(long long i=0;i<0x1000;i++){ //making 2D array
                program_to_first_level_page_table[task][i]=new unsigned long long[0x1000];
                for(long long j=0;j<0x1000;j++){
                    program_to_first_level_page_table[task][i][j]=0;
                }
            }
            total_memory_allocated+=memory_to_allocate;
            program_to_first_level_page_table[task][page_table_number][page_number]=frames_allocator(free_frame__numbers_address,allocated_frame_numbers_address,memory_to_allocate,PAGE_SIZE,frame_offset);
            if(string_keys_checker_2(memory_map,task)==true){
                memory_map[task]+=memory_to_allocate;
            }
            else{
                memory_map[task]=memory_to_allocate;
                memory_keys.push_back(task);
            }
            std::stringstream ss;
            ss<<hex <<program_to_first_level_page_table[task][page_table_number][page_number];
            string hex_string = ss.str();
            cout<<hex_string<<endl;
        }
        // // Output the parsed data (for demonstration purposes)
        // cout << "Task ID: " << task << endl;
        // cout << "Address: " << address << endl;
        // cout << "Size: " << size << endl;
        // cout << endl;
    }
    cout<<"=============================================="<<endl;
    cout<<"count:"<<count<<endl;
        for(long long i=0;i<memory_keys.end()-memory_keys.begin();i++){
            cout<<memory_keys[i]<<":"<<memory_map[memory_keys[i]]<<endl;
        }
        cout<<"Page hits:"<<num_page_table_hits<<endl;
        cout<<"Page misses:"<<num_page_table_miss<<endl;
        cout<<"Total memory:"<<PHYSICAL_MEMORY_SIZE<<endl;
        cout<<"Total memory allocated:"<<total_memory_allocated<<endl;
        cout<<"Free Memory:"<<PHYSICAL_MEMORY_SIZE-total_memory_allocated<<endl;
    traceFile.close();
}

//1kb_8gb_16gb
//maximum 368 mb would be used for page table(single level paging)
int main(){
    //mapping
    IOmanager("/home/harsh/visual_studio_code/memory_manager/tracefile_1KB_8GB_16GB.txt");
    return 0;
}