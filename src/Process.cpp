//
// Created by Nick Wang on 4/30/20.
//

#include "Process.h"
#include <iostream>


Process::Process() {
    std::cout<<"Default constructor called \n";
}

//Constructor to instantiate a process
Process::Process(const double &A, const double &B, const double &C, const int &process)
        : m_A(A), m_B(B), m_C(C), m_Process(process), m_NoWord(true), m_ResidentTime(0) {
}

////Get m_Process
//int Process::processNum() const {
//    return m_Process;
//}

//Get m_NoWord
bool Process::noWord() const {
    return m_NoWord;
}
//Set m_NoWord
void Process::yesWord() {
    m_NoWord = false;
}
//Set m_CurrentWord
void Process::setWord(const int &word) {
    m_CurrentWord = word;
}
//Get m_CurrentWord
int Process::currentWord() const {
    return m_CurrentWord;
}
//Get m_A
double Process::A() const {
    return m_A;
}
//Get m_B
double Process::B() const {
    return m_B;
}
//Get m_C
double Process::C() const {
    return m_C;
}
//Increment m_FaultCount
void Process::faultINC() {
    ++m_FaultCount;
}
//Get m_FaultCount
int Process::faultCount() const {
    return m_FaultCount;
}
//Add to m_ResidentTime
void Process::addResidentTime(const int &time) {
    m_ResidentTime += time;
}
//Get m_Rem_ResidentTimesTime
int Process::residentTime() const {
    return m_ResidentTime;
}
//Get m_EvicCount
int Process::evicCount() const {
    return m_EvicCount;
}
//Increment m_EvicCount
void Process::evicINC() {
    ++m_EvicCount;
}
//Get m_RefMade
int Process::refMade() const {
    return m_RefMade;
}
//Set m_RefMade
void Process::refINC() {
    ++m_RefMade;
}
//Get m_Process
int Process::processNum() const {
    return m_Process;
}





