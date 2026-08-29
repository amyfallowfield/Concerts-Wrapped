#include <gtest/gtest.h>

#include "InputManager.h"

class InputManagerTest : public testing::Test
{
public:

    std::ostringstream output;

    std::streambuf* original_cin;
    std::streambuf* original_cout;

    InputManager manager;

    void SetUp() override
    {
        original_cin = std::cin.rdbuf();
        original_cout = std::cout.rdbuf();

        std::cout.rdbuf(output.rdbuf());

        manager = InputManager();
    }

    void TearDown() override
    {
        std::cin.rdbuf(original_cin);
        std::cout.rdbuf(original_cout);
    }
};

TEST_F(InputManagerTest, select_valid_attribute)
{
    std::istringstream input("1\n");
    std::cin.rdbuf(input.rdbuf());

    ASSERT_EQ(1, manager.select_attribute()) << "Valid input of 1 should return 1";
}

TEST_F(InputManagerTest, select_too_small_invalid_attribute)
{
    std::istringstream input("0\n");
    std::cin.rdbuf(input.rdbuf());

    ASSERT_EQ(-1, manager.select_attribute()) << "Invalid input of 0 should return -1";
}

TEST_F(InputManagerTest, select_too_big_invalid_attribute)
{
    std::istringstream input("6\n");
    std::cin.rdbuf(input.rdbuf());

    ASSERT_EQ(-1, manager.select_attribute()) << "Invalid input of 6 should return -1";
}

TEST_F(InputManagerTest, select_non_number_invalid_attribute)
{
    std::istringstream input("?\n");
    std::cin.rdbuf(input.rdbuf());

    ASSERT_EQ(-1, manager.select_attribute()) << "Invalid input of ? should return -1";
}

TEST_F(InputManagerTest, get_string_input_with_prompt)
{
    std::istringstream input("Hello\n");
    std::cin.rdbuf(input.rdbuf());

    ASSERT_EQ("Hello", manager.get_string_input("Hello123")) << "Valid input should be outputted";
    ASSERT_NE(std::string::npos, output.str().find("Hello123")) << "Exact prompt should be outputted";
}

TEST_F(InputManagerTest, get_numerical_input_with_numeric_input)
{
    std::istringstream input("12\n");
    std::cin.rdbuf(input.rdbuf());

    ASSERT_EQ(12, manager.get_numerical_input("Hello123")) << "Valid input should be outputted";
    ASSERT_NE(std::string::npos, output.str().find("Hello123")) << "Exact prompt should be outputted";
}

TEST_F(InputManagerTest, get_numerical_input_with_decimal_input)
{
    std::istringstream input("1.2\n");
    std::cin.rdbuf(input.rdbuf());

    ASSERT_EQ(-1, manager.get_numerical_input("Hello123")) << "Invalid input should output -1";
    ASSERT_NE(std::string::npos, output.str().find("Hello123")) << "Exact prompt should be outputted";
}

TEST_F(InputManagerTest, get_numerical_input_with_non_numeric_input)
{
    std::istringstream input("Hello\n");
    std::cin.rdbuf(input.rdbuf());

    ASSERT_EQ(-1, manager.get_numerical_input("Hello123")) << "Invalid input should output -1";
    ASSERT_NE(std::string::npos, output.str().find("Hello123")) << "Exact prompt should be outputted";
}

TEST_F(InputManagerTest, get_decimal_input_with_decimal_input)
{
    std::istringstream input("1.2\n");
    std::cin.rdbuf(input.rdbuf());

    ASSERT_EQ(1.2, manager.get_decimal_input("Hello123")) << "Valid input should be outputted";
    ASSERT_NE(std::string::npos, output.str().find("Hello123")) << "Exact prompt should be outputted";
}

TEST_F(InputManagerTest, get_decimal_input_with_integer_input)
{
    std::istringstream input("12\n");
    std::cin.rdbuf(input.rdbuf());

    ASSERT_EQ(12, manager.get_decimal_input("Hello123")) << "Invalid input should output -1.0";
    ASSERT_NE(std::string::npos, output.str().find("Hello123")) << "Exact prompt should be outputted";
}

TEST_F(InputManagerTest, get_decimal_input_with_non_numeric_input)
{
    std::istringstream input("Hello\n");
    std::cin.rdbuf(input.rdbuf());

    ASSERT_EQ(-1.0, manager.get_decimal_input("Hello123")) << "Invalid input should output -1";
    ASSERT_NE(std::string::npos, output.str().find("Hello123")) << "Exact prompt should be outputted";
}

TEST_F(InputManagerTest, get_bool_input_with_true_input)
{
    std::istringstream input("yippee\n");
    std::cin.rdbuf(input.rdbuf());

    ASSERT_EQ(true, manager.get_bool_input("Hello123")) << "True input should output true";
    ASSERT_NE(std::string::npos, output.str().find("Hello123")) << "Exact prompt should be outputted";
}


TEST_F(InputManagerTest, get_bool_input_with_false_input)
{
    std::istringstream input("123\n");
    std::cin.rdbuf(input.rdbuf());

    ASSERT_EQ(false, manager.get_bool_input("Hello123")) << "False input should output false";
    ASSERT_NE(std::string::npos, output.str().find("Hello123")) << "Exact prompt should be outputted";
}
