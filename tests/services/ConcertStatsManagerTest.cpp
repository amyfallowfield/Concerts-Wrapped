#include <gtest/gtest.h>

#include "Concert.h"
#include "ConcertStatsManager.h"
#include "TestData.h"

class ConcertStatsManagerTest : public testing::Test
{
public:

    std::ostringstream output;

    std::streambuf* original_cout;

    void SetUp() override
    {
        original_cout = std::cout.rdbuf();

        std::cout.rdbuf(output.rdbuf());
    }

    void TearDown() override
    {
        std::cout.rdbuf(original_cout);
    }
};

TEST_F(ConcertStatsManagerTest, print_no_concerts)
{
    ConcertStatsManager manager = ConcertStatsManager();

    manager.print_stats({});

    ASSERT_NE(std::string::npos, output.str().find("No statistics to show")) << "Print stats should inform user of no statistics when no concerts";
}

TEST_F(ConcertStatsManagerTest, print_one_concert)
{
    ConcertStatsManager manager = ConcertStatsManager();
    Concert concert = TestData::create_test_concert1();

    manager.print_stats({concert});

    ASSERT_NE(std::string::npos, output.str().find("Total Shows: 1")) << "Print stats should print one concert";
    ASSERT_NE(std::string::npos, output.str().find("Total Cost: £20.00")) << "Print stats should print total cost of one concert";
    ASSERT_NE(std::string::npos, output.str().find("Average Cost: £20.00")) << "Print stats should print average of one concert";
}

TEST_F(ConcertStatsManagerTest, print_two_concerts)
{
    ConcertStatsManager manager = ConcertStatsManager();
    Concert concert1 = TestData::create_test_concert1();
    Concert concert2 = TestData::create_test_concert2();

    manager.print_stats({concert1, concert2});

    ASSERT_NE(std::string::npos, output.str().find("Total Shows: 2")) << "Print stats should print two concerts";
    ASSERT_NE(std::string::npos, output.str().find("Total Cost: £45.00")) << "Print stats should print total cost of two concert";
    ASSERT_NE(std::string::npos, output.str().find("Average Cost: £22.50")) << "Print stats should print average of two concerts";
}
