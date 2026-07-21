#include <ctime>
#include <iostream>
#include <limits>
#include <string>

#include "Utilities.h"

bool Utilities::parse_int(int& input)
{
    if (!(std::cin >> input))
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Invalid input\n";
        return false;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return true;
}

bool Utilities::parse_float(double& input)
{
    if (!(std::cin >> input))
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Invalid input\n";
        return false;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return true;
}

std::tm Utilities::parse_date(int& day, int& month, int& year, std::string input)
{
    std::tm tm_date = {};
    tm_date.tm_mday = day;
    tm_date.tm_mon  = month - 1;
    tm_date.tm_year = year - 1900;

    return tm_date;
}

std::tm Utilities::parse_date(std::string input)
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
