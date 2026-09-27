#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <vector>

#include "Artist.h"
#include "Concert.h"

#include "TestData.h"

using json = nlohmann::json;

class ArtistTest : public testing::Test
{
protected:
    void SetUp() override
    {
        Artist::reset();
    }
};

TEST_F(ArtistTest, one_concert_from_params)
{
    Concert concert = TestData::create_test_concert1();
    Artist artist = TestData::create_test_artist("Benjamin Steer");

    ASSERT_EQ(1, artist.get_id()) << "Artist ID should be 1";
    ASSERT_EQ("Benjamin Steer", artist.get_name()) << "Artist name should be Benjamin Steer";
}

TEST_F(ArtistTest, two_concerts_from_params)
{
    Concert concert1 = TestData::create_test_concert1();
    Concert concert2 = TestData::create_test_concert2();
    
    std::vector<Concert> concerts {};
    concerts.push_back(concert1);
    concerts.push_back(concert2);

    Artist artist = TestData::create_test_artist("Benjamin Steer");

    ASSERT_EQ(1, artist.get_id()) << "Artist ID should be 1";
    ASSERT_EQ("Benjamin Steer", artist.get_name()) << "Artist name should be Benjamin Steer";
}

TEST_F(ArtistTest, id_not_reused_after_artist_removed)
{
    std::vector<Artist> artists {};
    Artist artist1 = TestData::create_test_artist("Benjamin Steer");
    Artist artist2 = TestData::create_test_artist("Arthur Hill");
    artists.push_back(artist1);
    artists.push_back(artist2);

    artists.erase(artists.begin());

    Artist artist3 = TestData::create_test_artist("Only The Poets");
    artists.push_back(artist3);

    ASSERT_EQ(3, artist3.get_id()) << "Artist ID should not be reused after removing an artist";
    ASSERT_EQ("Only The Poets", artist3.get_name()) << "Artist name should be Only The Poets";
}

TEST_F(ArtistTest, multiple_concerts_multiple_roles_from_params)
{
    Concert concert1 = TestData::create_test_concert1();
    std::unordered_map<int32_t, ArtistRoles> artist_id_role_map {};
    artist_id_role_map.emplace(2, ArtistRoles::Headline);
    artist_id_role_map.emplace(1, ArtistRoles::Support);
    Concert concert2 = Concert{artist_id_role_map, "O2 Academy Brixton", "London", "27-11-2025", 3000};

    std::vector<Concert> concerts {};
    concerts.push_back(concert1);
    concerts.push_back(concert2);

    Artist artist1 = TestData::create_test_artist("Benjamin Steer");
    Artist artist2 = TestData::create_test_artist("Arthur Hill");

    ASSERT_EQ(1, artist1.get_id()) << "Artist ID should be 1";
    ASSERT_EQ("Benjamin Steer", artist1.get_name()) << "Artist name should be Benjamin Steer";

    ASSERT_EQ(2, artist2.get_id()) << "Artist ID should be 2";
    ASSERT_EQ("Arthur Hill", artist2.get_name()) << "Artist name should be Arthur Hill";
}

TEST_F(ArtistTest, one_concert_from_json)
{
    json artist_data = {
        {"id", 1},
        {"name", "Benjamin Steer"}
    };
    Artist artist = Artist(artist_data);

    ASSERT_EQ(1, artist.get_id()) << "Artist ID should be 1";
    ASSERT_EQ("Benjamin Steer", artist.get_name()) << "Artist name should be Benjamin Steer";
}

TEST_F(ArtistTest, to_json)
{
    Concert concert = TestData::create_test_concert1();
    Artist artist = TestData::create_test_artist("Benjamin Steer");

    json artist_data = {
        {"id", 1},
        {"name", "Benjamin Steer"},
    };

    ASSERT_EQ(artist_data, artist.to_json()) << "Artist data should be formatted as json";
}
