#ifndef CONSOLE_STATS_VIEW_H
#define CONSOLE_STATS_VIEW_H

#include "StatsManager.h"
#include "StatsView.h"

class ConsoleStatsView : public StatsView
{
public:
    void show_concert_stats(const ConcertStats& stats) override;
    void show_artist_stats(const ArtistStats& stats) override;
};

#endif