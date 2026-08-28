#ifndef TEST_DATA_H
#define TEST_DATA_H

#include "Artist.h"
#include "Concert.h"
#include "Performance.h"

namespace TestData
{
    Artist create_test_artist(std::string name, std::vector<Concert> concerts);
    Concert create_test_concert1();
    Concert create_test_concert2();
    Performance create_test_performance(Concert concert, Artist artist);
};

#endif
