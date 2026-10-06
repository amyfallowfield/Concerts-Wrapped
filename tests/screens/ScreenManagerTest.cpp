#include <gtest/gtest.h>
#include <memory>
#include <sstream>

#include "ScreenManager.h"
#include "StorageManager.h"
#include "ConsoleMenuView.h"
#include "ConsolePopUpView.h"

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
        original_cin = std::cin.rdbuf();
        original_cout = std::cout.rdbuf();
        original_cerr = std::cerr.rdbuf();

        std::cout.rdbuf(output.rdbuf());
        std::cerr.rdbuf(logs.rdbuf());

        concerts_view = std::make_unique<ConcertsViewMock>();
        stats_view = std::make_unique<StatsViewMock>();
        menu_view = std::make_unique<ConsoleMenuView>();
        pop_up_view = std::make_unique<ConsolePopUpView>();
        ui = std::make_unique<UIComponent>(UIComponent{
            *concerts_view,
            *stats_view,
            *menu_view,
            *pop_up_view
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
    std::istringstream input("1\n");
    std::cin.rdbuf(input.rdbuf());

    EXPECT_CALL(*repo, add()).Times(1);
    manager->process_current_screen();
    manager->process_current_screen();
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
    std::istringstream input("3\n");
    std::cin.rdbuf(input.rdbuf());

    EXPECT_CALL(*repo, remove()).Times(1);
    manager->process_current_screen();
    manager->process_current_screen();
}

TEST_F(ScreenManagerTest, menu_loads_edit_concert_page)
{
    std::istringstream input("4\n");
    std::cin.rdbuf(input.rdbuf());

    EXPECT_CALL(*repo, edit()).Times(1);
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
    Artist::reset();
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
