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

    std::streambuf* original_cin;
    std::streambuf* original_cout;
    std::streambuf* original_cerr;

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
    ASSERT_EQ(0, repo.get_concerts().size()) << "Concert list should be empty when using empty file";
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

    ASSERT_EQ(2, repo.get_artists().size()) << "Artist list should contain 1 artist when using empty file";
    ASSERT_EQ(1, repo.get_concerts().size()) << "Concert list should contain 1 concert when using empty file";
}

TEST_F(ConcertRepositoryTest, add_first_concert)
{
    StorageManager storage = StorageManager("test");
    storage.save<Artist>({});
    storage.save<Concert>({});

    ConcertRepository repo = ConcertRepository(storage);

    std::istringstream input("Benjamin Steer\nheadline\ny\nDolder\nsupport\nn\nVillage Underground\nLondon\n23-05-2026\n20.00\n");
    std::cin.rdbuf(input.rdbuf());

    repo.add();

    ASSERT_EQ(2, repo.get_artists().size()) << "Artist list should contain 2 artists when adding concert with 2 performers from empty file";
    ASSERT_EQ(1, repo.get_concerts().size()) << "Concert list should contain 1 concert adding concert from empty file";
}

TEST_F(ConcertRepositoryTest, add_additional_concert)
{
    StorageManager storage = StorageManager("test");
    storage.save<Artist>({});
    storage.save<Concert>({});

    ConcertRepository repo = ConcertRepository(storage);

    std::istringstream input1("Benjamin Steer\nheadline\ny\nDolder\nsupport\nn\nVillage Underground\nLondon\n23-05-2026\n20.00\n");
    std::cin.rdbuf(input1.rdbuf());

    repo.add();

    std::istringstream input2("Arthur Hill\nheadline\ny\nBenjamin Steer\nsupport\nn\nO2 Academy Brixton\nLondon\n27-11-2025\n30.00\n");
    std::cin.rdbuf(input2.rdbuf());

    repo.add();

    ASSERT_EQ(3, repo.get_artists().size()) << "Artist list should contain 3 artists when adding 2 concerts with 1 common artist";
    ASSERT_EQ(2, repo.get_concerts().size()) << "Concert list should contain 2 concerts when adding 2 concerts";
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

    std::istringstream input1("1\n");
    std::cin.rdbuf(input1.rdbuf());

    repo.remove();

    ASSERT_EQ(2, repo.get_artists().size()) << "Artist list should contain 2 artist when one of two concerts with same artist is removed";
    ASSERT_EQ(1, repo.get_concerts().size()) << "Concert list should contain 1 concert when one of two concerts is removed";

    std::istringstream input2("2\n");
    std::cin.rdbuf(input2.rdbuf());

    repo.remove();

    ASSERT_EQ(0, repo.get_artists().size()) << "Artist list should contain 0 artists when all concerts are removed";
    ASSERT_EQ(0, repo.get_concerts().size()) << "Concert list should contain 0 concerts when all concerts are removed";
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

    std::istringstream input("1\n");
    std::cin.rdbuf(input.rdbuf());

    repo.remove();

    ASSERT_EQ(0, repo.get_artists().size()) << "Artist list should contain 0 artists when only concert is removed";
    ASSERT_EQ(0, repo.get_concerts().size()) << "Concert list should contain 0 concerts when only concert is removed";
}
