#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <vector>

#include "Concert.h"

#include "TestData.h"

using json = nlohmann::json;

class ConcertTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        Concert::reset();
    }
};

TEST_F(ConcertTest, first_instantiation_from_params)
{
    Concert concert = TestData::create_test_concert1();
    std::unordered_map<int32_t, ArtistRoles> artist_id_role_map {};
    artist_id_role_map.emplace(1, ArtistRoles::Headline);
    artist_id_role_map.emplace(2, ArtistRoles::Support);

    ASSERT_EQ(1, concert.get_concert_id()) << "ID should be 1";
    ASSERT_EQ(artist_id_role_map, concert.get_artists()) << "Artists map should contain {1, Headline} and {2, Support}";
    ASSERT_EQ("Village Underground", concert.get_venue()) << "Venue should be Village Underground";
    ASSERT_EQ("London", concert.get_city()) << "City should be London";
    ASSERT_EQ("23-05-2026", concert.get_date()) << "Date should be 23-05-2026";
    ASSERT_EQ(2000, concert.get_cost()) << "Cost should be 2000 (£20.00";
}

TEST_F(ConcertTest, second_instantiation_from_params_increments_id)
{
    Concert concert1 = TestData::create_test_concert1();
    Concert concert2 = TestData::create_test_concert2();
    
    ASSERT_EQ(2, concert2.get_concert_id()) << "ID for second concert should be 2";
}

TEST_F(ConcertTest, id_not_reused_after_concert_removed)
{
    std::vector<Concert> concerts {};
    Concert concert1 = TestData::create_test_concert1();
    Concert concert2 = TestData::create_test_concert2();
    concerts.push_back(concert1);
    concerts.push_back(concert2);

    concerts.erase(concerts.begin());

    std::unordered_map<int32_t, ArtistRoles> artist_id_role_map {};
    artist_id_role_map.emplace(3, ArtistRoles::Headline);
    Concert concert3 = Concert{artist_id_role_map, "O2 Academy Brixton", "London", "27-11-2025", 3000};
    concerts.push_back(concert3);

    ASSERT_EQ(3, concert3.get_concert_id()) << "Concert ID should not be reused after removing a concert";
}

TEST_F(ConcertTest, first_instantiation_from_json)
{
    std::unordered_map<int32_t, ArtistRoles> artist_id_role_map {};
    artist_id_role_map.emplace(1, ArtistRoles::Headline);
    artist_id_role_map.emplace(2, ArtistRoles::Support);

    json concert_data = {
        {"concert_id", 1},
        {"artists", artist_id_role_map},
        {"venue", "Deaf Institute"},
        {"city", "Manchester"},
        {"date", "30-05-2026"},
        {"cost", 2500}
    };
    Concert concert = Concert(concert_data);

    ASSERT_EQ(1, concert.get_concert_id()) << "ID should be 1";
    ASSERT_EQ(artist_id_role_map, concert.get_artists()) << "Artists map should contain {1, Headline} and {2, Support}";
    ASSERT_EQ("Deaf Institute", concert.get_venue()) << "Venue should be Deaf Institute";
    ASSERT_EQ("Manchester", concert.get_city()) << "City should be Manchester";
    ASSERT_EQ("30-05-2026", concert.get_date()) << "Date should be 30-05-2026";
    ASSERT_EQ(2500, concert.get_cost()) << "Cost should be 2500 (£25.00)";
}

TEST_F(ConcertTest, second_instantiation_from_json)
{
    Concert concert1 = TestData::create_test_concert1();

    std::unordered_map<int32_t, ArtistRoles> artist_id_role_map {};
    artist_id_role_map.emplace(1, ArtistRoles::Headline);
    artist_id_role_map.emplace(2, ArtistRoles::Support);

    json concert_data = {
        {"concert_id", 3},
        {"artists", artist_id_role_map},
        {"venue", "Village Underground"},
        {"city", "London"},
        {"date", "23-05-2026"},
        {"cost", 2000}
    };
    Concert concert2 = Concert{concert_data};

    ASSERT_EQ(3, concert2.get_concert_id()) << "ID should be 3";
}

TEST_F(ConcertTest, to_json)
{
    Concert concert = TestData::create_test_concert1();

    std::unordered_map<int32_t, ArtistRoles> artist_id_role_map {};
    artist_id_role_map.emplace(1, ArtistRoles::Headline);
    artist_id_role_map.emplace(2, ArtistRoles::Support);

    json concert_data = {
        {"concert_id", 1},
        {"artists", artist_id_role_map},
        {"venue", "Village Underground"},
        {"city", "London"},
        {"date", "23-05-2026"},
        {"cost", 2000}
    };

    ASSERT_EQ(concert_data, concert.to_json()) << "Concert data should be formatted as json";
}

TEST_F(ConcertTest, operator_equals_when_equal)
{
    std::unordered_map<int32_t, ArtistRoles> artist_id_role_map {};
    artist_id_role_map.emplace(1, ArtistRoles::Headline);
    artist_id_role_map.emplace(2, ArtistRoles::Support);

    json duplicate_concert_data = {
        {"concert_id", 1},
        {"artists", artist_id_role_map},
        {"venue", "Village Underground"},
        {"city", "London"},
        {"date", "23-05-2026"},
        {"cost", 2000}
    };

    Concert concert1 = TestData::create_test_concert1();
    Concert concert2 = Concert(duplicate_concert_data);

    ASSERT_EQ(concert1, concert2) << "Concert1 = Concert2 should return true";
}

TEST_F(ConcertTest, operator_equals_when_not_equal)
{
    Concert concert1 = TestData::create_test_concert1();
    Concert concert2 = TestData::create_test_concert2();

    ASSERT_NE(concert1, concert2) << "Concert1 = Concert2 should return false";
}

TEST_F(ConcertTest, set_venue)
{
    Concert concert = TestData::create_test_concert1();

    ASSERT_EQ("Village Underground", concert.get_venue()) << "Venue after instantiation should be Village Underground";

    concert.set_venue("Deaf Institute");

    ASSERT_EQ("Deaf Institute", concert.get_venue()) << "Venue after setter should be Deaf Institute";
}

TEST_F(ConcertTest, set_city)
{
    Concert concert = TestData::create_test_concert1();

    ASSERT_EQ("London", concert.get_city()) << "City after instantiation should be London";

    concert.set_city("Manchester");

    ASSERT_EQ("Manchester", concert.get_city()) << "City after instantiation should be Manchester";
}

TEST_F(ConcertTest, set_date)
{
    Concert concert = TestData::create_test_concert1();

    ASSERT_EQ("23-05-2026", concert.get_date()) << "Date after instantiation should be 23-05-2026";

    concert.set_date("30-05-2026");

    ASSERT_EQ("30-05-2026", concert.get_date()) << "Date after instantiation should be 30-05-2026";
}

TEST_F(ConcertTest, set_cost)
{
    Concert concert = TestData::create_test_concert1();

    ASSERT_EQ(2000, concert.get_cost()) << "Cost after instantiation should be 2000 (£20)";

    concert.set_cost(2500);

    ASSERT_EQ(2500, concert.get_cost()) << "Cost after instantiation should be 2500 (£25)";
}

TEST_F(ConcertTest, add_new_artist)
{
    Concert concert = TestData::create_test_concert1();
    concert.add_artist(3, ArtistRoles::Guest);

    std::unordered_map<int32_t, ArtistRoles> artist_id_role_map {};
    artist_id_role_map.emplace(1, ArtistRoles::Headline);
    artist_id_role_map.emplace(2, ArtistRoles::Support);
    artist_id_role_map.emplace(3, ArtistRoles::Guest);

    ASSERT_EQ(artist_id_role_map, concert.get_artists()) << "New guest artist with ID 3 should be added to artists map";
}

TEST_F(ConcertTest, add_duplicate_artist)
{
    Concert concert = TestData::create_test_concert1();
    concert.add_artist(2, ArtistRoles::Support);

    std::unordered_map<int32_t, ArtistRoles> artist_id_role_map {};
    artist_id_role_map.emplace(1, ArtistRoles::Headline);
    artist_id_role_map.emplace(2, ArtistRoles::Support);

    ASSERT_EQ(artist_id_role_map, concert.get_artists()) << "Duplicate artist should not alter artists map";
}

TEST_F(ConcertTest, add_existing_artist_id_new_role)
{
    Concert concert = TestData::create_test_concert1();
    concert.add_artist(2, ArtistRoles::Guest);

    std::unordered_map<int32_t, ArtistRoles> artist_id_role_map {};
    artist_id_role_map.emplace(1, ArtistRoles::Headline);
    artist_id_role_map.emplace(2, ArtistRoles::Support);
    artist_id_role_map.insert_or_assign(2, ArtistRoles::Guest);

    ASSERT_EQ(artist_id_role_map, concert.get_artists()) << "Existing artist ID with enw role should override role in artists map";
}
