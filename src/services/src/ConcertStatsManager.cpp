#include <iostream>
#include <vector>

#include "Concert.h"
#include "ConcertStatsManager.h"

void ConcertStatsManager::print_stats(const std::vector<Concert>& concerts)
{
    if (total_shows(concerts) == 0)
    {
        std::cout << "No statistics to show\n";
    }
    else
    {
        std::cout << "Total Shows: " << total_shows(concerts) << '\n';
        std::cout << "Total Cost: " << total_cost(concerts) / 100.0 << '\n';
        std::cout << "Average Cost: £" << average_cost(concerts) / 100.0 << '\n';
    }
}

int32_t ConcertStatsManager::total_shows(const std::vector<Concert>& concerts)
{
    return concerts.size();
}

int32_t ConcertStatsManager::total_cost(const std::vector<Concert>& concerts)
{
    int32_t total{};

    for (const Concert& concert : concerts)
    {
        total += concert.get_cost();
    }

    return total;
}

int32_t ConcertStatsManager::average_cost(const std::vector<Concert>& concerts)
{
    return static_cast<int32_t>(total_cost(concerts) / total_shows(concerts));
}
