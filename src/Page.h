//
// Created by Nick Wang on 4/30/20.
//

#ifndef INC_4_PAGE_H
#define INC_4_PAGE_H


class Page {
private:
    int m_Process;
    int m_PageNum;
    int m_LoadTime;
    int m_FrameNum;
public:
    //Default constructor
    Page();
    //Constructor to initialize page
    Page(const int &process, const int &pageNum, const int &loadTime, const int &frameNum);
    //Get m_Process
    int process();
    //Get m_PageNum
    int pageNum();
    //Get m_LoadTime
    int loadTime();
    //Get m_LoadTime
    void setLoadTime(const int &loadTime);
    //Get m_FrameNum
    int frameNum();
};

#endif //INC_4_PAGE_H
