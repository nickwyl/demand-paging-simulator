//
// Created by Nick Wang on 4/30/20.
//

#ifndef INC_4_PROCESS_H
#define INC_4_PROCESS_H

//Store info about a process
class Process {
private:
    //Probabilities
    double m_A;
    double m_B;
    double m_C;
    //Process Number (1-4)
    int m_Process;
    //Stores whether process made any memory references
    bool m_NoWord;

    int m_CurrentWord;
    int m_FaultCount;
    int m_ResidentTime;
    int m_EvicCount;
    int m_RefMade;

public:
    Process();
    //Constructor to initialize process
    Process(const double &A, const double &B, const double &C, const int &process);
//    //Get m_Process
//    int processNum() const;
    //Get m_NoWord
    bool noWord() const;
    //Set m_NoWord
    void yesWord();
    //Set m_CurrentWord
    void setWord(const int &word);
    //Get m_CurrentWord
    int currentWord() const;
    //Get m_A
    double A() const;
    //Get m_B
    double B() const;
    //Get m_C
    double C() const;
    //Increment m_FaultCount
    void faultINC();
    //Get m_FaultCount
    int faultCount() const;
    //Add to m_ResidentTime
    void addResidentTime(const int &time);
    //Get m_ResidentTime
    int residentTime() const;
    //Get m_EvicCount
    int evicCount() const;
    //Increment m_EvicCount
    void evicINC();
    //Get m_RefMade
    int refMade() const;
    //Increment m_RefMade
    void refINC();
    //Get m_Process
    int processNum() const;
};


#endif //INC_4_PROCESS_H


