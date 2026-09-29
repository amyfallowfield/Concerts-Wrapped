#include <gtest/gtest.h>

#include "Concert.h"
#include "ConcertStatsManager.h"
#include "TestData.h"

TEST(ConcertStatsManagerTest, print_no_concerts)
{
    ConcertStats stats = ConcertStatsManager::get_stats({});

    ASSERT_EQ(0, stats.total_shows) << "Total shows should equal 0";
    ASSERT_EQ(0, stats.total_cost) << "Total cost should equal £0.00";
    ASSERT_EQ(0, stats.average_cost) << "Total shows should equal £0.00";
}

TEST(ConcertStatsManagerTest, print_one_concert)
{
    Concert concert = TestData::create_test_concert1();

    ConcertStats stats = ConcertStatsManager::get_stats({concert});

    ASSERT_EQ(1, stats.total_shows) << "Total shows should equal 1";
    ASSERT_EQ(2000, stats.total_cost) << "Total cost should equal £20.00";
    ASSERT_EQ(2000, stats.average_cost) << "Total shows should equal £20.00";
}

TEST(ConcertStatsManagerTest, print_two_concerts)
{
    Concert concert1 = TestData::create_test_concert1();
    Concert concert2 = TestData::create_test_concert2();

    ConcertStats stats = ConcertStatsManager::get_stats({concert1, concert2});

    ASSERT_EQ(2, stats.total_shows) << "Total shows should equal 2";
    ASSERT_EQ(4500, stats.total_cost) << "Total cost should equal £45.00";
    ASSERT_EQ(2250, stats.average_cost) << "Total shows should equal £22.50";
}
