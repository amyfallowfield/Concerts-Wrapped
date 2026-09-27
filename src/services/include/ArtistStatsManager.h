#ifndef ARTIST_STATS_MANAGER_H
#define ARTIST_STATS_MANAGER_H

#include <vector>

#include "Artist.h"
#include "Concert.h"

class ArtistStatsManager
{
public:
    virtual void print_stats(const std::vector<Artist>& artists, const std::vector<Concert>& concerts);
};

#endif
