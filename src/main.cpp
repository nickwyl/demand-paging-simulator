//
// Created by Nick Wang on 4/22/20.
//
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <iomanip>
#include "Process.h"
#include "Page.h"
#include "Input.h"

using std::stoi;

//Load processes
void loadProcesses(std::unordered_map<int,Process> &processes, int jobMix) {
    if (jobMix==1)
        processes.emplace(std::piecewise_construct,std::forward_as_tuple(1),std::forward_as_tuple(1,0,0,1));
    else if (jobMix==2) {
        for (int i=1; i<5; ++i)
            processes.emplace(std::piecewise_construct,std::forward_as_tuple(i),std::forward_as_tuple(1,0,0,i));
    }
    else if (jobMix==3) {
        for (int i=1; i<5; ++i)
            processes.emplace(std::piecewise_construct,std::forward_as_tuple(i),std::forward_as_tuple(0,0,0,i));
    }
    else if (jobMix==4) {
        processes.emplace(std::piecewise_construct,std::forward_as_tuple(1),std::forward_as_tuple(0.75,0.25,0,1));
        processes.emplace(std::piecewise_construct,std::forward_as_tuple(2),std::forward_as_tuple(0.75,0,0.25,2));
        processes.emplace(std::piecewise_construct,std::forward_as_tuple(3),std::forward_as_tuple(0.75,0.125,0.125,3));
        processes.emplace(std::piecewise_construct,std::forward_as_tuple(4),std::forward_as_tuple(.5,0.125,0.125,4));
    }
}

//Print description of input
void printInput(Input &input) {
    std::cout << "The machine size is " << input.m_machineSize << ".\n";
    std::cout << "The page size is " << input.m_pageSize << ".\n";
    std::cout << "The process size is " << input.m_processSize << ".\n";
    std::cout << "The job mix is " << input.m_jobMix << ".\n";
    std::cout << "The number of references per process is " << input.m_numRef << ".\n";
    std::cout << "The replacement algorithm is " << input.m_replaceAlgo << ".\n\n";
}

//Evict a page and return index of free frame
int evict(std::vector<Page> &frameTable,const std::string &replaceAlgo,std::deque<Page> &queuePages,std::queue<Page> &fifoPages,
          std::ifstream &randomIntFile, int evictTime,Process &process) {
    int freeFrame;
    //If FIFO
    if (replaceAlgo=="lru") {
        freeFrame = queuePages.front().frameNum();
        queuePages.pop_front();
    }
    //Else if LRU
    else if (replaceAlgo=="fifo") {
        freeFrame = fifoPages.front().frameNum();
        fifoPages.pop();
    }
    //Else RANDOM
    else {
        //Get next random int
        std::string line;
        getline(randomIntFile, line);
        int r = stoi(line);

        freeFrame = r % frameTable.size();
    }
    //Increment eviction count for process
    process.evicINC();
    //Sum residency time for process whose page was evicted
    process.addResidentTime(evictTime - frameTable[freeFrame].loadTime());

    return freeFrame;
}

void pageFault(Process &process, std::vector<Page> &frameTable, std::deque<Page> &queuePages, std::queue<Page> &fifoPages,
        int pageNum, const std::string &replaceAlgo, int time, std::ifstream &randomIntFile) {

    //Indicate first free frame
    std::pair<bool,int> freeFrame(false,frameTable.size()-1);

    //Iterate to find first free frame
    for (int i = frameTable.size()-1; i>-1; --i, --freeFrame.second) {
        if (frameTable[i].process()==-1 && !freeFrame.first) {
            freeFrame.first = true;
            break;
        }
    }

    //If there are no free frames
    if (!freeFrame.first) {
        //Evict page and return free frame
        freeFrame.second = evict(frameTable,replaceAlgo,queuePages,fifoPages,randomIntFile,time,process);
    }
    //Add page to frame
    frameTable[freeFrame.second] = Page(process.processNum(),pageNum,time,freeFrame.second);

    //Push frames onto stack or queue to keep track of which page was operated on
    if (replaceAlgo=="lru")
        queuePages.push_back(frameTable[freeFrame.second]);
    else if (replaceAlgo=="fifo")
        fifoPages.push(frameTable[freeFrame.second]);
}

//Check if memory reference causes page fault
bool isPageFault(Process &process, std::vector<Page> &frameTable, int pageNum, int time,
        const std::string &replaceAlgo, std::ifstream &randomIntFile) {

    //Queue of pages for LRU
    static std::deque<Page> queuePages;
    //Queue of page for FIFO
    static std::queue<Page> fifoPages;

    //Iterate to find whether page exists in frameTable
    for (int i = frameTable.size()-1; i>-1; --i) {
        //Page exists
        if (frameTable[i].process()==process.processNum() && frameTable[i].pageNum()==pageNum) {
            //If algorithm is LRU, iterate over queuePages and delete the used frame
            if (replaceAlgo=="lru") {
                for (int frame = 0; frame < queuePages.size(); ++frame) {
                    if (queuePages[frame].process()==process.processNum() && queuePages[frame].pageNum()==pageNum) {
                        //Push frame to back of deque, since it's been used
                        queuePages.push_back(frameTable[queuePages[frame].frameNum()]);
                        //Then, erase the first instance of the frame from deque
                        queuePages.erase(queuePages.begin() + frame);
                        break;
                    }
                }
            }
            return false;
        }
    }

    //There is a page fault
    pageFault(process, frameTable, queuePages, fifoPages, pageNum, replaceAlgo, time, randomIntFile);

    return true;
}

//Driver for simulating demand paging
void driver(const Input &input, std::ifstream &randomIntFile, std::unordered_map<int,Process> &processes) {

    //Store total number of references from all processes
    const int totalNumRef = input.m_numRef * processes.size();
    //Store total number of frames
    const int numFrames = input.m_machineSize / input.m_pageSize;
    //Track current reference
    int currentNumRef = 1;
    //Frame Table. The int represents the frame number and page number. The frame at that frame number also has info about the page
    std::vector<Page> frameTable; //Default constructor should initialize Page(-1,-1)
    //Make frameTable more efficient (avoid copying) by reserving capacity of total # of references
    frameTable.reserve(numFrames);
    //Resize frameTable
    frameTable.resize(numFrames);

    //Iterate until there are no more memory references
    while (currentNumRef < totalNumRef + 1) {
        //Iterate through processes
        for (int processNum = 1; processNum<processes.size()+1; ++processNum) {
            processes[processNum].setProcessNum(processNum);
            //Quantum of 3 for each process
            for (int QUANTUM = 3; QUANTUM > 0 && processes[processNum].refMade() < input.m_numRef; --QUANTUM) {
                //Check whether process made any memory references
                if (processes[processNum].noWord()) {
                    //Calculate memory reference (address of word) and store it in m_CurrentWord
                    processes[processNum].setWord((111 * processNum + input.m_processSize) % input.m_processSize);
                    //Indicate that current process has made a memory reference
                    processes[processNum].yesWord();
                }
                int word = processes[processNum].currentWord();
                //Calculate page number
                int pageNum = word / input.m_pageSize;

                //Check if memory reference causes page fault (demand paging simulator). If page fault, increment fault time
                if (isPageFault(processes[processNum], frameTable, pageNum, currentNumRef, input.m_replaceAlgo, randomIntFile)) {
                    processes[processNum].faultINC();
                }

                //Calculate next memory reference
                //______________________________________________________________
                std::string line;
                getline(randomIntFile, line);
                int rand = stoi(line);

                double y = rand / (2147483647 + 1.0);

                //Set next memory reference (next word)
                if (y < processes[processNum].A())
                    processes[processNum].setWord(
                            (processes[processNum].currentWord() + 1 + input.m_processSize) % input.m_processSize);
                else if (y < processes[processNum].A() + processes[processNum].B())
                    processes[processNum].setWord(
                            (processes[processNum].currentWord() - 5 + input.m_processSize) % input.m_processSize);
                else if (y < processes[processNum].A() + processes[processNum].B() + processes[processNum].C())
                    processes[processNum].setWord(
                            (processes[processNum].currentWord() + 4 + input.m_processSize) % input.m_processSize);
                else {
                    getline(randomIntFile, line);
                    rand = stoi(line);
                    processes[processNum].setWord(rand % input.m_processSize);
                }

                //Increment number of references current process made
                processes[processNum].refINC();

                ++currentNumRef;
            }
        }
    }
}

void printResults(std::unordered_map<int,Process> &processes) {
    std::tuple<int,double,double> finalFaultAndRes(0.0,0.0,0.0);
    bool isNAN = true;

    for (int i = 1; i < processes.size()+1; i++) {
        std::cout<<"Process "<<i<<" had "<<processes[i].faultCount()<<" faults";
        if (processes[i].evicCount()==0) {
            std::cout<<".\n\tWith no evictions, the average residence is undefined.\n\n";
        }
        else {
            std::cout<<std::setprecision(17)<<" and "<<(double)processes[i].residentTime()/processes[i].evicCount()
                     << " average residency\n\n";
        }
        //Store count info to calculate average
        std::get<0>(finalFaultAndRes) += processes[i].faultCount();
        std::get<1>(finalFaultAndRes) += processes[i].residentTime();
        std::get<2>(finalFaultAndRes) += processes[i].evicCount();
        //Check for # of eviction
        if (processes[i].evicCount()>0)
            isNAN = false;
    }
    std::cout<<"The total number of faults is "<<std::get<0>(finalFaultAndRes);
    if (isNAN) {
        std::cout<<".\n\tWith no evictions, the overall average residence is undefined.\n";
    }
    else {
        std::cout<<std::setprecision(17)<<" and the overall average residency is "<<
                 (double)std::get<1>(finalFaultAndRes)/std::get<2>(finalFaultAndRes)<<".\n";
    }
}

int main(int argc, char *argv[]) {
    //Check arguments
    if (argc!=7) {
        std::cout << "Must have 6 arguments\n";
        return 1;
    }
    //Read from command line input (6 arguments)
    Input input(stoi(argv[1]),stoi(argv[2]),stoi(argv[3]),
                   stoi(argv[4]),stoi(argv[5]),argv[6]);

    //Unordered map to store processes
    std::unordered_map<int,Process> processes;
    //Load processes
    loadProcesses(processes, input.m_jobMix);

    //Open random int file
    std::ifstream randomIntFile("data/random-numbers");
    //Read random int file
    if (randomIntFile.is_open()) {
        //Print description of input
        printInput(input);
        //Start driver
        driver(input, randomIntFile, processes);
        //Print result
        printResults(processes);
        //Close random int file
        randomIntFile.close();
    }
}
