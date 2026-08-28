#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <vector>

#include "Artist.h"
#include "Concert.h"

#include "TestData.h"

using json = nlohmann::json;

TEST(ArtistTest, one_concert_from_params)
{
    Concert concert = TestData::create_test_concert1();
    Artist artist = TestData::create_test_artist("Benjamin Steer", {concert});

    ASSERT_EQ("Benjamin Steer", artist.get_name()) << "Artist name should be Benjamin Steer";
    ASSERT_EQ("23-05-2026", artist.get_first_seen()) << "First seen should be 23-05-2026";
    ASSERT_EQ("23-05-2026", artist.get_last_seen()) << "Last seen should be 23-05-2026";
    ASSERT_EQ(1, artist.get_count()) << "Count should be 1";
    ASSERT_EQ(2000, artist.get_total_cost()) << "Cost should be 2000 (£20.00)";
}

TEST(ArtistTest, two_concerts_from_params)
{
    Concert concert1 = TestData::create_test_concert1();
    Concert concert2 = TestData::create_test_concert2();
    
    std::vector<Concert> concerts {};
    concerts.push_back(concert1);
    concerts.push_back(concert2);

    Artist artist = TestData::create_test_artist("Benjamin Steer", concerts);

    ASSERT_EQ("Benjamin Steer", artist.get_name()) << "Artist name should be Benjamin Steer";
    ASSERT_EQ("23-05-2026", artist.get_first_seen()) << "First seen should be 23-05-2026";
    ASSERT_EQ("30-05-2026", artist.get_last_seen()) << "Last seen should be 30-05-2026";
    ASSERT_EQ(2, artist.get_count()) << "Count should be 2";
    ASSERT_EQ(4500, artist.get_total_cost()) << "Cost should be 4500 (£45.00)";
}

TEST(ArtistTest, multiple_concerts_multiple_roles_from_params)
{
    Concert concert1 = TestData::create_test_concert1();
    Concert concert2 = TestData::create_test_concert2();
    Concert concert3 = Concert{"Arthur Hill", "O2 Academy Brixton", "London", "27-11-2025", 3000, {"Benjamin Steer"}};

    std::vector<Concert> concerts {};
    concerts.push_back(concert1);
    concerts.push_back(concert2);
    concerts.push_back(concert3);

    Artist artist = TestData::create_test_artist("Benjamin Steer", concerts);

    ASSERT_EQ("Benjamin Steer", artist.get_name()) << "Artist name should be Benjamin Steer";
    ASSERT_EQ("27-11-2025", artist.get_first_seen()) << "First seen should be 27-11-2025";
    ASSERT_EQ("30-05-2026", artist.get_last_seen()) << "Last seen should be 30-05-2026";
    ASSERT_EQ(3, artist.get_count()) << "Count should be 3";
    ASSERT_EQ(7500, artist.get_total_cost()) << "Cost should be 7500 (£75.00)";
}

TEST(ArtistTest, one_concert_from_json)
{
    json artist_data = {
        {"name", "Benjamin Steer"},
        {"first_seen", "23-05-2026"},
        {"last_seen", "30-05-2026"},
        {"count", 2},
        {"total_cost", 4500}
    };
    Artist artist = Artist(artist_data);

    ASSERT_EQ("Benjamin Steer", artist.get_name()) << "Artist name should be Benjamin Steer";
    ASSERT_EQ("23-05-2026", artist.get_first_seen()) << "First seen should be 23-05-2026";
    ASSERT_EQ("30-05-2026", artist.get_last_seen()) << "Last seen should be 30-05-2026";
    ASSERT_EQ(2, artist.get_count()) << "Count should be 2";
    ASSERT_EQ(4500, artist.get_total_cost()) << "Cost should be 4500 (£45.00)";
}

TEST(ArtistTest, to_json)
{
    Concert concert = TestData::create_test_concert1();
    Artist artist = TestData::create_test_artist("Benjamin Steer", {concert});

    json artist_data = {
        {"name", "Benjamin Steer"},
        {"first_seen", "23-05-2026"},
        {"last_seen", "23-05-2026"},
        {"count", 1},
        {"total_cost", 2000}
    };

    ASSERT_EQ(artist_data, artist.to_json()) << "Artist data should be formatted as json";
}
