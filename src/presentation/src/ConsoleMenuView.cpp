#include <iostream>

#include "ConsoleMenuView.h"

void ConsoleMenuView::show_welcome()
{
    std::cout << "\n===== Concerts Wrapped =====\n\n";
}

void ConsoleMenuView::show_main_menu()
{
    std::cout << "1. Add Concert\n";
    std::cout << "2. View All Concerts\n";
    std::cout << "3. Delete A Concert\n";
    std::cout << "4. Edit A Concert\n";
    std::cout << "5. View Concert Stats\n";
    std::cout << "6. View Artist Stats\n";
    std::cout << "7. Exit\n";
    std::cout << "Selection: ";
}
