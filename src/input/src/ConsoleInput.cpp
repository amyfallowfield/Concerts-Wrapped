#include <iostream>
#include <string>

#include "ConsoleInput.h"

InputResult<std::string> ConsoleInput::get_string_input()
{
    InputResult<std::string> input;

    input.was_successful = static_cast<bool>(std::getline(std::cin, input.value));
    return input;
}

