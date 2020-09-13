//
// Created by Nick Wang on 4/30/20.
//

#include "Page.h"

//Default constructor
Page::Page() {
    m_Process = -1;
    m_PageNum = -1;
}

//Constructor to initialize page
Page::Page(const int &process, const int &pageNum, const int &loadTime, const int &frameNum) {
    m_Process = process;
    m_PageNum = pageNum;
    m_LoadTime = loadTime;
    m_FrameNum = frameNum;
}

//Get m_Process
int Page::process() {
    return m_Process;
}

//Get m_PageNum
int Page::pageNum() {
    return m_PageNum;
}

//Get m_LoadTime
int Page::loadTime() {
    return m_LoadTime;
}

//Set m_LoadTime
void Page::setLoadTime(const int &loadTime) {
    m_LoadTime = loadTime;
}

//Get m_FrameNum
int Page::frameNum() {
    return m_FrameNum;
}
