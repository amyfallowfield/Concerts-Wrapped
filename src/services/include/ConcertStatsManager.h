#ifndef CONCERT_STATS_MANAGER_H
#define CONCERT_STATS_MANAGER_H

#include <vector>

#include "Concert.h"

struct ConcertStats
{
    int32_t total_shows;
    int32_t total_cost;
    int32_t average_cost;
};

namespace ConcertStatsManager
{
    ConcertStats get_stats(const std::vector<Concert>& concerts);
};

#endif
