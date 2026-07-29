#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include "Artist.h"
#include "Concert.h"
#include "Performance.h"

#include "TestData.h"

using json = nlohmann::json;

TEST(PerformanceTest, instantiation_from_params)
{
    Concert concert = TestData::create_concert_params();
    Artist artist = TestData::create_artist_params({concert});
    Performance performance = TestData::create_performance(concert, artist);

    ASSERT_EQ(concert.get_id(), performance.get_show_id()) << "Show ID should match concert ID";
    ASSERT_EQ(artist.get_name(), performance.get_artist()) << "Artist should match artist name";
    ASSERT_EQ("Headliner", performance.get_role()) << "Role should be Headliner";
}

TEST(PerformanceTest, instantiation_from_json)
{
    Concert concert = TestData::create_concert_params();
    Artist artist = TestData::create_artist_params({concert});

    json performance_data = {
        {"show_id", concert.get_id()},
        {"artist", artist.get_name()},
        {"role", "Headliner"}
    };

    Performance performance = Performance{performance_data};

    ASSERT_EQ(concert.get_id(), performance.get_show_id()) << "Show ID should match concert ID";
    ASSERT_EQ(artist.get_name(), performance.get_artist()) << "Artist should match artist name";
    ASSERT_EQ("Headliner", performance.get_role()) << "Role should be Headliner";
}

TEST(PerformanceTest, to_json)
{
    Concert concert = TestData::create_concert_params();
    Artist artist = TestData::create_artist_params({concert});

    json performance_data = {
        {"show_id", concert.get_id()},
        {"artist", artist.get_name()},
        {"role", "Headliner"}
    };

    Performance performance = Performance{performance_data};

    ASSERT_EQ(performance_data, performance.to_json()) << "Performance data should be formatted as json";
}
