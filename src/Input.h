//
// Created by Nick Wang on 9/13/20.
//

#ifndef INC_4_INPUT_H
#define INC_4_INPUT_H


struct Input {
     const int m_machineSize;
     const int m_pageSize;
     const int m_processSize;
     const int m_jobMix;
     const int m_numRef;
    std::string m_replaceAlgo;

     Input(int machineSize, int pageSize, int processSize, int jobMix, int numRef, std::string replaceAlgo):
        m_machineSize(machineSize), m_pageSize(pageSize), m_processSize(processSize), m_jobMix(jobMix),
        m_numRef(numRef), m_replaceAlgo(replaceAlgo) {}
};

#endif //INC_4_INPUT_H
