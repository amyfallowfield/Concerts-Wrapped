#include <iostream>

#include "ConcertStatsManager.h"
#include "ConsoleStatsView.h"

void ConsoleStatsView::show_concert_stats(const ConcertStats& stats)
{
    if (stats.total_shows == 0)
    {
        std::cout << "No statistics to show\n";
    }
    else
    {
        std::cout << "Total Shows: " << stats.total_shows << '\n';
        std::cout << "Total Cost: £"
                  << std::fixed
                  << std::setprecision(2)
                  << stats.total_cost / 100.0f
                  << '\n';
        std::cout << "Average Cost: £"
                  << std::fixed
                  << std::setprecision(2)
                  << stats.average_cost / 100.0f
                  << '\n';
    }
}