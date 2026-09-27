#include <iomanip>
#include <iostream>
#include <vector>
#include <unordered_map>

#include "ArtistStatsManager.h"
#include "Artist.h"
#include "Concert.h"
#include "Utilities.h"

void ArtistStatsManager::print_stats(const std::vector<Artist>& artists, const std::vector<Concert>& concerts)
{
    if (artists.size() == 0)
    {
        std::cout << "No statistics to show\n";
        return;
    }

    for (const Artist& artist : artists)
    {
        bool found_a_concert = false;
        std::string first_seen {};
        std::string last_seen {};
        int count = 0;
        int total_cost = 0;

        for (const Concert& concert : concerts)
        {
            const std::unordered_map<int32_t, ArtistRoles>& artist_id_role_map = concert.get_artists();
            auto it = artist_id_role_map.find(artist.get_id());
            if (it == artist_id_role_map.end())
            {
                continue;
            }

            if (!found_a_concert)
            {
                found_a_concert = true;
                first_seen = last_seen = concert.get_date();
            }
            else
            {
                std::tm first_seen_tm_date = Utilities::parse_date(first_seen);
                time_t first_seen_date = std::mktime(&first_seen_tm_date);

                std::tm last_seen_tm_date = Utilities::parse_date(last_seen);
                time_t last_seen_date = std::mktime(&last_seen_tm_date);

                std::tm concert_tm_date = Utilities::parse_date(concert.get_date());
                time_t concert_date = std::mktime(&concert_tm_date);

                first_seen = std::difftime(concert_date, first_seen_date) < 0 ? concert.get_date() : first_seen;
                last_seen = std::difftime(concert_date, last_seen_date) > 0 ? concert.get_date() : last_seen;
            }

            total_cost += concert.get_cost();
            count++;
        }

        if(count == 0)
        {
            std::cout << "Name: " << artist.get_name() << " has no concerts\n";
            continue;
        }

        std::cout << "Name: " << artist.get_name() << '\n';
        std::cout << "First Seen: " << first_seen << '\n';
        std::cout << "Last Seen: " << last_seen << '\n';
        std::cout << "Times Seen: " << count << '\n';
        std::cout << "Average Cost: £"
                  << std::fixed
                  << std::setprecision(2)
                  << static_cast<double>(total_cost) / count / 100
                  << '\n';
    }
}
