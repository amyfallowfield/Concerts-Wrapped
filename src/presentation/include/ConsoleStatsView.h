#ifndef CONSOLE_STATS_VIEW_H
#define CONSOLE_STATS_VIEW_H

#include "ConcertStatsManager.h"
#include "StatsView.h"

class ConsoleStatsView : public StatsView
{
public:
    void show_concert_stats(const ConcertStats& stats) override;
};

#endif