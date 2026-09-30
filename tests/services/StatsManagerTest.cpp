#include <gtest/gtest.h>

#include "Artist.h"
#include "Concert.h"
#include "StatsManager.h"
#include "TestData.h"

class StatsManagerTest : public testing::Test
{
protected:
    void SetUp() override
    {
        Artist::reset();
        Concert::reset();
    }
};

TEST_F(StatsManagerTest, print_empty_artists)
{
    Artist artist = TestData::create_test_artist("Benjamin Steer");
    ArtistStats stats = StatsManager::get_artist_stats(artist, {});

    ASSERT_TRUE(stats.name.empty()) << "Name should be empty";
    ASSERT_TRUE(stats.first_seen.empty()) << "First seen should be empty";
    ASSERT_TRUE(stats.last_seen.empty()) << "Last seen should be empty";
    ASSERT_EQ(0, stats.count) << "Count should equal 0";
    ASSERT_EQ(0, stats.total_cost) << "Total cost should equal £0.00";
}

TEST_F(StatsManagerTest, print_one_artist_one_concert)
{
    Concert concert = TestData::create_test_concert1();
    Artist artist = TestData::create_test_artist("Benjamin Steer");

    ArtistStats stats = StatsManager::get_artist_stats(artist, {concert});

    ASSERT_EQ("Benjamin Steer", stats.name) << "Name should equal Benjamin Steer";
    ASSERT_EQ("23-05-2026", stats.first_seen) << "First seen should equal 23-05-2026";
    ASSERT_EQ("23-05-2026", stats.last_seen) << "Last seen should equal 23-05-2026";
    ASSERT_EQ(1, stats.count) << "Count should equal 1";
    ASSERT_EQ(2000, stats.total_cost) << "Total cost should equal £20.00";
}

TEST_F(StatsManagerTest, print_one_artist_two_concerts)
{
    Concert concert1 = TestData::create_test_concert1();
    Concert concert2 = TestData::create_test_concert2();
    Artist artist = TestData::create_test_artist("Benjamin Steer");

    ArtistStats stats = StatsManager::get_artist_stats(artist, {concert1, concert2});

    ASSERT_EQ("Benjamin Steer", stats.name) << "Name should equal Benjamin Steer";
    ASSERT_EQ("23-05-2026", stats.first_seen) << "First seen should equal 23-05-2026";
    ASSERT_EQ("30-05-2026", stats.last_seen) << "Last seen should equal 30-05-2026";
    ASSERT_EQ(2, stats.count) << "Count should equal 2";
    ASSERT_EQ(4500, stats.total_cost) << "Total cost should equal £45.00";
}

TEST_F(StatsManagerTest, print_no_concerts)
{
    ConcertStats stats = StatsManager::get_concert_stats({});

    ASSERT_EQ(0, stats.total_shows) << "Total shows should equal 0";
    ASSERT_EQ(0, stats.total_cost) << "Total cost should equal £0.00";
    ASSERT_EQ(0, stats.average_cost) << "Total shows should equal £0.00";
}

TEST_F(StatsManagerTest, print_one_concert)
{
    Concert concert = TestData::create_test_concert1();

    ConcertStats stats = StatsManager::get_concert_stats({concert});

    ASSERT_EQ(1, stats.total_shows) << "Total shows should equal 1";
    ASSERT_EQ(2000, stats.total_cost) << "Total cost should equal £20.00";
    ASSERT_EQ(2000, stats.average_cost) << "Total shows should equal £20.00";
}

TEST_F(StatsManagerTest, print_two_concerts)
{
    Concert concert1 = TestData::create_test_concert1();
    Concert concert2 = TestData::create_test_concert2();

    ConcertStats stats = StatsManager::get_concert_stats({concert1, concert2});

    ASSERT_EQ(2, stats.total_shows) << "Total shows should equal 2";
    ASSERT_EQ(4500, stats.total_cost) << "Total cost should equal £45.00";
    ASSERT_EQ(2250, stats.average_cost) << "Total shows should equal £22.50";
}
