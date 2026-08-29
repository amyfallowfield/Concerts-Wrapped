#include <gtest/gtest.h>

#include "Logger.h"

class LoggerTest : public testing::Test
{
public:

    std::ostringstream logs;

    std::streambuf* original_cerr;

    void SetUp() override
    {
        original_cerr = std::cerr.rdbuf();

        std::cerr.rdbuf(logs.rdbuf());
    }

    void TearDown() override
    {
        std::cerr.rdbuf(original_cerr);
    }
};

TEST_F(LoggerTest, info_logs)
{
    Logger::Info("File", "Function", "Message");

    ASSERT_NE(std::string::npos, logs.str().find("[INFO] [File::Function] Message"));
}
TEST_F(LoggerTest, warn_logs)
{
    Logger::Warn("File", "Function", "Message");

    ASSERT_NE(std::string::npos, logs.str().find("[WARN] [File::Function] Message"));
}
TEST_F(LoggerTest, error_logs)
{
    Logger::Error("File", "Function", "Message");

    ASSERT_NE(std::string::npos, logs.str().find("[ERROR] [File::Function] Message"));
}
