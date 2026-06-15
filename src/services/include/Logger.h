#ifndef LOGGER_H
#define LOGGER_H

#include <string>

struct Logger
{
    static void Info(std::string file, std::string function, std::string message);
    static void Warn(std::string file, std::string function, std::string message);
    static void Error(std::string file, std::string function, std::string message);
};

#endif