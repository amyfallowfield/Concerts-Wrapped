#include <iomanip>
#include <iostream>
#include <vector>
#include <unordered_map>

#include "Artist.h"
#include "Concert.h"
#include "StatsManager.h"
#include "Utilities.h"

namespace StatsManager
{
    ConcertStats get_concert_stats(const std::vector<Concert>& concerts)
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

    ArtistStats get_artist_stats(const Artist& artist, const std::vector<Concert>& concerts)
    {
        ArtistStats output;

        bool found_a_concert = false;

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
                output.first_seen = output.last_seen = concert.get_date();
                output.name = artist.get_name();
            }
            else
            {
                std::tm first_seen_tm_date = Utilities::parse_date(output.first_seen);
                time_t first_seen_date = std::mktime(&first_seen_tm_date);

                std::tm last_seen_tm_date = Utilities::parse_date(output.last_seen);
                time_t last_seen_date = std::mktime(&last_seen_tm_date);

                std::tm concert_tm_date = Utilities::parse_date(concert.get_date());
                time_t concert_date = std::mktime(&concert_tm_date);

                output.first_seen= std::difftime(concert_date, first_seen_date) < 0 ? concert.get_date() : output.first_seen;
                output.last_seen= std::difftime(concert_date, last_seen_date) > 0 ? concert.get_date() : output.last_seen;
            }

            output.total_cost += concert.get_cost();
            output.count++;
        }

        if (found_a_concert)
        {
            output.average_cost = static_cast<double>(output.total_cost) / output.count / 100;
        }

        return output;
    }

}
