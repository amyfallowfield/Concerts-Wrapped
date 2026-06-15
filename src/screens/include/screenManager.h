#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include "ConcertRepository.h"
#include "ArtistStatsManager.h"
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
};

#endif