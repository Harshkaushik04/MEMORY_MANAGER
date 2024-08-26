#include<iostream>
#include<unordered_map>
#include <fstream>
#include <sstream>
#include <string>
#include<vector>
#include<ctime>
#include<cmath>
#include "config_1.h"
using namespace std;
//task module
class task{
public:
    string task_id;
    int instance_number;
    task(string task_id,int instance_number){
        this->task_id=task_id;
        this->instance_number=instance_number;
    }
    task(string task_id){
        this->task_id=task_id;
        this->instance_number=1;
    }
};
// bool instance_checker(unordered_map<string,vector<unsigned long long>> map,int instance){
//     bool flag=false;
//     for (pair<string,vector<unsigned long long>> pair : map) {
//         if(pair.first==to_check){
//             flag=true;
//             return flag;
//         }
//     }
//     return flag;
// }

//mapping
bool string_keys_checker_1(unordered_map<string,vector<unsigned long long>> map,string to_check){
    bool flag=false;
    for (pair<string,vector<unsigned long long>> pair : map) {
        if(pair.first==to_check){
            flag=true;
            return flag;
        }
    }
    return flag;
}
bool string_keys_checker_2_1(unordered_map<string,unsigned long long> map,string to_check){
    bool flag=false;
    for (pair<string,unsigned long long> pair : map) {
        if(pair.first==to_check){
            flag=true;
            return flag;
        }
    }
    return flag;
}


bool vec_checker_1(vector<unsigned long long> vec,unsigned long long to_check){
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

vector<pair<unsigned long long,unsigned long long>> physical_frames_1(unsigned long long memory_start,unsigned long long memory_end,unsigned long long page_size){
    vector<pair<unsigned long long,unsigned long long>> frames={};
    long long num_pages=(memory_end-memory_start)/page_size;
    pair<unsigned long long,unsigned long long> frame;
    for(long long i=0;i<num_pages;i++){
        frame={memory_start+i*page_size,memory_start+(i+1)*page_size};
        frames.push_back(frame);
    }
    return frames;
}

unsigned long long frames_allocator_1(vector<pair<unsigned long long,unsigned long long>>* free_frames,vector<pair<unsigned long long,unsigned long long>>* allocated_frames,unsigned long long memory_to_allocate,unsigned long long frame_size){
    int num_frames=memory_to_allocate/frame_size;
    unsigned long long physical_address;
    // vector<pair<unsigned long long,unsigned long long>> frames_to_allocate; //vector of frames
    // vector<pair<unsigned long long,unsigned long long>> actual_free_frames=*free_frames;
    // vector<pair<unsigned long long,unsigned long long>> actual_allocated_frames=*allocated_frames;
    // for(int i=0;i<num_frames;i++){
    //     frames_to_allocate.push_back(actual_free_frames[i]);
    // }
    physical_address=free_frames->at(0).first;
    for(long long i=0;i<num_frames;i++){
        allocated_frames->push_back(free_frames->at(0));
        free_frames->erase(free_frames->begin());
    }
    return physical_address;
}

void IOmanager_mapping(string trace_file_path,int verbose){
    cout<<"MAPPING"<<endl;
    clock_t start=clock();
    //mapping
    unordered_map<string,unsigned long long> memory_map;
    //performance module
    unsigned long long total_memory_allocated=0;
    int num_page_table_hits=0;
    int num_page_table_miss=0;
    int num_keys=0;
    //memory manager
    vector<string> memory_keys;
    vector<pair<unsigned long long,unsigned long long>> free_frames=physical_frames_1(PHYSICAL_MEMORY_START,PHYSICAL_MEMORY_LAST,PAGE_SIZE);
    vector<pair<unsigned long long,unsigned long long>> allocated_frames={};
    unordered_map<string,vector<unsigned long long>> program_to_LA; //mapping from program to list of logical addresses
    unordered_map<unsigned long long,unsigned long long> mpp; //mapping from logical address to physical address
    ifstream traceFile(trace_file_path);
    if (!traceFile) {
        std::cerr << "Error: Could not open the file!" << std::endl;
        return;
    }

    string line;
    int count=0;
    while (getline(traceFile, line)) {
        count+=1;
        vector<pair<unsigned long long,unsigned long long>>* free_frames_address=&free_frames;
        vector<pair<unsigned long long,unsigned long long>>* allocated_frames_address=&allocated_frames;
        stringstream ss(line);
        string task_id, address, size;

        // Parse the line
        getline(ss, task_id, ':');    // Task ID
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
        task new_task=task(task_id);
        unsigned long long actual_address=stoull(address,nullptr,16);
        if(verbose==1){
            cout<<new_task.task_id<<" "<<address<<" "<<size<<":";
        }
        if(string_keys_checker_1(program_to_LA,new_task.task_id)==true){
            if(vec_checker_1(program_to_LA[new_task.task_id],actual_address)==true){ //page table hit
                // cout<<task<<" "<<actual_address<<" "<<address<<" "<<size_allocate<<" "<<size<<endl;
                num_page_table_hits+=1; 
                if(verbose==1){
                    cout<<"Page tabel hit"<<endl;
                }
            }
            else{ //page miss
                num_page_table_miss+=1;
                program_to_LA[new_task.task_id].push_back(actual_address);
                total_memory_allocated+=memory_to_allocate;
                mpp[actual_address]=frames_allocator_1(free_frames_address,allocated_frames_address,memory_to_allocate,PAGE_SIZE);
                if(string_keys_checker_2_1(memory_map,new_task.task_id)==true){
                    memory_map[new_task.task_id]+=memory_to_allocate;
                }
                else{
                    memory_map[new_task.task_id]=memory_to_allocate;
                    memory_keys.push_back(new_task.task_id);
                }
                std::stringstream ss;
                ss<<hex <<mpp[actual_address];
                string hex_string = ss.str();
                if(verbose==1){
                    cout<<hex_string<<endl;
                }
            }
        }
        else{
            num_keys+=1;
            num_page_table_miss+=1;
            program_to_LA[new_task.task_id]={};
            program_to_LA[new_task.task_id].push_back(actual_address);
            total_memory_allocated+=memory_to_allocate;
            mpp[actual_address]=frames_allocator_1(free_frames_address,allocated_frames_address,memory_to_allocate,PAGE_SIZE);
            if(string_keys_checker_2_1(memory_map,new_task.task_id)==true){
                memory_map[new_task.task_id]+=memory_to_allocate;
            }
            else{
                memory_map[new_task.task_id]=memory_to_allocate;
                memory_keys.push_back(new_task.task_id);
            }
            std::stringstream ss;
            ss<<hex <<mpp[actual_address];
            string hex_string = ss.str();
            if(verbose==1){
                cout<<hex_string<<endl;
            }
        }
        // // Output the parsed data (for demonstration purposes)
        // cout << "Task ID: " << task << endl;
        // cout << "Address: " << address << endl;
        // cout << "Size: " << size << endl;
        // cout << endl;
    }
    clock_t end=clock();
    cout<<"=============================================="<<endl;
    cout<<"count:"<<count<<endl;
    cout<<"Time taken:"<<double(end-start)/CLOCKS_PER_SEC<<" sec"<<endl;
        for(int i=0;i<memory_keys.end()-memory_keys.begin();i++){
            cout<<memory_keys[i]<<":"<<memory_map[memory_keys[i]]<<endl;
        }
        cout<<"Page hits:"<<num_page_table_hits<<endl;
        cout<<"Page misses:"<<num_page_table_miss<<endl;
        cout<<"Total memory:"<<PHYSICAL_MEMORY_SIZE<<endl;
        cout<<"Total memory allocated:"<<total_memory_allocated<<endl;
        cout<<"Free Memory:"<<PHYSICAL_MEMORY_SIZE-total_memory_allocated<<endl;
    traceFile.close();
    cout<<"=============================================="<<endl;
}
//single level page table
bool string_keys_checker_2(unordered_map<string,unsigned long*> map,string to_check){
    bool flag=false;
    for (pair<string,unsigned long*> pair : map) {
        if(pair.first==to_check){
            flag=true;
            return flag;
        }
    }
    return flag;
}
bool string_keys_checker_2_2(unordered_map<string,unsigned long long> map,string to_check){
    bool flag=false;
    for (pair<string,unsigned long long> pair : map) {
        if(pair.first==to_check){
            flag=true;
            return flag;
        }
    }
    return flag;
}


// bool vec_checker_2(vector<unsigned long long> vec,unsigned long long to_check){
//     bool flag=false;
//     int vec_length=vec.end()-vec.begin();
//     for(int i=0;i<vec_length;i++) {
//         if(vec[i]==to_check){
//             flag=true;
//             return flag;
//         }
//     }
//     return flag;
// }

vector<unsigned long> physical_frames_numbers_2(unsigned long long memory_start,unsigned long long memory_end,unsigned long long page_size){
    vector<unsigned long> frame_numbers={};
    long num_pages=(memory_end-memory_start)/page_size;
    for(long i=0;i<num_pages;i++){
        frame_numbers.push_back(i);
    }
    return frame_numbers;
}

unsigned long long frames_allocator_2(vector<unsigned long>* free_frame_numbers,vector<unsigned long>* allocated_frame_numbers,unsigned long long memory_to_allocate,unsigned long long frame_size,unsigned long long physical_offset){
    int num_frames=memory_to_allocate/frame_size;
    unsigned long long physical_address;
    // vector<pair<unsigned long long,unsigned long long>> frames_to_allocate; //vector of frames
    // vector<pair<unsigned long long,unsigned long long>> actual_free_frames=*free_frames;
    // vector<pair<unsigned long long,unsigned long long>> actual_allocated_frames=*allocated_frames;
    // for(int i=0;i<num_frames;i++){
    //     frames_to_allocate.push_back(actual_free_frames[i]);
    // }
    physical_address=free_frame_numbers->at(0)*frame_size+physical_offset;
    for(int i=0;i<num_frames;i++){
        allocated_frame_numbers->push_back(free_frame_numbers->at(0));
        free_frame_numbers->erase(free_frame_numbers->begin());
    }
    return physical_address;
}

void IOmanager_single_level_page_table(string trace_file_path,int verbose){
    cout<<"SINGLE LEVEL PAGE TABLE"<<endl;
    clock_t start=clock();
    //single level page table
    long long memory_for_page_tables=0;
    unordered_map<string,unsigned long long> memory_map;
    unsigned long long total_memory_allocated=0;
    long num_page_table_hits=0;
    long num_page_table_miss=0;
    long num_keys=0;
    vector<string> memory_keys;
    vector<unsigned long> free_frame_numbers=physical_frames_numbers_2(PHYSICAL_MEMORY_START,PHYSICAL_MEMORY_LAST,PAGE_SIZE);
    vector<unsigned long> allocated_frame_numbers={};
    unordered_map<string,unsigned long*> program_to_page_table; //mapping from program to respective page table
    ifstream traceFile(trace_file_path);
    if (!traceFile) {
        std::cerr << "Error: Could not open the file!" << std::endl;
        return;
    }

    string line;
    int count=0;
    while (getline(traceFile, line)) {
        count+=1;
        vector<unsigned long>* free_frame__numbers_address=&free_frame_numbers;
        vector<unsigned long>* allocated_frame_numbers_address=&allocated_frame_numbers;
        stringstream ss(line);
        string task_id, address, size;

        // Parse the line
        getline(ss, task_id, ':');    // Task ID
        getline(ss, address, ':'); // Memory address
        getline(ss, size, ':');    // Size
        string size_type=size.substr(size.length()-2,2);
        unsigned long size_allocate=stoull(size.substr(0,size.length()-2));
        unsigned long memory_to_allocate;
        if(size_type=="KB"){
            memory_to_allocate=size_allocate<<10;
        }
        else if(size_type=="MB"){
            memory_to_allocate=size_allocate<<20;
        }
        // else{
        //     cout<<"wtf??"<<endl;
        //     return;
        // }
        task new_task=task(task_id);
        unsigned long long actual_address=stoull(address,nullptr,16);
        unsigned long long frame_offset=(PAGE_SIZE-1)&actual_address; //for page size=1kb,0x3FF
        unsigned long long page_number=actual_address>>(int)log2(PAGE_SIZE); //for page size=1kb,10
        if(verbose==1){
            cout<<new_task.task_id<<" "<<address<<" "<<size<<":";
        }
        if(string_keys_checker_2(program_to_page_table,new_task.task_id)==true){
            if(program_to_page_table[new_task.task_id][page_number]!=0){ //page table hit
                // cout<<task<<" "<<actual_address<<" "<<address<<" "<<size_allocate<<" "<<size<<endl;
                num_page_table_hits+=1; 
                if(verbose==1){
                    cout<<"Page tabel hit"<<endl;
                }
            }
            else{ //page miss
                num_page_table_miss+=1;
                total_memory_allocated+=memory_to_allocate;
                program_to_page_table[new_task.task_id][page_number]=frames_allocator_2(free_frame__numbers_address,allocated_frame_numbers_address,memory_to_allocate,PAGE_SIZE,frame_offset);
                if(string_keys_checker_2_2(memory_map,new_task.task_id)==true){
                    memory_map[new_task.task_id]+=memory_to_allocate;
                }
                else{
                    memory_map[new_task.task_id]=memory_to_allocate;
                    memory_keys.push_back(new_task.task_id);
                }
                std::stringstream ss;
                ss<<hex <<program_to_page_table[new_task.task_id][page_number];
                string hex_string = ss.str();
                if(verbose==1){
                    cout<<hex_string<<endl;
                }
            }
        }
        else{
            num_keys+=1;
            num_page_table_miss+=1;
            memory_for_page_tables+=sizeof(unsigned long)*VIRTUAL_MEMORY_SIZE/PAGE_SIZE;
            program_to_page_table[new_task.task_id]=new unsigned long[VIRTUAL_MEMORY_SIZE/PAGE_SIZE];
            for(long i=0;i<VIRTUAL_MEMORY_SIZE/PAGE_SIZE;i++){
                program_to_page_table[new_task.task_id][i]=0;
            }
            total_memory_allocated+=memory_to_allocate;
            program_to_page_table[new_task.task_id][page_number]=frames_allocator_2(free_frame__numbers_address,allocated_frame_numbers_address,memory_to_allocate,PAGE_SIZE,frame_offset);
            if(string_keys_checker_2_2(memory_map,new_task.task_id)==true){
                memory_map[new_task.task_id]+=memory_to_allocate;
            }
            else{
                memory_map[new_task.task_id]=memory_to_allocate;
                memory_keys.push_back(new_task.task_id);
            }
            std::stringstream ss;
            ss<<hex <<program_to_page_table[new_task.task_id][page_number];
            string hex_string = ss.str();
            if(verbose==1){
                cout<<hex_string<<endl;
            }
        }
        // // Output the parsed data (for demonstration purposes)
        // cout << "Task ID: " << task << endl;
        // cout << "Address: " << address << endl;
        // cout << "Size: " << size << endl;
        // cout << endl;
    }
    clock_t end=clock();
    cout<<"=============================================="<<endl;
    cout<<"count:"<<count<<endl;
    cout<<"Time taken:"<<double(end-start)/CLOCKS_PER_SEC<<" sec"<<endl;
        for(long long i=0;i<memory_keys.end()-memory_keys.begin();i++){
            cout<<memory_keys[i]<<":"<<memory_map[memory_keys[i]]<<endl;
        }
        cout<<"Page hits:"<<num_page_table_hits<<endl;
        cout<<"Page misses:"<<num_page_table_miss<<endl;
        cout<<"Total memory:"<<PHYSICAL_MEMORY_SIZE<<endl;
        cout<<"Total memory allocated:"<<total_memory_allocated<<endl;
        cout<<"Free Memory:"<<PHYSICAL_MEMORY_SIZE-total_memory_allocated<<endl;
        cout<<"Memory took for page tables:"<<memory_for_page_tables<<endl;
    traceFile.close();
    cout<<"=============================================="<<endl;
}
//double level page table
bool string_keys_checker_3(unordered_map<string,unsigned long long**> map,string to_check){
    bool flag=false;
    for (pair<string,unsigned long long**> pair : map) {
        if(pair.first==to_check){
            flag=true;
            return flag;
        }
    }
    return flag;
}
bool string_keys_checker_2_3(unordered_map<string,unsigned long long> map,string to_check){
    bool flag=false;
    for (pair<string,unsigned long long> pair : map) {
        if(pair.first==to_check){
            flag=true;
            return flag;
        }
    }
    return flag;
}


// bool vec_checker_3(vector<unsigned long long> vec,unsigned long long to_check){
//     bool flag=false;
//     int vec_length=vec.end()-vec.begin();
//     for(int i=0;i<vec_length;i++) {
//         if(vec[i]==to_check){
//             flag=true;
//             return flag;
//         }
//     }
//     return flag;
// }

vector<unsigned long long> physical_frames_numbers_3(unsigned long long memory_start,unsigned long long memory_end,unsigned long long page_size){
    vector<unsigned long long> frame_numbers={};
    long long num_pages=(memory_end-memory_start)/page_size;
    for(long long i=0;i<num_pages;i++){
        frame_numbers.push_back(i);
    }
    return frame_numbers;
}
unsigned long long frames_allocator_3(vector<unsigned long long>* free_frame_numbers,vector<unsigned long long>* allocated_frame_numbers,unsigned long long memory_to_allocate,unsigned long long frame_size,unsigned long long physical_offset){
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

void IOmanager_double_level_page_table(string trace_file_path,int verbose){
    cout<<"DOUBLE LEVEL PAGE TABLE"<<endl;
    clock_t start=clock();
    //double level page table
    long long memory_for_page_tables=0;
    unordered_map<string,unsigned long long> memory_map;
    unsigned long long total_memory_allocated=0;
    int num_page_table_hits=0;
    int num_page_table_miss=0;
    int num_keys=0;
    vector<string> memory_keys;
    vector<unsigned long long> free_frame_numbers=physical_frames_numbers_3(PHYSICAL_MEMORY_START,PHYSICAL_MEMORY_LAST,PAGE_SIZE);
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
        string task_id, address, size;

        // Parse the line
        getline(ss, task_id, ':');    // Task ID
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
        task new_task=task(task_id);
        unsigned long long actual_address=stoull(address,nullptr,16);
        unsigned long long frame_offset=(PAGE_SIZE-1)&actual_address; //for page size=1kb
        unsigned long long total_page_number=actual_address>>(int)log2(PAGE_SIZE); //for page size=10kb //10+12+12=34->2^23=16gb=virtual memory size
        unsigned long long page_table_number=total_page_number>>(int)(log2(VIRTUAL_MEMORY_SIZE)-log2(PAGE_SIZE))/2; //first 12 digits of the segmented portion
        unsigned long long page_number=total_page_number&(int)(pow(2,(int)(log2(VIRTUAL_MEMORY_SIZE)-log2(PAGE_SIZE))/2)-1); //last 12 digits of the segmented portion0xFFF
        if(verbose==1){
            cout<<new_task.task_id<<" "<<address<<" "<<size<<":";
        }
        if(string_keys_checker_3(program_to_first_level_page_table,new_task.task_id)==true){
            if(program_to_first_level_page_table[new_task.task_id][page_table_number][page_number]!=0){ //page table hit
                // cout<<task<<" "<<actual_address<<" "<<address<<" "<<size_allocate<<" "<<size<<endl;
                num_page_table_hits+=1; 
                if(verbose==1){
                    cout<<"Page tabel hit"<<endl;
                }
            }
            else{ //page miss
                num_page_table_miss+=1;
                total_memory_allocated+=memory_to_allocate;
                program_to_first_level_page_table[new_task.task_id][page_table_number][page_number]=frames_allocator_3(free_frame__numbers_address,allocated_frame_numbers_address,memory_to_allocate,PAGE_SIZE,frame_offset);
                if(string_keys_checker_2_3(memory_map,new_task.task_id)==true){
                    memory_map[new_task.task_id]+=memory_to_allocate;
                }
                else{
                    memory_map[new_task.task_id]=memory_to_allocate;
                    memory_keys.push_back(new_task.task_id);
                }
                std::stringstream ss;
                ss<<hex <<program_to_first_level_page_table[new_task.task_id][page_table_number][page_number];
                string hex_string = ss.str();
                if(verbose==1){
                    cout<<hex_string<<endl;
                }
            }
        }
        else{
            num_keys+=1;
            num_page_table_miss+=1;
            program_to_first_level_page_table[new_task.task_id]=new unsigned long long*[0x1000];
            if((int)(log2(VIRTUAL_MEMORY_SIZE)-log2(PAGE_SIZE))%2==0){
                for(long i=0;i<pow(2,(int)(log2(VIRTUAL_MEMORY_SIZE)-log2(PAGE_SIZE))/2);i++){ //making 2D array
                    program_to_first_level_page_table[new_task.task_id][i]=new unsigned long long[0x1000];
                    for(long j=0;j<pow(2,(int)(log2(VIRTUAL_MEMORY_SIZE)-log2(PAGE_SIZE))/2);j++){
                        program_to_first_level_page_table[new_task.task_id][i][j]=0;
                    }
                }
            }
            else{
                for(long i=0;i<pow(2,(int)(log2(VIRTUAL_MEMORY_SIZE)-log2(PAGE_SIZE))/2+1);i++){ //making 2D array
                    program_to_first_level_page_table[new_task.task_id][i]=new unsigned long long[0x1000];
                    for(long j=0;j<pow(2,(int)(log2(VIRTUAL_MEMORY_SIZE)-log2(PAGE_SIZE))/2);j++){
                        program_to_first_level_page_table[new_task.task_id][i][j]=0;
                    }
                }
            }
            total_memory_allocated+=memory_to_allocate;
            program_to_first_level_page_table[new_task.task_id][page_table_number][page_number]=frames_allocator_3(free_frame__numbers_address,allocated_frame_numbers_address,memory_to_allocate,PAGE_SIZE,frame_offset);
            if(string_keys_checker_2_3(memory_map,new_task.task_id)==true){
                memory_map[new_task.task_id]+=memory_to_allocate;
            }
            else{
                memory_map[new_task.task_id]=memory_to_allocate;
                memory_keys.push_back(new_task.task_id);
            }
            memory_for_page_tables+=sizeof(unsigned long)*VIRTUAL_MEMORY_SIZE/PAGE_SIZE;
            std::stringstream ss;
            ss<<hex <<program_to_first_level_page_table[new_task.task_id][page_table_number][page_number];
            string hex_string = ss.str();
            if(verbose==1){
                cout<<hex_string<<endl;
            }
        }
        // // Output the parsed data (for demonstration purposes)
        // cout << "Task ID: " << task << endl;
        // cout << "Address: " << address << endl;
        // cout << "Size: " << size << endl;
        // cout << endl;
    }
    clock_t end=clock();
    cout<<"=============================================="<<endl;
    cout<<"count:"<<count<<endl;
    cout<<"Time taken:"<<double(end-start)/CLOCKS_PER_SEC<<" sec"<<endl;
        for(long long i=0;i<memory_keys.end()-memory_keys.begin();i++){
            cout<<memory_keys[i]<<":"<<memory_map[memory_keys[i]]<<endl;
        }
        cout<<"Page hits:"<<num_page_table_hits<<endl;
        cout<<"Page misses:"<<num_page_table_miss<<endl;
        cout<<"Total memory:"<<PHYSICAL_MEMORY_SIZE<<endl;
        cout<<"Total memory allocated:"<<total_memory_allocated<<endl;
        cout<<"Free Memory:"<<PHYSICAL_MEMORY_SIZE-total_memory_allocated<<endl;
        cout<<"Memory for storing page tables:"<<memory_for_page_tables<<endl;
    cout<<"=============================================="<<endl;
    traceFile.close();
}

//1kb_8gb_16gb
int main(){
    int verbose;
    cout<<"Set verbose:(0 for for showing memory allocation,1 for showing)"<<endl;
    cin>>verbose;
    //mapping
    IOmanager_mapping("tracefile_1KB_8GB_16GB.txt",verbose);
    //single level page table
    IOmanager_single_level_page_table("tracefile_1KB_8GB_16GB.txt",verbose);
    //double level page table
    IOmanager_double_level_page_table("tracefile_1KB_8GB_16GB.txt",verbose);
    return 0;
}