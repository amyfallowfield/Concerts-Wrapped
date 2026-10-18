#include <gtest/gtest.h>
#include <memory>
#include <sstream>

#include "ScreenManager.h"
#include "StorageManager.h"
#include "ConsoleMenuView.h"
#include "ConsolePopUpView.h"
#include "ConsoleInput.h"

#include "ConcertRepositoryMock.h"
#include "ConcertsViewMock.h"
#include "StatsViewMock.h"
#include "TestData.h"

class ScreenManagerTest : public ::testing::Test
{
protected:
    std::unique_ptr<ConcertsViewMock> concerts_view;
    std::unique_ptr<StatsViewMock> stats_view;
    std::unique_ptr<ConsoleMenuView> menu_view;
    std::unique_ptr<ConsolePopUpView> pop_up_view;
    std::unique_ptr<ConsoleInput> input_view;
    std::unique_ptr<UIComponent> ui;
    std::unique_ptr<ScreenManager> manager;
    std::unique_ptr<ConcertRepositoryMock> repo;
    std::unique_ptr<StorageManager> storage;

    std::ostringstream output;
    std::ostringstream logs;

    std::streambuf* original_cin;
    std::streambuf* original_cout;
    std::streambuf* original_cerr;

    void SetUp() override
    {
        Artist::reset();

        original_cin = std::cin.rdbuf();
        original_cout = std::cout.rdbuf();
        original_cerr = std::cerr.rdbuf();

        std::cout.rdbuf(output.rdbuf());
        std::cerr.rdbuf(logs.rdbuf());

        concerts_view = std::make_unique<ConcertsViewMock>();
        stats_view = std::make_unique<StatsViewMock>();
        menu_view = std::make_unique<ConsoleMenuView>();
        pop_up_view = std::make_unique<ConsolePopUpView>();
        input_view = std::make_unique<ConsoleInput>();
        ui = std::make_unique<UIComponent>(UIComponent{
            *concerts_view,
            *input_view,
            *menu_view,
            *pop_up_view,
            *stats_view
        });
    
        storage = std::make_unique<StorageManager>("test");
        storage->save<Artist>({});
        storage->save<Concert>({});

        repo = std::make_unique<ConcertRepositoryMock>(*storage);
        manager = std::make_unique<ScreenManager>(*repo, *storage, *ui);

        storage->save<Artist>({});
        storage->save<Concert>({});
    }

    void TearDown() override
    {
        std::cin.rdbuf(original_cin);
        std::cout.rdbuf(original_cout);
        std::cerr.rdbuf(original_cerr);

        std::filesystem::remove_all("test");
    }
};

TEST_F(ScreenManagerTest, run_initially_opens_menu)
{
    std::istringstream input("x\n");
    std::cin.rdbuf(input.rdbuf());

    manager->process_current_screen();

    ASSERT_NE(std::string::npos, logs.str().find("menu screen initialised")) << "Menu page should be loaded";
}

TEST_F(ScreenManagerTest, menu_loads_add_concert_page)
{
    std::istringstream input("1\nBenjamin Steer\nheadline\nn\nVillage Underground\nLondon\n23-05-2020\n20.00\n");
    std::cin.rdbuf(input.rdbuf());

    EXPECT_CALL(*repo, add_concert(testing::_, "village underground", "london", "23-05-2020", 20.0)).Times(1);
    manager->process_current_screen();
    manager->process_current_screen();
}

TEST_F(ScreenManagerTest, invalid_artist_role_cancels_adding_concert)
{
    std::istringstream input("1\nArtist Name\ninvalid role\nVillage Underground\nLondon\n23-05-2020\n20.00\n");
    std::cin.rdbuf(input.rdbuf());

    EXPECT_CALL(*repo, add_concert(testing::_, testing::_, testing::_, testing::_, testing::_)).Times(0);
    manager->process_current_screen();
    manager->process_current_screen();

    ASSERT_NE(std::string::npos, output.str().find("Artist role invalid"));
    ASSERT_NE(std::string::npos, logs.str().find("Screen changed from add concert to menu"));
}

TEST_F(ScreenManagerTest, invalid_concert_date_returns_to_menu)
{
    std::istringstream input("1\nArtist Name\nheadline\nn\nVenue\nCity\n1\n");
    std::cin.rdbuf(input.rdbuf());

    EXPECT_CALL(*repo, add_concert(testing::_, testing::_, testing::_, testing::_, testing::_)).Times(0);
    manager->process_current_screen();
    manager->process_current_screen();

    ASSERT_NE(std::string::npos, output.str().find("Date must use format DD-MM-YYYY"));
    ASSERT_NE(std::string::npos, logs.str().find("Screen changed from add concert to menu"));
    ASSERT_EQ(std::string::npos, output.str().find("Enter artist name:", output.str().find("Date must use format")));
}

TEST_F(ScreenManagerTest, menu_loads_view_concerts_page)
{
    std::istringstream input("2\n");
    std::cin.rdbuf(input.rdbuf());
    EXPECT_CALL(*concerts_view, show(testing::_, testing::_)).Times(1);
    manager->process_current_screen();
    manager->process_current_screen();
}

TEST_F(ScreenManagerTest, menu_loads_delete_concert_page)
{
    Artist main_artist = TestData::create_test_artist("Benjamin Steer");
    Artist support_artist = TestData::create_test_artist("Dolder");
    Concert concert = TestData::create_test_concert1();

    storage->save<Artist>({main_artist, support_artist});
    storage->save<Concert>({concert});

    repo = std::make_unique<ConcertRepositoryMock>(*storage);
    manager = std::make_unique<ScreenManager>(*repo, *storage, *ui);

    std::istringstream input("3\n1\n");
    std::cin.rdbuf(input.rdbuf());

    EXPECT_CALL(*repo, remove_concert(1)).Times(1);
    manager->process_current_screen();
    manager->process_current_screen();
}

TEST_F(ScreenManagerTest, menu_loads_edit_concert_page)
{
    Artist main_artist = TestData::create_test_artist("Benjamin Steer");
    Artist support_artist = TestData::create_test_artist("Dolder");
    Concert concert = TestData::create_test_concert1();

    storage->save<Artist>({main_artist, support_artist});
    storage->save<Concert>({concert});

    repo = std::make_unique<ConcertRepositoryMock>(*storage);
    manager = std::make_unique<ScreenManager>(*repo, *storage, *ui);

    std::istringstream input("4\n1\n2\nVillage Underground Updated\n");
    std::cin.rdbuf(input.rdbuf());

    EXPECT_CALL(*repo, update_venue(1, "village underground updated")).Times(1);
    manager->process_current_screen();
    manager->process_current_screen();
}

TEST_F(ScreenManagerTest, invalid_concert_id_returns_to_menu_when_editing)
{
    std::istringstream input("4\n1\n");
    std::cin.rdbuf(input.rdbuf());

    manager->process_current_screen();
    manager->process_current_screen();

    ASSERT_NE(std::string::npos, output.str().find("ID does not match existing concert"));
    ASSERT_NE(std::string::npos, logs.str().find("Screen changed from edit concert to menu"));
    ASSERT_EQ(std::string::npos, output.str().find("1. Artists"));
}

TEST_F(ScreenManagerTest, edit_concert_can_add_new_artist)
{
    Artist main_artist = TestData::create_test_artist("Benjamin Steer");
    Artist support_artist = TestData::create_test_artist("Dolder");
    Concert concert = TestData::create_test_concert1();

    storage->save<Artist>({main_artist, support_artist});
    storage->save<Concert>({concert});

    repo = std::make_unique<ConcertRepositoryMock>(*storage);
    manager = std::make_unique<ScreenManager>(*repo, *storage, *ui);

    std::istringstream input("4\n1\n1\n1\nNew Artist\nsupport\n");
    std::cin.rdbuf(input.rdbuf());

    EXPECT_CALL(*repo, add_artist(1, "new artist", ArtistRoles::Support)).Times(1);
    manager->process_current_screen();
    manager->process_current_screen();
}

TEST_F(ScreenManagerTest, menu_loads_concert_stats_page)
{
    std::istringstream input("5\n");
    std::cin.rdbuf(input.rdbuf());

    EXPECT_CALL(*stats_view, show_concert_stats(testing::_)).Times(1);
    manager->process_current_screen();
    manager->process_current_screen();
}

TEST_F(ScreenManagerTest, menu_loads_artist_stats_page)
{
    Artist artist = TestData::create_test_artist("Benjamin Steer");
    storage->save<Artist>({artist});
    repo = std::make_unique<ConcertRepositoryMock>(*storage);
    manager = std::make_unique<ScreenManager>(*repo, *storage, *ui);

    std::istringstream input("6\n");
    std::cin.rdbuf(input.rdbuf());

    EXPECT_CALL(*stats_view, show_artist_stats(testing::_)).Times(1);
    manager->process_current_screen();
    manager->process_current_screen();
}

TEST_F(ScreenManagerTest, menu_loads_exit_page)
{
    std::istringstream input("7\n");
    std::cin.rdbuf(input.rdbuf());

    manager->process_current_screen();

    ASSERT_NE(std::string::npos, logs.str().find("Screen changed from menu to exit")) << "Exit page should be loaded";
}

TEST_F(ScreenManagerTest, invalid_inputs_throw_error)
{
    std::istringstream input("-3\n0\n8\ns\n");
    std::cin.rdbuf(input.rdbuf());

    manager->process_current_screen();
    ASSERT_NE(std::string::npos, output.str().find("Invalid input")) << "Negative inputs should throw error";

    output.str("");
    output.clear();

    manager->process_current_screen();
    ASSERT_NE(std::string::npos, output.str().find("Invalid input")) << "Zero input should throw error";

    output.str("");
    output.clear();

    manager->process_current_screen();
    ASSERT_NE(std::string::npos, output.str().find("Invalid input")) << "Too big inputs should throw error";

    output.str("");
    output.clear();

    manager->process_current_screen();
    ASSERT_NE(std::string::npos, output.str().find("Invalid input")) << "String inputs should throw error";
}
