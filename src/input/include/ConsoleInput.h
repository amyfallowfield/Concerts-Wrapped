#ifndef CONSOLE_INPUT_H
#define CONSOLE_INPUT_H

#include <string>

#include "Input.h"

class ConsoleInput : public Input
{
public:
    InputResult<std::string> get_string_input() override;
};

#endif
