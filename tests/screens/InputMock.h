#ifndef MOCK_INPUT_H
#define MOCK_INPUT_H

#include <gmock/gmock.h>

#include "Input.h"

class InputMock : public Input
{
  public:
    MOCK_METHOD(InputResult<std::string>, get_string_input, (), (override));
};

#endif
