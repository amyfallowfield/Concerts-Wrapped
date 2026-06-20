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
        std::cout << "Name: " << artist.get_name() << '\n';
        std::cout << "First Seen: "<< artist.get_first_seen() << '\n';
        std::cout << "Last Seen: "<< artist.get_last_seen() << '\n';
        std::cout << "Times Seen: "<< artist.get_count() << '\n';
        std::cout << "Average Cost: £"<< average_cost(artist) / 100 << '\n\n';
    }
}

int32_t ArtistStatsManager::average_cost(const Artist& artist)
{
    return artist.get_total_cost() / artist.get_count();
}
