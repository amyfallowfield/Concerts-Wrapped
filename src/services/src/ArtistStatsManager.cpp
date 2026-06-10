#include <iostream>
#include <vector>

#include "ArtistStatsManager.h"
#include "Artist.h"

void ArtistStatsManager::print_stats(const std::vector<Artist>& artists)
{
    if (artists.size() == 0)
    {
        std::cout << "No statistics to show\n";
        return;
    }

    for (const Artist& artist : artists)
    {
        std::cout << "Name: " << artist.name << '\n';
        std::cout << "First Seen: "<< artist.first_seen << '\n';
        std::cout << "Last Seen: "<< artist.last_seen << '\n';
        std::cout << "Times Seen: "<< artist.count << '\n';
        std::cout << "Average Cost: £"<< average_cost(artist) / 100 << '\n\n';
    }
}

int32_t ArtistStatsManager::average_cost(const Artist& artist)
{
    return artist.total_cost / artist.count;
}