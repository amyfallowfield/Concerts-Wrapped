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

void ConsoleMenuView::show_edit_menu()
{
    std::cout << "1. Artists\n";
    std::cout << "2. Venue\n";
    std::cout << "3. City\n";
    std::cout << "4. Date\n";
    std::cout << "5. Cost\n";
    std::cout << "Selection: ";
}

void ConsoleMenuView::show_edit_artist_options()
{
    std::cout << "1. Add Artist\n";
    std::cout << "2. Edit Artist\n";
    std::cout << "3. Delete Artist\n";
    std::cout << "Selection: ";
}
