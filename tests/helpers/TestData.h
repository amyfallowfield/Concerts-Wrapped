#ifndef TEST_DATA_H
#define TEST_DATA_H

#include "Artist.h"
#include "ConcertArtist.h"
#include "Concert.h"

namespace TestData
{
Artist create_test_artist(std::string name);
ConcertArtist create_artist_concert(int32_t concert_id, int32_t artist_id, ArtistRoles role);
Concert create_test_concert1();
Concert create_test_concert2();
}; // namespace TestData

#endif
