#include <iostream>
#include <limits>
#include <vector>

#include "ConcertRepository.h"
#include "ConsoleConcertsView.h"
#include "ConsoleStatsView.h"
#include "ConsoleMenuView.h"
#include "ConsolePopUpView.h"
#include "ScreenManager.h"
#include "StatsManager.h"

#define LOG_INFO(message) Logger::Info(__FILE__, __func__, message)
#define LOG_WARN(message) Logger::Warn(__FILE__, __func__, message)
#define LOG_ERROR(message) Logger::Error(__FILE__, __func__, message)

int main()
{
    try
    {
        StorageManager storage {"data"};
        ConcertRepository repo {storage};
        ConsoleConcertsView concerts_view {};
        ConsoleStatsView stats_view {};
        ConsoleMenuView menu_view {};
        ConsolePopUpView pop_up_view {};
        ScreenManager screen_manager = ScreenManager(repo, storage, concerts_view, stats_view, menu_view, pop_up_view);
        screen_manager.run();
    }
    catch (const std::exception& e)
    {
        LOG_ERROR(std::string("Fatal error: ") + e.what());
    }
}
