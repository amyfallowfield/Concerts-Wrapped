#ifndef UTILITIES_H
#define UTILITIES_H

#include <ctime>
#include <string>

struct Utilities
{
    static bool parse_int(int& input);
    static bool parse_float(double& input);
    static std::tm parse_date(int& day, int& month, int& year, std::string input);
    static std::tm parse_date(std::string input);
};

#endif
