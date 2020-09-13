# Demand Paging Simulator

This is a program that simulates demand paging. Based on the command line input, generate a number of memory references; if a reference causes a page fault and no free frame exists, a resident page will be evicted based on the given replacement algorithm.

### Compiling the Project

To compile program:

`$ g++ -std=c++0x -o a.out main.cpp Page.cpp Process.cpp`

I am assuming that the "random-numbers" file is in the same directory as "main.cpp".

To run the executable:

`$ ./a.out M P S J N R`

<br />

What each letter means:

• M, the machine size in words.  
• P, the page size in words.  
• S, the size of each process.  
• J, the “job mix”.  
• N, the number of references for each process.  
• R, the replacement algorithm, fifo, random, or lru (all lowercase).
