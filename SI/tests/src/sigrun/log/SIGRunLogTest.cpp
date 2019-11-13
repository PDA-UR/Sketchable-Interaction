

#include <sigrun/log/Log.hpp>
#include <debug/Print.hpp>
#include "SIGRunLogTest.hpp"
#include <regex>

std::vector<int> levels
{
    Log::LOG_LEVEL::INFO_LEVEL, Log::LOG_LEVEL::WARN_LEVEL, Log::LOG_LEVEL::DEBUG_LEVEL, Log::LOG_LEVEL::ERROR_LEVEL, Log::LOG_LEVEL::UNDEFINED_LEVEL
};

INSTANTIATE_TEST_SUITE_P(LogLevelTest, SIGRunLogLogLevelTest, testing::ValuesIn(levels));

TEST_P(SIGRunLogLogLevelTest, log_level_logging)
{
    switch (GetParam())
    {
        case Log::LOG_LEVEL::INFO_LEVEL:
            ASSERT_TRUE(Log::log_level(GetParam()) == "[INFO]");
            break;

        case Log::LOG_LEVEL::WARN_LEVEL:
            ASSERT_TRUE(Log::log_level(GetParam()) == "[WARN]");
            break;

        case Log::LOG_LEVEL::DEBUG_LEVEL:
            ASSERT_TRUE(Log::log_level(GetParam()) == "[DEBUG]");
            break;

        case Log::LOG_LEVEL::ERROR_LEVEL:
            ASSERT_TRUE(Log::log_level(GetParam()) == "[ERROR]");
            break;

        default:
            ASSERT_TRUE(Log::log_level(GetParam()) == "[UNDEFINED]");
    }
}

TEST_F(SIGRunLogTest, time)
{
    std::regex re(R"(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}.\d*)");
    std::cmatch m;

    EXPECT_TRUE(std::regex_match(Log::time().c_str(), m, re));
}

TEST_F(SIGRunLogTest, set_log_file_path)
{
    Log::set_log_file_path("Hello World");

    ASSERT_TRUE(Log::log_file_path == "Hello World");
}

TEST_F(SIGRunLogTest, log_show)
{
    Log::SHOW = LOG_SHOW_ALL;
    ASSERT_EQ(Log::SHOW, Log::SHOW_TYPE::INFO | Log::SHOW_TYPE::WARN | Log::SHOW_TYPE::ERROR | Log::SHOW_TYPE::DEBUG);

    Log::SHOW = LOG_SHOW_NONE;
    ASSERT_FALSE(Log::SHOW);

    // INFO
    Log::SHOW = LOG_SHOW_INFO;
    INFO("", LOG_CONSOLE);
    ASSERT_EQ(Log::SHOW,  Log::SHOW_TYPE::INFO | Log::SHOW_TYPE::ERROR | Log::SHOW_TYPE::UNDEFINED);

    // WARN
    Log::SHOW = LOG_SHOW_WARN;
    WARN("", LOG_CONSOLE);
    ASSERT_EQ(Log::SHOW,  Log::SHOW_TYPE::WARN | Log::SHOW_TYPE::ERROR | Log::SHOW_TYPE::UNDEFINED);

    // DEBUG
    Log::SHOW = LOG_SHOW_DEBUG;
    DEBUG("", LOG_CONSOLE);
    ASSERT_EQ(Log::SHOW,  Log::SHOW_TYPE::DEBUG | Log::SHOW_TYPE::ERROR | Log::SHOW_TYPE::UNDEFINED);

    // ERROR
    Log::SHOW = LOG_SHOW_ERROR;
    ERROR("", LOG_CONSOLE);
    ASSERT_EQ(Log::SHOW, Log::SHOW_TYPE::ERROR | Log::SHOW_TYPE::UNDEFINED);

    // INFO + WARN
    Log::SHOW = LOG_SHOW_INFO | LOG_SHOW_WARN;
    WARN("", LOG_CONSOLE);
    ASSERT_EQ(Log::SHOW,  Log::SHOW_TYPE::INFO | Log::SHOW_TYPE:: WARN | Log::SHOW_TYPE::ERROR | Log::SHOW_TYPE::UNDEFINED);

    // INFO + DEBUG
    Log::SHOW = LOG_SHOW_INFO | LOG_SHOW_DEBUG;
    DEBUG("", LOG_CONSOLE);
    ASSERT_EQ(Log::SHOW,  Log::SHOW_TYPE::INFO | Log::SHOW_TYPE:: DEBUG | Log::SHOW_TYPE::ERROR | Log::SHOW_TYPE::UNDEFINED);

    // WARN + DEBUG
    Log::SHOW = LOG_SHOW_DEBUG | LOG_SHOW_WARN;
    DEBUG("", LOG_CONSOLE);
    ASSERT_EQ(Log::SHOW,  Log::SHOW_TYPE::DEBUG | Log::SHOW_TYPE:: WARN | Log::SHOW_TYPE::ERROR | Log::SHOW_TYPE::UNDEFINED);

    Log::SHOW = LOG_SHOW_NONE;
    ERROR("", LOG_CONSOLE);
    ASSERT_EQ(Log::SHOW, Log::SHOW_TYPE::ERROR | Log::SHOW_TYPE::UNDEFINED);

    UNDEFINED("", LOG_CONSOLE);
    ASSERT_EQ(Log::SHOW, Log::SHOW_TYPE::ERROR | Log::SHOW_TYPE::UNDEFINED);
}

TEST_F(SIGRunLogTest, log_color)
{
    std::string info_color = std::string(INFO_COLOR(std::string("")));
    std::string error_color = std::string(ERROR_COLOR(std::string("")));
    std::string debug_color = std::string(DEBUG_COLOR(std::string("")));
    std::string warn_color = std::string(WARN_COLOR(std::string("")));
    std::string undefined_color = std::string(UNDEFINED_COLOR(std::string("")));

    ASSERT_EQ(std::string("\033[31m\033[0m"), error_color);
    ASSERT_EQ(std::string("\033[1;31m\033[0m"), undefined_color);
    ASSERT_EQ(std::string("\033[32m\033[0m"), info_color);
    ASSERT_EQ(std::string("\033[33m\033[0m"), warn_color);
    ASSERT_EQ(std::string("\033[37m\033[0m"), debug_color);
}