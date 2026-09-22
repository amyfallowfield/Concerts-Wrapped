#include <gtest/gtest.h>

#include "ValidationManager.h"

#include "TestData.h"

class ValidationManagerTest : public testing::Test
{
public:
    ValidationManager manager;

    void SetUp() override
    {
        manager = ValidationManager();
    }
};

TEST_F(ValidationManagerTest, validate_valid_date)
{
    std::string input = "01-01-2000";
    ValidationResult result = manager.validate_date(input);

    ASSERT_EQ("01-01-2000", result.value) << "Validated date should be 01-01-2000";
    ASSERT_EQ(true, result.is_valid) << "Valid date should return true";
    ASSERT_TRUE(result.error_message.empty()) << "Valid date should have empty error message";
}

TEST_F(ValidationManagerTest, validate_valid_date_with_whitespace)
{
    std::string input = "  01-01-2000  ";
    ValidationResult result = manager.validate_date(input);

    ASSERT_EQ("01-01-2000", result.value) << "Validated date should be 01-01-2000";
    ASSERT_EQ(true, result.is_valid) << "Valid date should return true";
    ASSERT_TRUE(result.error_message.empty()) << "Valid date should have empty error message";
}

TEST_F(ValidationManagerTest, validate_future_date)
{
    std::string input = "01-01-3000";
    ValidationResult result = manager.validate_date(input);

    ASSERT_EQ("01-01-3000", result.value) << "Returned date should be 01-01-3000";
    ASSERT_EQ(false, result.is_valid) << "Invalid date should return false";
    ASSERT_EQ("Date cannot be in the future", result.error_message) << "Future date should detail error in error message";
}

TEST_F(ValidationManagerTest, validate_non_existent_date)
{
    std::string input = "31-02-2000";
    ValidationResult result = manager.validate_date(input);

    ASSERT_EQ("31-02-2000", result.value) << "Returned date should be 31-02-2000";
    ASSERT_EQ(false, result.is_valid) << "Invalid date should return false";
    ASSERT_EQ("Date does not exist", result.error_message) << "Non-existent date should detail error in error message";
}

TEST_F(ValidationManagerTest, validate_wrong_format_date)
{
    std::string input = "01/01/2000";
    ValidationResult result = manager.validate_date(input);

    ASSERT_EQ("01/01/2000", result.value) << "Returned date should be 01/01/2000";
    ASSERT_EQ(false, result.is_valid) << "Invalid date should return false";
    ASSERT_EQ("Date must use format DD-MM-YYYY", result.error_message) << "Incorrectly formatted date should detail error in error message";
}

TEST_F(ValidationManagerTest, validate_invalid_string_date)
{
    std::string input = "hello";
    ValidationResult result = manager.validate_date(input);

    ASSERT_EQ("hello", result.value) << "Returned date should be hello";
    ASSERT_EQ(false, result.is_valid) << "Invalid date should return false";
    ASSERT_EQ("Date must use format DD-MM-YYYY", result.error_message) << "Invalid string should detail error in error message";
}

TEST_F(ValidationManagerTest, validate_valid_non_zero_cost)
{
    double input = 123.00;
    ValidationResult result = manager.validate_cost(input);

    ASSERT_EQ(123.00, result.value) << "Valid cost should be £123.00";
    ASSERT_EQ(true, result.is_valid) << "Valid cost should return true";
    ASSERT_TRUE(result.error_message.empty()) << "Valid cost should have empty error message";
}

TEST_F(ValidationManagerTest, validate_valid_zero_cost)
{
    double input = 0.00;
    ValidationResult result = manager.validate_cost(input);

    ASSERT_EQ(0.00, result.value) << "Valid cost should be £0.00";
    ASSERT_EQ(true, result.is_valid) << "Valid cost should return true";
    ASSERT_TRUE(result.error_message.empty()) << "Valid cost should have empty error message";
}

TEST_F(ValidationManagerTest, validate_negative_cost)
{
    double input = -1.00;
    ValidationResult result = manager.validate_cost(input);

    ASSERT_EQ(-1.00, result.value) << "Returned cost should be -£1.00";
    ASSERT_EQ(false, result.is_valid) << "Invalid cost should return false";
    ASSERT_EQ("Cost cannot be negative", result.error_message) << "Negative cost should detail error in error message";
}

TEST_F(ValidationManagerTest, validate_negative_id)
{
    int input = -1;
    ValidationResult result = manager.validate_id(input, {});

    ASSERT_EQ(-1, result.value) << "Returned ID should be -1";
    ASSERT_EQ(false, result.is_valid) << "Negative ID should return false";
    ASSERT_EQ("ID cannot be negative", result.error_message) << "Negative ID should detail error in error message";
}

TEST_F(ValidationManagerTest, validate_id_empty_concerts)
{
    int input = 0;
    ValidationResult result = manager.validate_id(input, {});

    ASSERT_EQ(0, result.value) << "Returned ID should be 0";
    ASSERT_EQ(false, result.is_valid) << "Unused ID should return false";
    ASSERT_EQ("ID does not match existing concert", result.error_message) << "Unused ID should detail error in error message";
}

TEST_F(ValidationManagerTest, validate_id_valid_concert)
{
    int input = 1;
    Concert concert = TestData::create_test_concert1();
    ValidationResult result = manager.validate_id(input, {concert});

    ASSERT_EQ(1, result.value) << "Returned ID should be 1";
    ASSERT_EQ(true, result.is_valid) << "Valid ID should return true";
    ASSERT_TRUE(result.error_message.empty()) << "Valid ID should have empty error message";
}

TEST_F(ValidationManagerTest, validate_id_one_concert_with_non_zero_id)
{
    int input = 2;
    Concert concert = TestData::create_test_concert2();
    ValidationResult result = manager.validate_id(input, {concert});

    ASSERT_EQ(2, result.value) << "Returned ID should be 2";
    ASSERT_EQ(true, result.is_valid) << "Valid ID should return true";
    ASSERT_TRUE(result.error_message.empty()) << "Valid ID should have empty error message";
}

TEST_F(ValidationManagerTest, validate_valid_stringe)
{
    std::string input = "hello";
    ValidationResult result = manager.validate_artist(input);

    ASSERT_EQ("hello", result.value) << "Returned string should be hello";
    ASSERT_EQ(true, result.is_valid) << "Valid string should return true";
    ASSERT_TRUE(result.error_message.empty()) << "Valid string should have empty error message";
}

TEST_F(ValidationManagerTest, validate_cleans_whitespace_of_valid_string)
{
    std::string input = "  hello  ";
    ValidationResult result = manager.validate_artist(input);

    ASSERT_EQ("hello", result.value) << "Returned string should be hello";
    ASSERT_EQ(true, result.is_valid) << "Valid string should return true";
    ASSERT_TRUE(result.error_message.empty()) << "Valid string should have empty error message";
}

TEST_F(ValidationManagerTest, validate_empty_string)
{
    std::string input = "";
    ValidationResult result = manager.validate_artist(input);

    ASSERT_EQ("", result.value) << "Returned string should be empty";
    ASSERT_EQ(false, result.is_valid) << "Empty string should return false";
    ASSERT_EQ("Artist must contain non-whitespace characters", result.error_message) << "Invalid string should detail error in error message";
}

TEST_F(ValidationManagerTest, validate_only_whitespace_string)
{
    std::string input = "  ";
    ValidationResult result = manager.validate_artist(input);

    ASSERT_EQ("  ", result.value) << "Returned string should be empty";
    ASSERT_EQ(false, result.is_valid) << "Empty string should return false";
    ASSERT_EQ("Artist must contain non-whitespace characters", result.error_message) << "Invalid string should detail error in error message";
}
