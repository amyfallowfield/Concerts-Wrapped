#include <iostream>
#include <string>

#include "Logger.h"

void Logger::Info(std::string file, std::string function, std::string message)
{
    std::cerr << "[INFO] [" << file << "::" << function << "()] " << message << '\n';
}

void Logger::Warn(std::string file, std::string function, std::string message)
{
    std::cerr << "[WARN] [" << file << "::" << function << "()] " << message << '\n';
}

void Logger::Error(std::string file, std::string function, std::string message)
{
    std::cerr << "[ERROR] [" << file << "::" << function << "()] " << message << '\n';
}
