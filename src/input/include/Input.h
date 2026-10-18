#ifndef INPUT_H
#define INPUT_H

#include <sstream>
#include <string>
#include <algorithm>

#include "Utilities.h"

template <typename T>
struct InputResult
{
    T value;
    bool was_successful;
};

class Input
{
public:
    virtual InputResult<std::string> get_string_input() = 0;

    template <typename T>
    InputResult<T> get_numerical_input()
    {
        InputResult<T> final_input;
        InputResult<std::string> raw_input = get_string_input();

        if (!raw_input.was_successful)
        {
            return {T{}, false};
        }

        final_input.was_successful = Utilities::parse_number(raw_input.value, final_input.value);
        return final_input;
    }

    InputResult<bool> get_bool_input()
    {
        InputResult<std::string> raw_input = get_string_input();

        if (!raw_input.was_successful)
        {
            return {false, false};
        }

        std::string string_value = raw_input.value;
        std::transform(string_value.begin(), string_value.end(), string_value.begin(),
            [](unsigned char c)
            {return std::tolower(c); });
        bool bool_value = string_value == "y" || string_value == "yes" ? true : false;

        return {bool_value, true};
    }

};

#endif
