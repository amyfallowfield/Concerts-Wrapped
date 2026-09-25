#include <iostream>
#include <string>

#include "InputManager.h"
#include "Utilities.h"

#define LOG_INFO(message) Logger::Info(__FILE__, __func__, message)
#define LOG_WARN(message) Logger::Warn(__FILE__, __func__, message)
#define LOG_ERROR(message) Logger::Error(__FILE__, __func__, message)

int InputManager::select_attribute()
{
    std::cout << "1. Artist\n";
    std::cout << "2. Venue\n";
    std::cout << "3. City\n";
    std::cout << "4. Date\n";
    std::cout << "5. Cost\n";
    std::cout << "6. Supports\n";
    std::cout << "Selection: ";

    int input = _get_bounded_numerical_int(1, 6);

    std::cout << '\n';

    return input;
}

SupportUpdateRequest InputManager::get_support_update_data(const std::vector<std::string>& supports)
{
    SupportUpdateRequest output;

    std::cout << "1. Add Suppport\n";
    std::cout << "2. Edit Suppport\n";
    std::cout << "3. Delete Suppport\n";
    std::cout << "Selection: ";

    int action_input = _get_bounded_numerical_int(1, 3);
    if (action_input == -1)
    {
        output.success = false;
        return output;
    }

    if (action_input == static_cast<int>(Actions::Edit) || action_input == static_cast<int>(Actions::Delete))
    {
        if(supports.empty())
        {
            output.success = false;
            return output;
        }

        for (int i = 0; i < supports.size(); i++)
        {
            std::cout << i << ". " << supports.at(i);
        }

        int index_input = _get_bounded_numerical_int(0, supports.size() - 1);
        if (index_input == -1)
        {
            output.success = false;
            return output;
        }
        output.index = index_input;
    }

    output.success = true;
    output.action = static_cast<Actions>(action_input);
    return output;
}

int InputManager::_get_bounded_numerical_int(int lower, int upper)
{
    int input;

    if (!Utilities::parse_int(input) || input < lower || input > upper)
    {
        std::cout << '\n';
        LOG_WARN("Selection out of allowed range");
        return -1;
    }

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
    {
        LOG_WARN("Invalid attribute selection");
        return -1;
    }

    return input;
}

double InputManager::get_decimal_input(std::string prompt)
{
    double input;

    std::cout << prompt;

    if (!Utilities::parse_float(input))
    {
        LOG_WARN("Invalid attribute selection");
        return -1.0;
    }

    return input;
}

bool InputManager::get_bool_input(std::string prompt)
{
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);

    return input.at(0) == 'y';
}
