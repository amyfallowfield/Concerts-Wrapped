#include "Artist.h"
#include "Concert.h"

#include "TestData.h"

namespace TestData
{
Artist create_test_artist(std::string name)
{
    return {name};
}

ConcertArtist create_artist_concert(int32_t concert_id, int32_t artist_id, ArtistRoles role)
{
    return {concert_id, artist_id, role};
}

Concert create_test_concert1()
{
    return {{{"concert_id", 1},
             {"venue", "Village Underground"},
             {"city", "London"},
             {"event_date", "23-05-2026"},
             {"cost", 2000}}};
}

Concert create_test_concert2()
{
    return {{{"concert_id", 2},
             {"venue", "Deaf Institute"},
             {"city", "Manchester"},
             {"event_date", "30-05-2026"},
             {"cost", 2500}}};
}
} // namespace TestData
