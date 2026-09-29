#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include "ArtistStatsManager.h"
#include "ConcertRepository.h"
#include "ConcertStatsManager.h"
#include "ConcertsView.h"
#include "StorageManager.h"

enum class Screen
{
    Menu,
    AddConcert,
    ViewConcerts,
    DeleteConcert,
    EditConcert,
    ConcertStats,
    ArtistStats,
    Exit
};

class ScreenManager
{
public:
    ScreenManager(ConcertRepository& repo, StorageManager& storage, ArtistStatsManager& artist_stats, ConcertStatsManager& concert_stats, ConcertsView& concerts_view);

    void run();
    void process_current_screen();

private:
    ArtistStatsManager& artist_stats;
    ConcertStatsManager& concert_stats;
    StorageManager storage;
    ConcertRepository& repo;
    ConcertsView& concerts_view;
    Screen current_screen;

    void show_menu();

    std::string enum_to_string(Screen screen);
};

#endif
