#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include "ArtistStatsManager.h"
#include "ConcertRepository.h"
#include "ConcertStatsManager.h"
#include "StorageManager.h"

enum class Screen
{
    Menu,
    AddConcert,
    ViewConcerts,
    ConcertStats,
    ArtistStats,
    Exit
};

class ScreenManager
{
public:
    ScreenManager();

    void run();

private:
    ArtistStatsManager artist_stats;
    ConcertStatsManager concert_stats;
    StorageManager storage;
    ConcertRepository repo;
    Screen current_screen;

    void show_menu();

    std::string enum_to_string(Screen screen);
};

#endif