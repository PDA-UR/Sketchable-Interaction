

#ifndef SITEST_LOG_HPP
#define SITEST_LOG_HPP

#include <fstream>
#include <string>

#include "sigrun/MetaType.hpp"

/**
    \brief coloring for console output
    \details
        Coloring for console output.
        Unsused for file output.
        See table of codes here: https://en.wikipedia.org/wiki/ANSI_escape_code#graphics
 */
#define ERROR_COLOR(x) ("\033[31m" + x + "\033[0m")       //red
#define UNDEFINED_COLOR(x) ("\033[1;31m" + x + "\033[0m") // bold red
#define INFO_COLOR(x) ("\033[32m" + x + "\033[0m")        // green
#define WARN_COLOR(x) ("\033[33m" + x + "\033[0m")        // yellow
#define DEBUG_COLOR(x) ("\033[37m" + x + "\033[0m")       // white / gray

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define LOG_NONE Log::MODE::NONE
#define LOG_CONSOLE Log::MODE::CONSOLE
#define LOG_FILE Log::MODE::FILE

#define LOG_SHOW_NONE Log::SHOW_TYPE::HIDDEN
#define LOG_SHOW_INFO Log::SHOW_TYPE::INFO
#define LOG_SHOW_WARN Log::SHOW_TYPE::WARN
#define LOG_SHOW_ERROR Log::SHOW_TYPE::ERROR
#define LOG_SHOW_DEBUG Log::SHOW_TYPE::DEBUG
#define LOG_SHOW_ALL Log::SHOW_TYPE::INFO | Log::SHOW_TYPE::WARN | Log::SHOW_TYPE::ERROR | Log::SHOW_TYPE::DEBUG

#define DEBUG(what, log_mode) LOG(what, Log::LOG_LEVEL::DEBUG_LEVEL, log_mode, meta_type(),__FILENAME__, __FUNCTION__, std::to_string(__LINE__))
#define INFO(what, log_mode) Log::log(what, Log::LOG_LEVEL::INFO_LEVEL, log_mode, meta_type())
#define ERROR(what, log_mode) Log::log(what, Log::LOG_LEVEL::ERROR_LEVEL, log_mode, meta_type(),__FILENAME__, __FUNCTION__, std::to_string(__LINE__))
#define WARN(what, log_mode) Log::log(what, Log::LOG_LEVEL::WARN_LEVEL, log_mode, meta_type())
#define UNDEFINED(what, log_mode) Log::log(what, Log::LOG_LEVEL::UNDEFINED_LEVEL, log_mode, meta_type(), __FILENAME__, __FUNCTION__, std::to_string(__LINE__))

class Log
{
private:
    static std::ofstream log_file;
    static std::string log_file_path;
    static const std::string PATH_DEFAULT;
    static std::string log_level(int log_level);
    static std::string time();

public:
    enum LOG_LEVEL
    {
        INFO_LEVEL = 1,
        WARN_LEVEL = 2,
        DEBUG_LEVEL = 4,
        ERROR_LEVEL = 8,
        UNDEFINED_LEVEL = 16
    };

    enum MODE
    {
        NONE = 0,
        CONSOLE = 1,
        FILE = 2
    };

    enum SHOW_TYPE
    {
        HIDDEN = 0,
        INFO = 1,
        WARN = 2,
        DEBUG = 4,
        ERROR = 8,
        UNDEFINED = 16
    };

    static int SHOW;
    static bool __DEBUG__;

    static void log(const std::string& what, int level, int logging_flags, const std::string& type, const std::string& file="", const std::string& func="", const std::string& line="");
    static void set_log_file_path(const std::string& path);
};


#endif //SITEST_LOG_HPP
