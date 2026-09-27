#ifndef TEST_DATA_H
#define TEST_DATA_H

#include "Artist.h"
#include "Concert.h"

namespace TestData
{
    Artist create_test_artist(std::string name);
    Concert create_test_concert1();
    Concert create_test_concert2();
};

#endif
