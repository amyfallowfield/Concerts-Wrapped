#include <iostream>

#include "ConsolePopUpView.h"

void ConsolePopUpView::show_error_message(const std::string& message)
{
    std::cout << message << '\n';
}
