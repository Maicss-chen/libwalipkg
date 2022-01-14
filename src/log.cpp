//
// Created by maicss on 2022/1/12.
//
#include "log.h"
#include <iostream>
#include <ctime>
#include <iomanip>
std::string log(const std::string& content, LogType type) {
#ifndef _DEBUG
    if(type == NORMAL)
        return content;
#endif

    time_t tt;
    time(&tt);
    tt=tt+8*3600;
    tm* time= gmtime(&tt);

    std::string t;
    switch (type) {
        case NORMAL:
            t="";
            break;
        case WARRING:
            t="[\033[33mWARRING\033[0m]";
            break;
        case ERROR:
            t="[\033[31mERROR\033[0m]";
            break;
    }
    std::cout<<std::endl
             <<time->tm_year+1900<<"-"
             <<std::setw(2)<<std::setfill('0')<<time->tm_mon+1<<"-"
             <<std::setw(2)<<std::setfill('0')<<time->tm_mday<<" "
             <<std::setw(2)<<std::setfill('0')<<time->tm_hour<<":"
             <<std::setw(2)<<std::setfill('0')<<time->tm_min<<":"
             <<std::setw(2)<<std::setfill('0')<<time->tm_sec<<" "
             <<t<<": "
             <<content
             <<std::endl;
    return content;
}
