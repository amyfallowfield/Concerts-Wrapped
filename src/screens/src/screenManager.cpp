#include "ArtistStatsManager.h"
#include "Concert.h"
#include "ConcertRepository.h"
#include "ConcertStatsManager.h"
#include "Logger.h"
#include "ScreenManager.h"
#include "StorageManager.h"
#include "Utilities.h"

ScreenManager::ScreenManager()
    : current_screen(Screen::Menu)
{}

void ScreenManager::run()
{
    std::cout << "\n===== Concerts Wrapped =====\n\n";

    while (current_screen != Screen::Exit)
    {
        Screen previous_screen = current_screen;

        switch(current_screen)
        {
        case Screen::Menu:
            show_menu();
            break;
        case Screen::AddConcert:
            repo.add();
            current_screen = Screen::Menu;
            break;
        case Screen::ViewConcerts:
            repo.print();
            current_screen = Screen::Menu;
            break;
        case Screen::DeleteConcert:
            repo.remove();
            current_screen = Screen::Menu;
            break;
        case Screen::EditConcert:
            repo.edit();
            current_screen = Screen::Menu;
            break;
        case Screen::ConcertStats:
            concert_stats.print_stats(repo.get_concerts());
            current_screen = Screen::Menu;
            break;
        case Screen::ArtistStats:
            artist_stats.print_stats(repo.get_artists());
            current_screen = Screen::Menu;
            break;
        default:
            throw std::runtime_error("Screen value is not recognised"); 
        }

        Logger::Info("ScreenManager", "run", "Screen changed from " + enum_to_string(previous_screen) + " to " + enum_to_string(current_screen));
    }

    storage.save(repo.get_artists());
    storage.save(repo.get_concerts());
    storage.save(repo.get_performances());
}

void ScreenManager::show_menu()
{
    int input;

    std::cout << "1. Add Concert\n";
    std::cout << "2. View All Concerts\n";
    std::cout << "3. Delete A Concert\n";
    std::cout << "4. Edit A Concert\n";
    std::cout << "5. View Concert Stats\n";
    std::cout << "6. View Artist Stats\n";
    std::cout << "7. Exit\n";
    std::cout << "Selection: ";

    if (!Utilities::parse_int(input) || input < 1 || input > 7)
    {
        std::cout << '\n';
        return;
    }

    std::cout << '\n';

    switch(input)
    {
    case 1:
        current_screen = Screen::AddConcert;
        break;
    case 2:
        current_screen = Screen::ViewConcerts;
        break;
    case 3:
        current_screen = Screen::DeleteConcert;
        break;
    case 4:
        current_screen = Screen::EditConcert;
        break;
    case 5:
        current_screen = Screen::ConcertStats;
        break;
    case 6:
        current_screen = Screen::ArtistStats;
        break;
    case 7:
        current_screen = Screen::Exit;
        break;
    }
}

std::string ScreenManager::enum_to_string(Screen screen)
{
    switch(screen)
    {
    case Screen::Menu: { return "menu"; }
    case Screen::AddConcert: { return "add concert"; }
    case Screen::ViewConcerts: { return "view concerts"; }
    case Screen::DeleteConcert: { return "delete concert"; }
    case Screen::EditConcert: { return "edit concert"; }
    case Screen::ConcertStats: { return "concert stats"; }
    case Screen::ArtistStats: { return "artist stats"; }
    case Screen::Exit: { return "exit"; }
    default: { return "Error"; }
    }
}