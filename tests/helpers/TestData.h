#ifndef TEST_DATA_H
#define TEST_DATA_H

#include "Artist.h"
#include "Concert.h"
#include "Performance.h"

namespace TestData
{
    Artist create_artist_params(std::vector<Concert> concerts);
    Artist create_artist_json();
    Concert create_concert_params();
    Concert create_concert_json();
    Performance create_performance(Concert concert, Artist artist);
};

#endif
