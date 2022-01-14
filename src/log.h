//
// Created by maicss on 2022/1/12.
//

#ifndef PKGKIT_LOG_H
#define PKGKIT_LOG_H
#define LOG_FUNCTION_NAME std::string(__FUNCTION__)+"(): "+
#include <string>
typedef enum {
    NORMAL,
    WARRING,
    ERROR,
} LogType;
std::string log(const std::string& content,LogType type=NORMAL);
//void log(char* content,LogType type=NORMAL);

#endif //PKGKIT_LOG_H
