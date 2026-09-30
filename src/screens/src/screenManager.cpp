#include "Concert.h"
#include "ConcertRepository.h"
#include "StatsManager.h"
#include "Logger.h"
#include "ScreenManager.h"
#include "StorageManager.h"
#include "Utilities.h"

#define LOG_INFO(message) Logger::Info(__FILE__, __func__, message)
#define LOG_WARN(message) Logger::Warn(__FILE__, __func__, message)
#define LOG_ERROR(message) Logger::Error(__FILE__, __func__, message)

ScreenManager::ScreenManager(ConcertRepository& repo, StorageManager& storage, ConcertsView& concerts_view, StatsView& stats_view)
    : current_screen(Screen::Menu),
      repo(repo),
      storage(storage),
      concerts_view(concerts_view),
      stats_view(stats_view)
{}

void ScreenManager::run()
{
    std::cout << "\n===== Concerts Wrapped =====\n\n";

    while (current_screen != Screen::Exit)
    {
        process_current_screen();
    }

    storage.save(repo.get_artists());
    storage.save(repo.get_concerts());
}

void ScreenManager::process_current_screen()
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
        concerts_view.show(repo.get_concerts(), repo.get_artists());
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
        stats_view.show_concert_stats(StatsManager::get_concert_stats(repo.get_concerts()));
        current_screen = Screen::Menu;
        break;
    case Screen::ArtistStats:
        for (const Artist& artist : repo.get_artists())
        {
            stats_view.show_artist_stats(StatsManager::get_artist_stats(artist, repo.get_concerts()));
        }
        current_screen = Screen::Menu;
        break;
    default:
        throw std::runtime_error("Screen value is not recognised"); 
    }

    if (previous_screen == current_screen)
    {
        LOG_INFO(enum_to_string(current_screen) + " screen initialised");
    } else {
        LOG_INFO("Screen changed from " + enum_to_string(previous_screen) + " to " + enum_to_string(current_screen));
    }
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

    if (!Utilities::parse_int(input))
    {
        std::cout << '\n';
        return;
    }
    if (input < 1 || input > 7)
    {
        std::cout << "Invalid input\n";
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
