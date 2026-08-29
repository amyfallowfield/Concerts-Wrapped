#include <iostream>
#include <string>

#include "InputManager.h"
#include "Utilities.h"

#define LOG_INFO(message) Logger::Info(__FILE__, __func__, message)
#define LOG_WARN(message) Logger::Warn(__FILE__, __func__, message)
#define LOG_ERROR(message) Logger::Error(__FILE__, __func__, message)

int InputManager::select_attribute()
{
    int input;

    std::cout << "1. Artist\n";
    std::cout << "2. Venue\n";
    std::cout << "3. City\n";
    std::cout << "4. Date\n";
    std::cout << "5. Cost\n";
    std::cout << "Selection: ";

    if (!Utilities::parse_int(input) || input < 1 || input > 5)
    {
        std::cout << '\n';
        LOG_WARN("Invalid attribute selection");
        return -1;
    }

    std::cout << '\n';

    return input;
}

std::string InputManager::get_string_input(std::string prompt)
{
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);

    return input;
}

int32_t InputManager::get_numerical_input(std::string prompt)
{
    int32_t input;

    std::cout << prompt;
    
    if (!Utilities::parse_int(input))
        LOG_WARN("Invalid attribute selection");
        return -1;

    return input;
}

double InputManager::get_decimal_input(std::string prompt)
{
    double input;

    std::cout << prompt;

    if (!Utilities::parse_float(input))
        LOG_WARN("Invalid attribute selection");
        return -1.0;

    return input;
}

bool InputManager::get_bool_input(std::string prompt)
{
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);

    return input.at(0) == 'y';
}
