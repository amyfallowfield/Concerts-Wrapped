#include "Artist.h"
#include "Concert.h"

#include "TestData.h"

namespace TestData
{
    Artist create_test_artist(std::string name)
    {
        return {name};
    }

    Concert create_test_concert1()
    {
        std::unordered_map<int32_t, ArtistRoles> artist_id_role_map {};
        artist_id_role_map.emplace(1, ArtistRoles::Headline);
        artist_id_role_map.emplace(2, ArtistRoles::Support);
        return {{
            {"concert_id", 1},
            {"artists", artist_id_role_map},
            {"venue", "Village Underground"},
            {"city", "London"},
            {"date", "23-05-2026"},
            {"cost", 2000}
        }};
    }

    Concert create_test_concert2()
    {
        std::unordered_map<int32_t, ArtistRoles> artist_id_role_map {};
        artist_id_role_map.emplace(1, ArtistRoles::Headline);
        artist_id_role_map.emplace(2, ArtistRoles::Support);
        return {{
            {"concert_id", 2},
            {"artists", artist_id_role_map},
            {"venue", "Deaf Institute"},
            {"city", "Manchester"},
            {"date", "30-05-2026"},
            {"cost", 2500}
        }};
    }
}
