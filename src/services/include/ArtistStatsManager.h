#ifndef ARTIST_STATS_MANAGER_H
#define ARTIST_STATS_MANAGER_H

#include <vector>

#include "Artist.h"

class ArtistStatsManager
{
public:
    void print_stats(const std::vector<Artist>& artists);

private:
    int32_t average_cost(const Artist& artist);
};

#endif
