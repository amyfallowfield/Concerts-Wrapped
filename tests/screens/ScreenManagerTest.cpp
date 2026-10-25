#include <gtest/gtest.h>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "ScreenManager.h"
#include "StorageManager.h"
#include "ConcertRepositoryMock.h"
#include "ConcertsViewMock.h"
#include "InputMock.h"
#include "MenuViewMock.h"
#include "PopUpViewMock.h"
#include "StatsViewMock.h"
#include "TestData.h"

class ScreenManagerTest : public ::testing::Test
{
protected:
    std::unique_ptr<ConcertsViewMock> concerts_view;
    std::unique_ptr<StatsViewMock> stats_view;
    std::unique_ptr<MenuViewMock> menu_view;
    std::unique_ptr<PopUpViewMock> pop_up_view;
    std::unique_ptr<InputMock> input_view;
    std::unique_ptr<UIComponent> ui;
    std::unique_ptr<ScreenManager> manager;
    std::unique_ptr<ConcertRepositoryMock> repo;
    std::unique_ptr<StorageManager> storage;

    std::ostringstream logs;
    std::vector<std::string> input_responses;
    std::size_t next_input = 0;

    std::streambuf* original_cerr;

    void SetUp() override
    {
        Artist::reset();

        original_cerr = std::cerr.rdbuf();

        std::cerr.rdbuf(logs.rdbuf());

        concerts_view = std::make_unique<ConcertsViewMock>();
        stats_view = std::make_unique<StatsViewMock>();
        menu_view = std::make_unique<MenuViewMock>();
        pop_up_view = std::make_unique<PopUpViewMock>();
        input_view = std::make_unique<InputMock>();
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

        manager.reset();
        repo = std::make_unique<ConcertRepositoryMock>(*storage);

        storage->save<Artist>({});
        storage->save<Concert>({});
    }

    void TearDown() override
    {
        std::cerr.rdbuf(original_cerr);

        std::filesystem::remove_all("test");
    }

    void set_input(std::vector<std::string> inputs)
    {
        manager.reset();
        ui.reset();

        input_view = std::make_unique<InputMock>();
        input_responses = std::move(inputs);
        next_input = 0;

        EXPECT_CALL(*input_view, get_string_input())
            .Times(input_responses.size())
            .WillRepeatedly([this]
            {
                if (next_input >= input_responses.size())
                {
                    return InputResult<std::string>{"", false};
                }

                return InputResult<std::string>{input_responses[next_input++], true};
            });
        ui = std::make_unique<UIComponent>(UIComponent{
            *concerts_view,
            *input_view,
            *menu_view,
            *pop_up_view,
            *stats_view
        });
        manager = std::make_unique<ScreenManager>(*repo, *storage, *ui);
    }
};

TEST_F(ScreenManagerTest, run_initially_opens_menu)
{
    set_input({"x"});

    EXPECT_CALL(*menu_view, show_main_menu()).Times(1);
    EXPECT_CALL(*pop_up_view, show_error_message("Invalid input")).Times(1);
    manager->process_current_screen();

    ASSERT_NE(std::string::npos, logs.str().find("menu screen initialised")) << "Menu page should be loaded";
}

TEST_F(ScreenManagerTest, screen_manager_accepts_injected_input)
{
    set_input({"7"});

    manager->process_current_screen();

    ASSERT_NE(std::string::npos, logs.str().find("Screen changed from menu to exit"));
}

TEST_F(ScreenManagerTest, menu_loads_add_concert_page)
{
    set_input({"1", "Benjamin Steer", "headline", "n", "Village Underground", "London", "23-05-2020", "20.00"});

    EXPECT_CALL(*repo, add_concert(testing::_, "village underground", "london", "23-05-2020", 20.0)).Times(1);
    manager->process_current_screen();
    manager->process_current_screen();
}

TEST_F(ScreenManagerTest, invalid_artist_role_cancels_adding_concert)
{
    set_input({"1", "Artist Name", "invalid role"});

    EXPECT_CALL(*repo, add_concert(testing::_, testing::_, testing::_, testing::_, testing::_)).Times(0);
    EXPECT_CALL(*pop_up_view, show_error_message("Artist role invalid")).Times(1);
    manager->process_current_screen();
    manager->process_current_screen();

    ASSERT_NE(std::string::npos, logs.str().find("Screen changed from add concert to menu"));
}

TEST_F(ScreenManagerTest, invalid_concert_date_returns_to_menu)
{
    set_input({"1", "Artist Name", "headline", "n", "Venue", "City", "1"});

    EXPECT_CALL(*repo, add_concert(testing::_, testing::_, testing::_, testing::_, testing::_)).Times(0);
    testing::InSequence sequence;
    EXPECT_CALL(*pop_up_view, show_input_request("artist name"));
    EXPECT_CALL(*pop_up_view, show_input_request("artist role [Headline, Support, Guest]"));
    EXPECT_CALL(*pop_up_view, show_input_request("another artist [Y/N]"));
    EXPECT_CALL(*pop_up_view, show_input_request("venue name"));
    EXPECT_CALL(*pop_up_view, show_input_request("city name"));
    EXPECT_CALL(*pop_up_view, show_input_request("date [DD-MM-YYYY]"));
    EXPECT_CALL(*pop_up_view, show_error_message("Date must use format DD-MM-YYYY"));
    manager->process_current_screen();
    manager->process_current_screen();

    ASSERT_NE(std::string::npos, logs.str().find("Screen changed from add concert to menu"));
}

TEST_F(ScreenManagerTest, menu_loads_view_concerts_page)
{
    set_input({"2"});
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

    manager.reset();
    repo = std::make_unique<ConcertRepositoryMock>(*storage);

    set_input({"3", "1"});

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

    manager.reset();
    repo = std::make_unique<ConcertRepositoryMock>(*storage);

    set_input({"4", "1", "2", "Village Underground Updated"});

    EXPECT_CALL(*repo, update_venue(1, "village underground updated")).Times(1);
    manager->process_current_screen();
    manager->process_current_screen();
}

TEST_F(ScreenManagerTest, invalid_concert_id_returns_to_menu_when_editing)
{
    set_input({"4", "1"});

    EXPECT_CALL(*pop_up_view, show_error_message("ID does not match existing concert")).Times(1);
    EXPECT_CALL(*menu_view, show_edit_menu()).Times(0);
    manager->process_current_screen();
    manager->process_current_screen();

    ASSERT_NE(std::string::npos, logs.str().find("Screen changed from edit concert to menu"));
}

TEST_F(ScreenManagerTest, edit_concert_can_add_new_artist)
{
    Artist main_artist = TestData::create_test_artist("Benjamin Steer");
    Artist support_artist = TestData::create_test_artist("Dolder");
    Concert concert = TestData::create_test_concert1();

    storage->save<Artist>({main_artist, support_artist});
    storage->save<Concert>({concert});

    manager.reset();
    repo = std::make_unique<ConcertRepositoryMock>(*storage);

    set_input({"4", "1", "1", "1", "New Artist", "support"});

    EXPECT_CALL(*repo, add_artist(1, "new artist", ArtistRoles::Support)).Times(1);
    manager->process_current_screen();
    manager->process_current_screen();
}

TEST_F(ScreenManagerTest, menu_loads_concert_stats_page)
{
    set_input({"5"});

    EXPECT_CALL(*stats_view, show_concert_stats(testing::_)).Times(1);
    manager->process_current_screen();
    manager->process_current_screen();
}

TEST_F(ScreenManagerTest, menu_loads_artist_stats_page)
{
    Artist artist = TestData::create_test_artist("Benjamin Steer");
    storage->save<Artist>({artist});
    manager.reset();
    repo = std::make_unique<ConcertRepositoryMock>(*storage);

    set_input({"6"});

    EXPECT_CALL(*stats_view, show_artist_stats(testing::_)).Times(1);
    manager->process_current_screen();
    manager->process_current_screen();
}

TEST_F(ScreenManagerTest, menu_loads_exit_page)
{
    set_input({"7"});

    manager->process_current_screen();

    ASSERT_NE(std::string::npos, logs.str().find("Screen changed from menu to exit")) << "Exit page should be loaded";
}

TEST_F(ScreenManagerTest, invalid_inputs_throw_error)
{
    set_input({"-3", "0", "8", "s"});

    EXPECT_CALL(*pop_up_view, show_error_message("Invalid input")).Times(4);
    manager->process_current_screen();

    manager->process_current_screen();

    manager->process_current_screen();

    manager->process_current_screen();
}
