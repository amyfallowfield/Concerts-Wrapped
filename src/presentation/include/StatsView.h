#ifndef STATS_VIEW_H
#define STATS_VIEW_H

#include "Artist.h"
#include "StatsManager.h"

class StatsView
{
public:
    virtual void show_concert_stats(const ConcertStats& stats) = 0;
    virtual void show_artist_stats(const ArtistStats& stats) = 0;
};

#endif
