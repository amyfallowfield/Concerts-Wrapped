#include "Artist.h"
#include "Concert.h"
#include "Performance.h"

#include "TestData.h"

namespace TestData
{
    Artist create_test_artist(std::string name, std::vector<Concert> concerts)
    {
        return {name, concerts};
    }

    Concert create_test_concert1()
    {
        return {{
            {"id", 1},
            {"artist", "Benjamin Steer"},
            {"venue", "Village Underground"},
            {"city", "London"},
            {"date", "23-05-2026"},
            {"cost", 2000},
            {"supports", {"Dolder"}}
        }};
    }

    Concert create_test_concert2()
    {
        return {{
            {"id", 2},
            {"artist", "Benjamin Steer"},
            {"venue", "Deaf Institute"},
            {"city", "Manchester"},
            {"date", "30-05-2026"},
            {"cost", 2500},
            {"supports", {"Dolder"}}
        }};
    }

    Performance create_test_performance(Concert concert, Artist artist)
    {
        return {concert.get_id(), artist.get_name(), "Headliner"};
    }
}
