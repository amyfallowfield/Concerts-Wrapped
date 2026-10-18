#include <gtest/gtest.h>

#include "ConsoleInput.h"

class ConsoleInputTest : public testing::Test
{
public:

    std::ostringstream output;
    std::ostringstream logs;

    std::streambuf* original_cin;
    std::streambuf* original_cout;
    std::streambuf* original_cerr;

    ConsoleInput input;

    void SetUp() override
    {
        original_cin = std::cin.rdbuf();
        original_cout = std::cout.rdbuf();
        original_cerr = std::cerr.rdbuf();

        std::cout.rdbuf(output.rdbuf());
        std::cerr.rdbuf(logs.rdbuf());

        input = ConsoleInput();
    }

    void TearDown() override
    {
        std::cin.rdbuf(original_cin);
        std::cout.rdbuf(original_cout);
        std::cerr.rdbuf(original_cerr);
    }
};

TEST_F(ConsoleInputTest, get_string_input_reads_line)
{
    std::istringstream input_stream("Hello\n");
    std::cin.rdbuf(input_stream.rdbuf());

    InputResult<std::string> result = input.get_string_input();
    ASSERT_TRUE(result.was_successful);
    ASSERT_EQ("Hello", result.value);
}

TEST_F(ConsoleInputTest, get_numerical_input_parses_integer)
{
    std::istringstream input_stream("12\n");
    std::cin.rdbuf(input_stream.rdbuf());

    InputResult<int> result = input.get_numerical_input<int>();
    ASSERT_TRUE(result.was_successful);
    ASSERT_EQ(12, result.value);
}

TEST_F(ConsoleInputTest, get_numerical_input_rejects_decimal_for_integer)
{
    std::istringstream input_stream("1.2\n");
    std::cin.rdbuf(input_stream.rdbuf());

    InputResult<int> result = input.get_numerical_input<int>();
    ASSERT_FALSE(result.was_successful);
}

TEST_F(ConsoleInputTest, get_numerical_input_rejects_non_numeric_input)
{
    std::istringstream input_stream("Hello\n");
    std::cin.rdbuf(input_stream.rdbuf());

    InputResult<int> result = input.get_numerical_input<int>();
    ASSERT_FALSE(result.was_successful);
}

TEST_F(ConsoleInputTest, get_numerical_input_parses_decimal)
{
    std::istringstream input_stream("1.2\n");
    std::cin.rdbuf(input_stream.rdbuf());

    InputResult<double> result = input.get_numerical_input<double>();
    ASSERT_TRUE(result.was_successful);
    ASSERT_DOUBLE_EQ(1.2, result.value);
}

TEST_F(ConsoleInputTest, get_bool_input_recognizes_yes)
{
    std::istringstream input_stream("yes\n");
    std::cin.rdbuf(input_stream.rdbuf());

    InputResult<bool> result = input.get_bool_input();
    ASSERT_TRUE(result.was_successful);
    ASSERT_TRUE(result.value);
}

TEST_F(ConsoleInputTest, get_bool_input_treats_other_text_as_no)
{
    std::istringstream input_stream("no\n");
    std::cin.rdbuf(input_stream.rdbuf());

    InputResult<bool> result = input.get_bool_input();
    ASSERT_TRUE(result.was_successful);
    ASSERT_FALSE(result.value);
}

TEST_F(ConsoleInputTest, input_failure_is_reported)
{
    std::istringstream input_stream;
    std::cin.rdbuf(input_stream.rdbuf());

    InputResult<std::string> result = input.get_string_input();
    ASSERT_FALSE(result.was_successful);
}
