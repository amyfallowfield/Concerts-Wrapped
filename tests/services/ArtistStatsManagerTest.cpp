#include <gtest/gtest.h>

#include "Artist.h"
#include "ArtistStatsManager.h"
#include "Concert.h"
#include "TestData.h"

class ArtistStatsManagerTest : public testing::Test
{
public:
    std::ostringstream output;

    std::streambuf* original_cout;

    ArtistStatsManager manager;

    void SetUp() override
    {
        Artist::reset();

        original_cout = std::cout.rdbuf();

        std::cout.rdbuf(output.rdbuf());

        ArtistStatsManager manager = ArtistStatsManager();
    }

    void TearDown() override
    {
        std::cout.rdbuf(original_cout);
    }
};

TEST_F(ArtistStatsManagerTest, print_empty_artists)
{
    manager.print_stats({}, {});

    ASSERT_NE(std::string::npos, output.str().find("No statistics to show")) << "Print stats should inform user of no statistics when no artists";
}

TEST_F(ArtistStatsManagerTest, print_one_artist_one_concert)
{
    Concert concert = TestData::create_test_concert1();
    Artist artist = TestData::create_test_artist("Benjamin Steer");

    manager.print_stats({artist}, {concert});

    ASSERT_NE(std::string::npos, output.str().find("Name: Benjamin Steer")) << "Print stats should print artist name";
    ASSERT_NE(std::string::npos, output.str().find("First Seen: 23-05-2026")) << "Print stats should print first seen date";
    ASSERT_NE(std::string::npos, output.str().find("Last Seen: 23-05-2026")) << "Print stats should print last seen date";
    ASSERT_NE(std::string::npos, output.str().find("Times Seen: 1")) << "Print stats should print times seen";
    ASSERT_NE(std::string::npos, output.str().find("Average Cost: £20")) << "Print stats should print average cost";
}

TEST_F(ArtistStatsManagerTest, print_one_artist_two_concerts)
{
    Concert concert1 = TestData::create_test_concert1();
    Concert concert2 = TestData::create_test_concert2();
    Artist artist = TestData::create_test_artist("Benjamin Steer");

    manager.print_stats({artist}, {concert1, concert2});

    ASSERT_NE(std::string::npos, output.str().find("Name: Benjamin Steer")) << "Print stats should print artist name";
    ASSERT_NE(std::string::npos, output.str().find("First Seen: 23-05-2026")) << "Print stats should print first seen date";
    ASSERT_NE(std::string::npos, output.str().find("Last Seen: 30-05-2026")) << "Print stats should print last seen date";
    ASSERT_NE(std::string::npos, output.str().find("Times Seen: 2")) << "Print stats should print times seen";
    ASSERT_NE(std::string::npos, output.str().find("Average Cost: £22.50")) << "Print stats should print average cost";
}

TEST_F(ArtistStatsManagerTest, print_two_artist_two_concerts)
{
    Artist main_artist = TestData::create_test_artist("Benjamin Steer");
    Artist support_artist = TestData::create_test_artist("Dolder");

    std::unordered_map<int32_t, ArtistRoles> artist_id_role_map1 {};
    artist_id_role_map1.emplace(main_artist.get_id(), ArtistRoles::Headline);
    artist_id_role_map1.emplace(support_artist.get_id(), ArtistRoles::Support);
    Concert concert1 = Concert{artist_id_role_map1, "Village Underground", "London", "23-05-2026", 2000};

    std::unordered_map<int32_t, ArtistRoles> artist_id_role_map2 {};
    artist_id_role_map2.emplace(main_artist.get_id(), ArtistRoles::Headline);
    Concert concert2 = Concert{artist_id_role_map2, "Deaf Institute", "Manchester", "30-05-2026", 2500};

    manager.print_stats({main_artist, support_artist}, {concert1, concert2});

    ASSERT_NE(std::string::npos, output.str().find("Name: Benjamin Steer")) << "Print stats should print artist name";
    ASSERT_NE(std::string::npos, output.str().find("First Seen: 23-05-2026")) << "Print stats should print first seen date";
    ASSERT_NE(std::string::npos, output.str().find("Last Seen: 30-05-2026")) << "Print stats should print last seen date";
    ASSERT_NE(std::string::npos, output.str().find("Times Seen: 2")) << "Print stats should print times seen";
    ASSERT_NE(std::string::npos, output.str().find("Average Cost: £22.50")) << "Print stats should print average cost";

    ASSERT_NE(std::string::npos, output.str().find("Name: Dolder")) << "Print stats should print artist name";
    ASSERT_NE(std::string::npos, output.str().find("First Seen: 23-05-2026")) << "Print stats should print first seen date";
    ASSERT_NE(std::string::npos, output.str().find("Last Seen: 23-05-2026")) << "Print stats should print last seen date";
    ASSERT_NE(std::string::npos, output.str().find("Times Seen: 1")) << "Print stats should print times seen";
    ASSERT_NE(std::string::npos, output.str().find("Average Cost: £20.00")) << "Print stats should print average cost";
}
