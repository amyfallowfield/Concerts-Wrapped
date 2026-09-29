#include <iostream>
#include <vector>

#include "Concert.h"
#include "ConcertStatsManager.h"

namespace ConcertStatsManager
{
    ConcertStats get_stats(const std::vector<Concert>& concerts)
    {
        ConcertStats output = {};

        output.total_shows = concerts.size();

        for (const Concert& concert : concerts)
        {
            output.total_cost += concert.get_cost();
        }

        output.average_cost = output.total_shows > 0 ? std::round(static_cast<double>(output.total_cost) / output.total_shows) : 0.0;

        return output;
    }
}
