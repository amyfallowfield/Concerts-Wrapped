#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include "Artist.h"
#include "Concert.h"
#include "Performance.h"

using json = nlohmann::json;

TEST(PerformanceTest, instantiation_from_params)
{
    std::vector<std::string> supports {"Dolder"};
    Concert concert = Concert{"Benjamin Steer", "Village Underground", "London", "23-05-2026", 2000, supports};
    std::vector<Concert> concerts {};
    concerts.push_back(concert);
    Artist artist = Artist{"Benjamin Steer", concerts};

    Performance performance = Performance{concert.get_id(), artist.get_name(), "Headliner"};

    ASSERT_EQ(concert.get_id(), performance.get_show_id()) << "Show ID should match concert ID";
    ASSERT_EQ(artist.get_name(), performance.get_artist()) << "Artist should match artist name";
    ASSERT_EQ("Headliner", performance.get_role()) << "Role should be Headliner";
}

TEST(PerformanceTest, instantiation_from_json)
{
    std::vector<std::string> supports {"Dolder"};
    Concert concert = Concert{"Benjamin Steer", "Village Underground", "London", "23-05-2026", 2000, supports};
    std::vector<Concert> concerts {};
    concerts.push_back(concert);
    Artist artist = Artist{"Benjamin Steer", concerts};

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
    std::vector<std::string> supports {"Dolder"};
    Concert concert = Concert{"Benjamin Steer", "Village Underground", "London", "23-05-2026", 2000, supports};
    std::vector<Concert> concerts {};
    concerts.push_back(concert);
    Artist artist = Artist{"Benjamin Steer", concerts};

    json performance_data = {
        {"show_id", concert.get_id()},
        {"artist", artist.get_name()},
        {"role", "Headliner"}
    };

    Performance performance = Performance{performance_data};

    ASSERT_EQ(performance_data, performance.to_json()) << "Performance data should be formatted as json";
}
