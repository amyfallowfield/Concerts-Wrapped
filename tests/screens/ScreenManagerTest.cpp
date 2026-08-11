#include <gtest/gtest.h>
#include <memory>
#include <sstream>

#include "ScreenManager.h"

class ScreenManagerTest : public ::testing::Test
{
protected:
    std::unique_ptr<ScreenManager> manager;

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
        manager = std::make_unique<ScreenManager>();
    }

    void TearDown() override
    {
        std::cin.rdbuf(original_cin);
        std::cout.rdbuf(original_cout);
        std::cerr.rdbuf(original_cerr);
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

    manager->process_current_screen();

    ASSERT_NE(std::string::npos, logs.str().find("Screen changed from menu to add concert")) << "Add concert page should be loaded";
}

TEST_F(ScreenManagerTest, menu_loads_view_concerts_page)
{
    std::istringstream input("2\n");
    std::cin.rdbuf(input.rdbuf());

    manager->process_current_screen();

    ASSERT_NE(std::string::npos, logs.str().find("Screen changed from menu to view concerts")) << "View concerts page should be loaded";
}

TEST_F(ScreenManagerTest, menu_loads_delete_concert_page)
{
    std::istringstream input("3\n");
    std::cin.rdbuf(input.rdbuf());

    manager->process_current_screen();

    ASSERT_NE(std::string::npos, logs.str().find("Screen changed from menu to delete concert")) << "Delete concert page should be loaded";
}

TEST_F(ScreenManagerTest, menu_loads_edit_concert_page)
{
    std::istringstream input("4\n");
    std::cin.rdbuf(input.rdbuf());

    manager->process_current_screen();

    ASSERT_NE(std::string::npos, logs.str().find("Screen changed from menu to edit concert")) << "Edit concert page should be loaded";
}
TEST_F(ScreenManagerTest, menu_loads_concert_stats_page)
{
    std::istringstream input("5\n");
    std::cin.rdbuf(input.rdbuf());

    manager->process_current_screen();

    ASSERT_NE(std::string::npos, logs.str().find("Screen changed from menu to concert stats")) << "Concert stats page should be loaded";
}

TEST_F(ScreenManagerTest, menu_loads_artist_stats_page)
{
    std::istringstream input("6\n");
    std::cin.rdbuf(input.rdbuf());

    manager->process_current_screen();

    ASSERT_NE(std::string::npos, logs.str().find("Screen changed from menu to artist stats")) << "Artist stats page should be loaded";
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
