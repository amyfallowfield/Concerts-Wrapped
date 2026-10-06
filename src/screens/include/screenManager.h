#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include "ConcertRepository.h"
#include "ConcertsView.h"
#include "MenuView.h"
#include "PopUpView.h"
#include "StatsManager.h"
#include "StatsView.h"
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
    ScreenManager(ConcertRepository& repo, StorageManager& storage, ConcertsView& concerts_view, StatsView& stats_view, MenuView& menu_view, PopUpView& pop_up_view);

    void run();
    void process_current_screen();

private:
    StorageManager storage;
    ConcertRepository& repo;
    ConcertsView& concerts_view;
    StatsView& stats_view;
    MenuView& menu_view;
    PopUpView& pop_up_view;
    Screen current_screen;

    void show_menu();

    std::string enum_to_string(Screen screen);
};

#endif
