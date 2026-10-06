#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include "ComponentManager.h"
#include "ConcertRepository.h"
#include "StatsManager.h"
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
    ScreenManager(ConcertRepository& repo, StorageManager& storage, UIComponent& ui);

    void run();
    void process_current_screen();

private:
    StorageManager storage;
    ConcertRepository& repo;
    UIComponent& ui;
    Screen current_screen;

    void show_menu();

    std::string enum_to_string(Screen screen);
};

#endif
