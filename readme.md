In this project, we have made 3 types of memory managers:

\(1\) based on mapping

\(2\) based on single level page table

\(3\) based on double level page table

# 1.Mapping

we created 2 maps: program_to_LA,mpp

program_to_LA maped from program/task name(string) to list of logical
addresses(vector of unsigned long long)

mpp maped from logical address(unsigned long long) to physical
address(unsigned long long)

we also created a map memory_map from task name(string) to the memory
allocated to that task instance, this map is part of the performance
module.

Time taken for mapping,other performance module things:

(1) 1000 lines of instructions:

![image1](https://github.com/user-attachments/assets/00cde877-9215-45cb-bb72-4fc23b03c869)


Mapping took about 300 sec, by extrapolation(since it's a linear
process), it would take about 300000 sec for 1000000 lines of
instruction which is about 83.33 hr for mapping.

# 2.Single level page table

In this,we created free_frame_numbers and allocated_frame_numbers for
physical memory management and created a program_to_page_table map,
mapping from program/task string to its respective page table(array) and
we are storing the physical frame number in the index respective to the
logical address given by a particular task.

We also create a memory_map similar to in mapping in which we map each
task with memory allocated to it.

Time taken for single level page table,other performance module things:

(1) 1000 lines of instruction:

![image2](https://github.com/user-attachments/assets/038cd45e-daa6-4678-81cd-b6eac3eaafce)


Single level page table for 1000 lines took 30.9 sec for memory
allocation.

(2) 10000 lines of instruction

![image3](https://github.com/user-attachments/assets/30e18503-c8a6-49a6-b891-4401f5dcc905)

![image4](https://github.com/user-attachments/assets/2524b700-39fd-4249-81a9-7dd1deeb5f11)



Single level page table memory allocation for 10000 lines toom about 300
sec.

By extrapolation, for 1000000 lines of instruction it would take about
8hr.

# 3.Double level page table

Physical memory management is mostly similar to the single level page
table implementation, just here we created 2 levels of page tables in
form of matrix instead of an array.

For example if:

Virtual memory=16gb

Physical memory=8gb

Page size=1kb

-   Virtual memory=2\^34 bytes, page size=2\^10 bytes=\> in logical
    address, bits allocated for page table indexing would be from 0-23,
    here we allocated first 12 bits to page table index amd last 12 to
    page number in that particular page table.

Time taken for single level page table,other performance module things:

(1) 1000 lines of instruction:

![image5](https://github.com/user-attachments/assets/a24badca-34ae-4929-bc6d-031097baa583)



Time taken for 1000 lines of instruction: 31.9 sec

(2)10000 lines of instructions:
![image6](https://github.com/user-attachments/assets/3d57a660-0ae3-4029-8351-cb8386eab22c)


![image7](https://github.com/user-attachments/assets/6f023de6-4c8f-4663-8335-7bcccd82cbd5)



Time taken for 10000 lines of instructions:335.74sec

By exptrapolation,it would take about 9.32hr for it to do for 1000000
lines of instructions.
