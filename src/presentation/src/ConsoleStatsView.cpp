#include <iostream>

#include "StatsManager.h"
#include "ConsoleStatsView.h"

void ConsoleStatsView::show_concert_stats(const ConcertStats& stats)
{
    if (stats.total_shows == 0)
    {
        std::cout << "No statistics to show\n";
        return;
    }

    std::cout << "Total Shows: " << stats.total_shows << '\n';
    std::cout << "Total Cost: £"
                << std::fixed << std::setprecision(2)
                << stats.total_cost / 100.0f << '\n';
    std::cout << "Average Cost: £"
                << std::fixed << std::setprecision(2)
                << stats.average_cost / 100.0f << '\n';
}

void ConsoleStatsView::show_artist_stats(const ArtistStats& stats)
{
    if(stats.count == 0)
    {
        std::cout << "Name: " << stats.name << " has no concerts\n";
        return;
    }

    std::cout << "Name: " << stats.name << '\n';
    std::cout << "First Seen: " << stats.first_seen << '\n';
    std::cout << "Last Seen: " << stats.last_seen << '\n';
    std::cout << "Times Seen: " << stats.count << '\n';
    std::cout << "Total Spent: £"
              << std::fixed << std::setprecision(2)
              << stats.total_cost / 100.0f << '\n';
    std::cout << "Average Cost: £"
              << std::fixed << std::setprecision(2)
              << stats.average_cost / 100.0f << '\n';
}
