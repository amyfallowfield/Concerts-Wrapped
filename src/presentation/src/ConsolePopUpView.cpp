#include <iostream>

#include "ConsolePopUpView.h"

void ConsolePopUpView::show_error_message(const std::string &message)
{
    std::cout << message << '\n';
}

void ConsolePopUpView::show_input_request(const std::string &message)
{
    std::cout << "Enter " << message << ": ";
}

void ConsolePopUpView::show_menu_option(const std::string &message)
{
    std::cout << message << '\n';
}
