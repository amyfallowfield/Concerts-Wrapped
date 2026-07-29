#include "Artist.h"
#include "Concert.h"
#include "Performance.h"

#include "TestData.h"

namespace TestData
{
    Artist create_artist_params(std::vector<Concert> concerts)
    {
        return {"Benjamin Steer", concerts};
    }

    Artist create_artist_json()
    {
        json artist_data = {
            {"name", "Benjamin Steer"},
            {"first_seen", "23-05-2026"},
            {"last_seen", "30-05-2026"},
            {"count", 2},
            {"total_cost", 4500}
        };
        return {artist_data};
    }

    Concert create_concert_params()
    {
        return {"Benjamin Steer", "Village Underground", "London", "23-05-2026", 2000, {"Dolder"}};
    }

    Concert create_concert_json()
    {
        json concert_data = {
            {"id", 1},
            {"artist", "Benjamin Steer"},
            {"venue", "Deaf Institute"},
            {"city", "Manchester"},
            {"date", "30-05-2026"},
            {"cost", 2500},
            {"supports", {"Dolder"}}
        };
        return {concert_data};
    }

    Performance create_performance(Concert concert, Artist artist)
    {
        return {concert.get_id(), artist.get_name(), "Headliner"};
    }
}
