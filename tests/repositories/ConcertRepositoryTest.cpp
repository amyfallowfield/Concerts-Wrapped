#include <filesystem>
#include <gtest/gtest.h>
#include <sstream>

#include "Artist.h"
#include "Concert.h"
#include "ConcertArtist.h"
#include "ConcertRepository.h"
#include "JSONStorageManager.h"

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
        Concert::reset();
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
    JSONStorageManager storage{"test"};
    storage.save_artists({});
    storage.save_concerts({});

    ConcertRepository repo = ConcertRepository(storage);

    ASSERT_EQ(0, repo.get_artists().size()) << "Artist list should be empty when using empty file";
    ASSERT_EQ(0, repo.get_concerts().size())
        << "Concert list should be empty when using empty file";
}

TEST_F(ConcertRepositoryTest, new_repo_with_data_files_restores_data)
{
    JSONStorageManager storage{"test"};

    Concert concert = TestData::create_test_concert1();
    Artist main_artist = TestData::create_test_artist("Benjamin Steer");
    Artist support_artist = TestData::create_test_artist("Dolder");
    ConcertArtist concert_artist1 = TestData::create_artist_concert(1, 1, ArtistRoles::Headline);
    ConcertArtist concert_artist2 = TestData::create_artist_concert(1, 2, ArtistRoles::Support);

    storage.save_artists({main_artist, support_artist});
    storage.save_concerts({concert});
    storage.save_concert_artists({concert_artist1, concert_artist2});

    ConcertRepository repo = ConcertRepository(storage);

    ASSERT_EQ(2, repo.get_artists().size())
        << "Artist list should contain 1 artist when using empty file";
    ASSERT_EQ(1, repo.get_concerts().size())
        << "Concert list should contain 1 concert when using empty file";
    ASSERT_EQ(2, repo.get_concert_artists().size());
    ASSERT_EQ(1, repo.get_concert_artists()[0].get_concert_id());
    ASSERT_EQ(1, repo.get_concert_artists()[0].get_artist_id());
    ASSERT_EQ(ArtistRoles::Headline, repo.get_concert_artists()[0].get_role());
    ASSERT_EQ(ArtistRoles::Support, repo.get_concert_artists()[1].get_role());
}

TEST_F(ConcertRepositoryTest, add_first_concert)
{
    JSONStorageManager storage{"test"};
    storage.save_artists({});
    storage.save_concerts({});

    ConcertRepository repo = ConcertRepository(storage);
    std::map<std::string, ArtistRoles> artists{{"Benjamin Steer", ArtistRoles::Headline},
                                               {"Dolder", ArtistRoles::Support}};

    repo.add_concert(artists, "Village Underground", "London", "23-05-2020", 20.00);

    ASSERT_EQ(2, repo.get_artists().size()) << "Artist list should contain 2 artists when adding "
                                               "concert with 2 performers from empty file";
    ASSERT_EQ(1, repo.get_concerts().size())
        << "Concert list should contain 1 concert adding concert from empty file";
    ASSERT_EQ(2, repo.get_concert_artists().size());
}

TEST_F(ConcertRepositoryTest, add_additional_concert)
{
    JSONStorageManager storage{"test"};
    storage.save_artists({});
    storage.save_concerts({});

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
    ASSERT_EQ(4, repo.get_concert_artists().size());
}

TEST_F(ConcertRepositoryTest, add_new_artist_to_existing_concert)
{
    JSONStorageManager storage{"test"};
    Artist main_artist = TestData::create_test_artist("Benjamin Steer");
    Artist support_artist = TestData::create_test_artist("Dolder");
    Concert concert = TestData::create_test_concert1();
    ConcertArtist concert_artist1 = TestData::create_artist_concert(1, 1, ArtistRoles::Headline);
    ConcertArtist concert_artist2 = TestData::create_artist_concert(1, 2, ArtistRoles::Support);
    storage.save_artists({main_artist, support_artist});
    storage.save_concerts({concert});
    storage.save_concert_artists({concert_artist1, concert_artist2});

    ConcertRepository repo = ConcertRepository(storage);
    repo.add_artist(1, "new artist", ArtistRoles::Guest);

    ASSERT_EQ(3, repo.get_artists().size());
    ASSERT_EQ("new artist", repo.get_artists().back().get_name());
    ASSERT_EQ(3, repo.get_concert_artists().size());
}

TEST_F(ConcertRepositoryTest, remove_one_of_two_concerts)
{
    JSONStorageManager storage{"test"};

    Concert concert1 = TestData::create_test_concert1();
    Concert concert2 = TestData::create_test_concert2();
    Artist main_artist = TestData::create_test_artist("Benjamin Steer");
    Artist support_artist = TestData::create_test_artist("Dolder");
    ConcertArtist concert1_artist1 = TestData::create_artist_concert(1, 1, ArtistRoles::Headline);
    ConcertArtist concert1_artist2 = TestData::create_artist_concert(1, 2, ArtistRoles::Support);
    ConcertArtist concert2_artist1 = TestData::create_artist_concert(2, 1, ArtistRoles::Headline);
    ConcertArtist concert2_artist2 = TestData::create_artist_concert(2, 2, ArtistRoles::Support);

    storage.save_artists({main_artist, support_artist});
    storage.save_concerts({concert1, concert2});
    storage.save_concert_artists(
        {concert1_artist1, concert1_artist2, concert2_artist1, concert2_artist2});

    ConcertRepository repo = ConcertRepository(storage);

    repo.remove_concert(1);

    ASSERT_EQ(2, repo.get_artists().size()) << "Artist list should contain 2 artist when one of "
                                               "two concerts with same artist is removed";
    ASSERT_EQ(1, repo.get_concerts().size())
        << "Concert list should contain 1 concert when one of two concerts is removed";
    ASSERT_EQ(2, repo.get_concert_artists().size());

    repo.remove_concert(2);

    ASSERT_EQ(0, repo.get_artists().size())
        << "Artist list should contain 0 artists when all concerts are removed";
    ASSERT_EQ(0, repo.get_concerts().size())
        << "Concert list should contain 0 concerts when all concerts are removed";
    ASSERT_EQ(0, repo.get_concert_artists().size());
}

TEST_F(ConcertRepositoryTest, remove_only_concert)
{
    JSONStorageManager storage{"test"};

    Concert concert = TestData::create_test_concert1();
    Artist main_artist = TestData::create_test_artist("Benjamin Steer");
    Artist support_artist = TestData::create_test_artist("Dolder");
    ConcertArtist concert_artist1 = TestData::create_artist_concert(1, 1, ArtistRoles::Headline);
    ConcertArtist concert_artist2 = TestData::create_artist_concert(1, 2, ArtistRoles::Support);

    storage.save_artists({main_artist, support_artist});
    storage.save_concerts({concert});
    storage.save_concert_artists({concert_artist1, concert_artist2});

    ConcertRepository repo = ConcertRepository(storage);

    repo.remove_concert(1);

    ASSERT_EQ(0, repo.get_artists().size())
        << "Artist list should contain 0 artists when only concert is removed";
    ASSERT_EQ(0, repo.get_concerts().size())
        << "Concert list should contain 0 concerts when only concert is removed";
    ASSERT_EQ(0, repo.get_concert_artists().size());
}
