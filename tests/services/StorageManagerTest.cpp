#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include "Concert.h"
#include "StorageManager.h"
#include "TestData.h"

class StorageManagerTest : public testing::Test
{
public:
    std::unique_ptr<StorageManager> manager;

    std::ostringstream logs;

    std::streambuf* original_cerr;

    void SetUp() override
    {
        original_cerr = std::cerr.rdbuf();

        std::cerr.rdbuf(logs.rdbuf());

        manager = std::make_unique<StorageManager>("test");

        std::filesystem::create_directory("test");
    }

    void TearDown() override
    {
        std::cerr.rdbuf(original_cerr);

        std::filesystem::remove_all("test");
    }
};

TEST_F(StorageManagerTest, save_empty_items)
{
    ASSERT_EQ(true, manager->save(std::vector<Concert>{})) << "Save should return true when successful";

    std::ifstream file("test/concert_data.json");

    ASSERT_EQ(true, file.is_open()) << "concert_data.json should be open after saving";
    
    json data;
    file >> data;

    ASSERT_EQ(true, data.is_array()) << "concert_data.json should be saved as a json array";
    ASSERT_EQ(true, data.empty()) << "concert_data.json should be empty after saving with no concerts";
}

TEST_F(StorageManagerTest, save_one_item)
{
    Concert concert = TestData::create_test_concert1();

    ASSERT_EQ(true, manager->save(std::vector<Concert>{concert})) << "Save should return true when successful";

    std::ifstream file("test/concert_data.json");

    ASSERT_EQ(true, file.is_open()) << "concert_data.json should be open after saving";
    
    json data;
    file >> data;

    ASSERT_EQ(true, data.is_array()) << "concert_data.json should be saved as a json array";
    ASSERT_EQ(concert.to_json(), data[0]) << "concert_data.json should contain data for added concert";
    ASSERT_EQ(1, data.size()) << "concert_data.json should only contain data for one concert";
}

TEST_F(StorageManagerTest, save_multiple_items)
{
    Concert concert1 = TestData::create_test_concert1();
    Concert concert2 = TestData::create_test_concert2();

    ASSERT_EQ(true, manager->save(std::vector<Concert>{concert1, concert2})) << "Save should return true when successful";

    std::ifstream file("test/concert_data.json");

    ASSERT_EQ(true, file.is_open()) << "concert_data.json should be open after saving";
    
    json data;
    file >> data;

    ASSERT_EQ(true, data.is_array()) << "concert_data.json should be saved as a json array";
    ASSERT_EQ(concert1.to_json(), data[0]) << "concert_data.json should contain data for first concert";
    ASSERT_EQ(concert2.to_json(), data[1]) << "concert_data.json should contain data for second concert";
    ASSERT_EQ(2, data.size()) << "concert_data.json should only contain data for two concerts";
}

TEST_F(StorageManagerTest, save_unopenable_file)
{
    std::filesystem::create_directory("test/concert_data.json");

    ASSERT_THROW(manager->save(std::vector<Concert>{}), std::runtime_error) << "Saving unopenable file should throw runtime error";
}

TEST_F(StorageManagerTest, load_empty_items)
{
    manager->save(std::vector<Concert>{});

    ASSERT_EQ(std::vector<Concert>{}, manager->load<Concert>()) << "Load should return empty when no existing concerts";
}

TEST_F(StorageManagerTest, load_one_item)
{
    Concert concert = TestData::create_test_concert1();
    manager->save(std::vector<Concert>{concert});

    ASSERT_EQ(concert, manager->load<Concert>()[0]) << "Load should return concert when one existing concert";
    ASSERT_EQ(1, manager->load<Concert>().size()) << "Load should load one concert";
}

TEST_F(StorageManagerTest, load_multiple_items)
{
    Concert concert1 = TestData::create_test_concert1();
    Concert concert2 = TestData::create_test_concert2();
    manager->save(std::vector<Concert>{concert1, concert2});

    ASSERT_EQ(concert1, manager->load<Concert>()[0]) << "Load should return data for first concert";
    ASSERT_EQ(concert2, manager->load<Concert>()[1]) << "Load should return data for second concert";
    ASSERT_EQ(2, manager->load<Concert>().size()) << "Load should load two concerts";
}

TEST_F(StorageManagerTest, load_unopenable_file)
{
    std::filesystem::create_directory("test/concert_data.json");

    ASSERT_THROW(manager->load<Concert>(), std::runtime_error) << "Loading unopenable file should throw runtime error";
}
