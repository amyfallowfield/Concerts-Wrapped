#include <filesystem>
#include <gtest/gtest.h>
#include <sstream>

#include "Artist.h"
#include "Concert.h"
#include "ConcertRepository.h"
#include "StorageManager.h"

#include "TestData.h"

class ConcertRepositoryTest : public testing::Test
{
  protected:
    std::filesystem::path test_directory = std::filesystem::path("test");

    std::istringstream input;
    std::ostringstream output;
    std::ostringstream logs;

    std::streambuf *original_cin;
    std::streambuf *original_cout;
    std::streambuf *original_cerr;

    void SetUp() override
    {
        original_cin = std::cin.rdbuf();
        original_cout = std::cout.rdbuf();
        original_cerr = std::cerr.rdbuf();

        std::cout.rdbuf(output.rdbuf());
        std::cerr.rdbuf(logs.rdbuf());

        std::filesystem::remove_all(test_directory);
        std::filesystem::create_directory(test_directory);

        Artist::reset();
    }

    void TearDown() override
    {
        std::filesystem::remove_all(test_directory);

        std::cin.rdbuf(original_cin);
        std::cout.rdbuf(original_cout);
        std::cerr.rdbuf(original_cerr);
    }
};

TEST_F(ConcertRepositoryTest, new_repo_with_no_data_files_empty)
{
    StorageManager storage = StorageManager("test");
    storage.save<Artist>({});
    storage.save<Concert>({});

    ConcertRepository repo = ConcertRepository(storage);

    ASSERT_EQ(0, repo.get_artists().size()) << "Artist list should be empty when using empty file";
    ASSERT_EQ(0, repo.get_concerts().size())
        << "Concert list should be empty when using empty file";
}

TEST_F(ConcertRepositoryTest, new_repo_with_data_files_restores_data)
{
    StorageManager storage = StorageManager("test");

    Concert concert = TestData::create_test_concert1();
    Artist main_artist = TestData::create_test_artist("Benjamin Steer");
    Artist support_artist = TestData::create_test_artist("Dolder");

    storage.save<Artist>({main_artist, support_artist});
    storage.save<Concert>({concert});

    ConcertRepository repo = ConcertRepository(storage);

    ASSERT_EQ(2, repo.get_artists().size())
        << "Artist list should contain 1 artist when using empty file";
    ASSERT_EQ(1, repo.get_concerts().size())
        << "Concert list should contain 1 concert when using empty file";
}

TEST_F(ConcertRepositoryTest, add_first_concert)
{
    StorageManager storage = StorageManager("test");
    storage.save<Artist>({});
    storage.save<Concert>({});

    ConcertRepository repo = ConcertRepository(storage);
    std::map<std::string, ArtistRoles> artists{{"Benjamin Steer", ArtistRoles::Headline},
                                               {"Dolder", ArtistRoles::Support}};

    repo.add_concert(artists, "Village Underground", "London", "23-05-2020", 20.00);

    ASSERT_EQ(2, repo.get_artists().size()) << "Artist list should contain 2 artists when adding "
                                               "concert with 2 performers from empty file";
    ASSERT_EQ(1, repo.get_concerts().size())
        << "Concert list should contain 1 concert adding concert from empty file";
}

TEST_F(ConcertRepositoryTest, add_additional_concert)
{
    StorageManager storage = StorageManager("test");
    storage.save<Artist>({});
    storage.save<Concert>({});

    ConcertRepository repo = ConcertRepository(storage);
    std::map<std::string, ArtistRoles> first_concert_artists{
        {"Benjamin Steer", ArtistRoles::Headline}, {"Dolder", ArtistRoles::Support}};
    std::map<std::string, ArtistRoles> second_concert_artists{
        {"Arthur Hill", ArtistRoles::Headline}, {"Benjamin Steer", ArtistRoles::Support}};

    repo.add_concert(first_concert_artists, "Village Underground", "London", "23-05-2020", 20.00);
    repo.add_concert(second_concert_artists, "O2 Academy Brixton", "London", "27-11-2020", 30.00);

    ASSERT_EQ(3, repo.get_artists().size())
        << "Artist list should contain 3 artists when adding 2 concerts with 1 common artist";
    ASSERT_EQ(2, repo.get_concerts().size())
        << "Concert list should contain 2 concerts when adding 2 concerts";
}

TEST_F(ConcertRepositoryTest, add_new_artist_to_existing_concert)
{
    StorageManager storage = StorageManager("test");
    Artist main_artist = TestData::create_test_artist("Benjamin Steer");
    Artist support_artist = TestData::create_test_artist("Dolder");
    Concert concert = TestData::create_test_concert1();
    storage.save<Artist>({main_artist, support_artist});
    storage.save<Concert>({concert});

    ConcertRepository repo = ConcertRepository(storage);
    repo.add_artist(1, "new artist", ArtistRoles::Guest);

    ASSERT_EQ(3, repo.get_artists().size());
    ASSERT_EQ("new artist", repo.get_artists().back().get_name());
    ASSERT_EQ(3, repo.get_concerts().front().get_artists().size());
}

TEST_F(ConcertRepositoryTest, remove_one_of_two_concerts)
{
    StorageManager storage = StorageManager("test");

    Concert concert1 = TestData::create_test_concert1();
    Concert concert2 = TestData::create_test_concert2();
    Artist main_artist = TestData::create_test_artist("Benjamin Steer");
    Artist support_artist = TestData::create_test_artist("Dolder");
    storage.save<Artist>({main_artist, support_artist});
    storage.save<Concert>({concert1, concert2});

    ConcertRepository repo = ConcertRepository(storage);

    repo.remove_concert(1);

    ASSERT_EQ(2, repo.get_artists().size()) << "Artist list should contain 2 artist when one of "
                                               "two concerts with same artist is removed";
    ASSERT_EQ(1, repo.get_concerts().size())
        << "Concert list should contain 1 concert when one of two concerts is removed";

    repo.remove_concert(2);

    ASSERT_EQ(0, repo.get_artists().size())
        << "Artist list should contain 0 artists when all concerts are removed";
    ASSERT_EQ(0, repo.get_concerts().size())
        << "Concert list should contain 0 concerts when all concerts are removed";
}

TEST_F(ConcertRepositoryTest, remove_only_concert)
{
    StorageManager storage = StorageManager("test");

    Concert concert = TestData::create_test_concert1();
    Artist main_artist = TestData::create_test_artist("Benjamin Steer");
    Artist support_artist = TestData::create_test_artist("Dolder");

    storage.save<Artist>({main_artist, support_artist});
    storage.save<Concert>({concert});

    ConcertRepository repo = ConcertRepository(storage);

    repo.remove_concert(1);

    ASSERT_EQ(0, repo.get_artists().size())
        << "Artist list should contain 0 artists when only concert is removed";
    ASSERT_EQ(0, repo.get_concerts().size())
        << "Concert list should contain 0 concerts when only concert is removed";
}
