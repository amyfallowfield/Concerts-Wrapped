#include <iostream>
#include <limits>
#include <vector>

#include "ArtistStatsManager.h"
#include "ConcertStatsManager.h"
#include "ConcertRepository.h"
#include "ScreenManager.h"

int main()
{
    try
    {
        StorageManager storage {"data"};
        ConcertRepository repo {storage};
        ArtistStatsManager artist_stats {};
        ConcertStatsManager concert_stats {};
        ScreenManager screen_manager = ScreenManager(repo, storage, artist_stats, concert_stats);
        screen_manager.run();
    }
    catch (const std::exception& e)
    {
        std::cout << "Fatal error: " << e.what() << "\n";
    }
}
