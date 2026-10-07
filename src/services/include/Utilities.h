#ifndef UTILITIES_H
#define UTILITIES_H

#include <ctime>
#include <sstream>
#include <string>

namespace Utilities
{
    template <typename T>
    bool parse_number(const std::string& raw_input, T& final_input)
    {
        std::istringstream stream(raw_input);
        T parsed_input;
        char extra;

        if (!(stream >> parsed_input) || (stream >> extra))
        {
            return false;
        }

        final_input = parsed_input;
        return true;
    }

    inline std::tm parse_date(int& day, int& month, int& year, std::string input)
    {
        std::tm tm_date = {};
        tm_date.tm_mday = day;
        tm_date.tm_mon  = month - 1;
        tm_date.tm_year = year - 1900;

        return tm_date;
    }

    inline std::tm parse_date(std::string input)
    {
        int day = std::stoi(input.substr(0, 2));
        int month = std::stoi(input.substr(3, 2));
        int year = std::stoi(input.substr(6, 4));

        std::tm tm_date = {};
        tm_date.tm_mday = day;
        tm_date.tm_mon  = month - 1;
        tm_date.tm_year = year - 1900;

        return tm_date;
    }
}

#endif
