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

    ASSERT_EQ(1, concert.get_id()) << "ID should be 1";
    ASSERT_EQ("Benjamin Steer", concert.get_artist()) << "Artist should be Benjamin Steer";
    ASSERT_EQ("Village Underground", concert.get_venue()) << "Venue should be Village Underground";
    ASSERT_EQ("London", concert.get_city()) << "City should be London";
    ASSERT_EQ("23-05-2026", concert.get_date()) << "Date should be 23-05-2026";
    ASSERT_EQ(2000, concert.get_cost()) << "Cost should be 2000 (£20.00";
    ASSERT_EQ(std::vector<std::string>({"Dolder"}), concert.get_supports()) << "Supports should be Dolder";
}

TEST_F(ConcertTest, second_instantiation_from_params_increments_id)
{
    Concert concert1 = TestData::create_test_concert1();
    Concert concert2 = TestData::create_test_concert2();
    
    ASSERT_EQ(2, concert2.get_id()) << "ID for second concert should be 2";
}

TEST_F(ConcertTest, first_instantiation_from_json)
{
    json concert_data = {
        {"id", 1},
        {"artist", "Benjamin Steer"},
        {"venue", "Deaf Institute"},
        {"city", "Manchester"},
        {"date", "30-05-2026"},
        {"cost", 2500},
        {"supports", {"Dolder"}}
    };
    Concert concert = Concert(concert_data);

    ASSERT_EQ(1, concert.get_id()) << "ID should be 1";
    ASSERT_EQ("Benjamin Steer", concert.get_artist()) << "Artist should be Benjamin Steer";
    ASSERT_EQ("Deaf Institute", concert.get_venue()) << "Venue should be Deaf Institute";
    ASSERT_EQ("Manchester", concert.get_city()) << "City should be Manchester";
    ASSERT_EQ("30-05-2026", concert.get_date()) << "Date should be 30-05-2026";
    ASSERT_EQ(2500, concert.get_cost()) << "Cost should be 2500 (£25.00)";
    ASSERT_EQ(std::vector<std::string>({"Dolder"}), concert.get_supports()) << "Supports should be Dolder";
}

TEST_F(ConcertTest, second_instantiation_from_json)
{
    Concert concert1 = TestData::create_test_concert1();
    json concert_data = {
        {"id", 3},
        {"artist", "Benjamin Steer"},
        {"venue", "Village Underground"},
        {"city", "London"},
        {"date", "23-05-2026"},
        {"cost", 2000},
        {"supports", {"Dolder"}}
    };
    Concert concert2 = Concert{concert_data};

    ASSERT_EQ(3, concert2.get_id()) << "ID should be 3";
}

TEST_F(ConcertTest, to_json)
{
    Concert concert = TestData::create_test_concert1();

    json concert_data = {
        {"id", 1},
        {"artist", "Benjamin Steer"},
        {"venue", "Village Underground"},
        {"city", "London"},
        {"date", "23-05-2026"},
        {"cost", 2000},
        {"supports", {"Dolder"}}
    };

    ASSERT_EQ(concert_data, concert.to_json()) << "Concert data should be formatted as json";
}

TEST_F(ConcertTest, operator_equals_when_equal)
{
    json duplicate_concert_data = {
        {"id", 1},
        {"artist", "Benjamin Steer"},
        {"venue", "Village Underground"},
        {"city", "London"},
        {"date", "23-05-2026"},
        {"cost", 2000},
        {"supports", {"Dolder"}}
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

TEST_F(ConcertTest, set_artist)
{
    Concert concert = TestData::create_test_concert1();

    ASSERT_EQ("Benjamin Steer", concert.get_artist()) << "Artist after instantiation should be Benjamin Steer";

    concert.set_artist("Only The Poets");

    ASSERT_EQ("Only The Poets", concert.get_artist()) << "Artist after setter should be Only The Poets";
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
TEST_F(ConcertTest, set_supports)
{
    Concert concert = TestData::create_test_concert1();

    ASSERT_EQ(std::vector<std::string>({"Dolder"}), concert.get_supports()) << "Supports after instantiation should be Dolder";

    concert.set_supports({"Only The Poets", "Tors"});

    ASSERT_EQ(std::vector<std::string>({"Only The Poets", "Tors"}), concert.get_supports()) << "Supports after instantiation should be Only The Poets & Tors";
}

