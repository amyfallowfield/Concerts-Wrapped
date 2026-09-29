#ifndef STATS_VIEW_H
#define STATS_VIEW_H

#include "ConcertStatsManager.h"

class StatsView
{
public:
    virtual void show_concert_stats(const ConcertStats& stats) = 0;
};

#endif
