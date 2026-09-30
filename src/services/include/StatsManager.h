#ifndef STATS_MANAGER_H
#define STATS_MANAGER_H

#include <vector>

#include "Artist.h"
#include "Concert.h"

struct ArtistStats
{
    std::string name {};
    std::string first_seen {};
    std::string last_seen {};
    int count = 0;
    int total_cost = 0;
    int average_cost = 0;
};

struct ConcertStats
{
    int32_t total_shows;
    int32_t total_cost;
    int32_t average_cost;
};

namespace StatsManager
{
    ConcertStats get_concert_stats(const std::vector<Concert>& concerts);
    ArtistStats get_artist_stats(const Artist& artist, const std::vector<Concert>& concerts);
};

#endif
