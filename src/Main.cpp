#include <iostream>
#include <limits>
#include <vector>

#include "ArtistStatsManager.h"
#include "ConcertRepository.h"
#include "ConsoleConcertsView.h"
#include "ConsoleStatsView.h"
#include "ScreenManager.h"

#define LOG_INFO(message) Logger::Info(__FILE__, __func__, message)
#define LOG_WARN(message) Logger::Warn(__FILE__, __func__, message)
#define LOG_ERROR(message) Logger::Error(__FILE__, __func__, message)

int main()
{
    try
    {
        StorageManager storage {"data"};
        ConcertRepository repo {storage};
        ArtistStatsManager artist_stats {};
        ConsoleConcertsView concerts_view {};
        ConsoleStatsView stats_view {};
        ScreenManager screen_manager = ScreenManager(repo, storage, artist_stats, concerts_view, stats_view);
        screen_manager.run();
    }
    catch (const std::exception& e)
    {
        LOG_ERROR(std::string("Fatal error: ") + e.what());
    }
}
