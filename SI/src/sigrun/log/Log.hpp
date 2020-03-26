

#ifndef SITEST_LOG_HPP
#define SITEST_LOG_HPP

#include <fstream>
#include <string>

#include "sigrun/SIObject.hpp"

/**
    \brief red coloring for console output
    \details
        Coloring for console output.
        Unsused for file output.
        See table of codes here: https://en.wikipedia.org/wiki/ANSI_escape_code#graphics
 */
#define ERROR_COLOR(x) ("\033[31m" + x + "\033[0m")

/**
    \brief bold red coloring for console output
    \details
        Coloring for console output.
        Unsused for file output.
        See table of codes here: https://en.wikipedia.org/wiki/ANSI_escape_code#graphics
 */
#define UNDEFINED_COLOR(x) ("\033[1;31m" + x + "\033[0m")

/**
    \brief green coloring for console output
    \details
        Coloring for console output.
        Unsused for file output.
        See table of codes here: https://en.wikipedia.org/wiki/ANSI_escape_code#graphics
 */
#define INFO_COLOR(x) ("\033[32m" + x + "\033[0m")

/**
    \brief yellow coloring for console output
    \details
        Coloring for console output.
        Unsused for file output.
        See table of codes here: https://en.wikipedia.org/wiki/ANSI_escape_code#graphics
 */
#define WARN_COLOR(x) ("\033[33m" + x + "\033[0m")

/**
    \brief white/gray coloring for console output
    \details
        Coloring for console output.
        Unsused for file output.
        See table of codes here: https://en.wikipedia.org/wiki/ANSI_escape_code#graphics
 */
#define DEBUG_COLOR(x) ("\033[37m" + x + "\033[0m")

/**
    \brief file name and extension without full path
    \details
        file name and extension without full path
 */
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

/**
    \brief disable logging output
 */
#define LOG_NONE Log::MODE::NONE

/**
    \brief output logging data to stdout
 */
#define LOG_CONSOLE Log::MODE::CONSOLE

/**
    \brief output logging data to file
 */
#define LOG_FILE Log::MODE::FILE

/**
    \brief disable logging except for errors and undefined behaviour
 */
#define LOG_SHOW_NONE Log::SHOW_TYPE::HIDDEN

/**
    \brief enable logging of data tagged as INFO (information) additionally to errors and undefined behaviour
 */
#define LOG_SHOW_INFO Log::SHOW_TYPE::INFO

/**
    \brief enable logging of data tagged as WARN (warning) additionally to errors and undefined behaviour
 */
#define LOG_SHOW_WARN Log::SHOW_TYPE::WARN

/**
    \brief enable logging of data tagged as ERROR (error) however this per default enabled and cannot be disabled
 */
#define LOG_SHOW_ERROR Log::SHOW_TYPE::ERROR

/**
    \brief enable logging of data tagged as DEBUG (debugging information) additionally to errors and undefined behaviour
 */
#define LOG_SHOW_DEBUG Log::SHOW_TYPE::DEBUG

/**
    \brief enable logging of any tagged data
 */
#define LOG_SHOW_ALL Log::SHOW_TYPE::INFO | Log::SHOW_TYPE::WARN | Log::SHOW_TYPE::ERROR | Log::SHOW_TYPE::DEBUG

/**
    \brief perform logging of data with the DEBUG tag
    \details
        Shortcut macro for logging of data with the DEBUG tag which uses static access of log() function of Log class

    \param what the message to be logged
    \param log_mode the description where the message is outputted (\see Log::MODE::CONSOLE or \see Log::MODE::FILE or both)

    \see Log::log()
 */
#define DEBUG(what) Log::log(origin(), what, Log::LOG_LEVEL::DEBUG_LEVEL, meta_type(),__FILENAME__, __FUNCTION__, std::to_string(__LINE__))

/**
    \brief perform logging of data with the INFO tag
    \details
        Shortcut macro for logging of data with the INFO tag which uses static access of log() function of Log class

    \param what the message to be logged
    \param log_mode the description where the message is outputted (\see Log::MODE::CONSOLE or \see Log::MODE::FILE or both)

    \see Log::log()
 */
#define INFO(what) Log::log(origin(), what, Log::LOG_LEVEL::INFO_LEVEL, meta_type())

/**
    \brief perform logging of data with the ERROR tag
    \details
        Shortcut macro for logging of data with the ERROR tag which uses static access of log() function of Log class

    \param what the message to be logged
    \param log_mode the description where the message is outputted (\see Log::MODE::CONSOLE or \see Log::MODE::FILE or both)

    \see Log::log()
 */
#define ERROR(what) Log::log(origin(), what, Log::LOG_LEVEL::ERROR_LEVEL, meta_type(),__FILENAME__, __FUNCTION__, std::to_string(__LINE__))

/**
    \brief perform logging of data with the WARN tag
    \details
        Shortcut macro for logging of data with the WARN tag which uses static access of log() function of Log class

    \param what the message to be logged
    \param log_mode the description where the message is outputted (\see Log::MODE::CONSOLE or \see Log::MODE::FILE or both)

    \see Log::log()
 */
#define WARN(what) Log::log(origin(), what, Log::LOG_LEVEL::WARN_LEVEL, meta_type())

/**
    \brief perform logging of data with the UNDEFINED tag
    \details
        Shortcut macro for logging of data with the UNDEFINED tag which uses static access of log() function of Log class

    \param what the message to be logged
    \param log_mode the description where the message is outputted (\see Log::MODE::CONSOLE or \see Log::MODE::FILE or both)

    \see Log::log()
 */
#define UNDEFINED(what) Log::log(origin(), what, Log::LOG_LEVEL::UNDEFINED_LEVEL, meta_type(), __FILENAME__, __FUNCTION__, std::to_string(__LINE__))

/** \class Log
	\brief Log class serving as central logging functionality for easy logging data output
    \details
        This class serves as the central knot for all output operations in terms of log messages.
        This class is a static class featuring no ctor or dtor.
        Shortcut macros make the access to this class logging functionality more easier.

    @see DEBUG(what, log_mode)
    @see WARN(what, log_mode)
    @see ERROR(what, log_mode)
    @see INFO(what, log_mode)
    @see UNDEFINED(what, log_mode)
*/
class Log
{
private:
    /**
        object used for file opening, writing and closing operations
    */
    static std::ofstream log_file;

    /**
        constant used for storing the default path of the log file
    */
    static const std::string PATH_DEFAULT;

public:

    /**
        actual path to logfile
    */
    static std::string log_file_path;

    /**
        \brief enum for log level selection modelled as a bitfield
        \details
            The log level describes which tag is assigned to a log message.
    */
    enum LOG_LEVEL
    {
        INFO_LEVEL = 0b00001,
        WARN_LEVEL = 0b00010,
        DEBUG_LEVEL = 0b00100,
        ERROR_LEVEL = 0b01000,
        UNDEFINED_LEVEL = 0b10000
    };

    /**
        \brief enum for log mode selection modelled as a bitfield
        \details
            The log mode describes where a log message is outputted.
            A mode is ignored if it is not specified.
            Due to the enum being modelled as a bitfield, users can use the | operator to selectively enable modes for logging output.
            Example for enabling printing to stdout as well as to a file:
                int mode = CONSOLE | FILE;
    */
    enum MODE
    {
        NONE = 0,
        CONSOLE = 1,
        FILE = 2
    };

    /**
        \brief enum for log show type selection modelled as a bitfield
        \details
            The log show type describes which log messages are outputted based on their tags.
            Tags which are not specified are ignored.
            Due to the enum being modelled as a bitfield, users can use the | operator to selectively enable tags for logging output.
            Example for enabling DEBUG and WARN tags without the INFO tag: int loglevel = WARN | DEBUG
    */
    enum SHOW_TYPE
    {
        HIDDEN = 0,
        INFO = 1,
        WARN = 2,
        DEBUG = 4,
        ERROR = 8,
        UNDEFINED = 16
    };

    /**
        \brief the integer variable containing which log messages are outputted based on their tag
        \details
            This integer variable regulates which log messages are outputted, according to their tags.
    */
    static int16_t SHOW;

    /**

    */
    static uint16_t WHERE;

    /**
        \brief the flag which is required to be set to true if the logging system is required to be used.
        \details
            This flag is the center of enabling (__DEBUG__ is set to true) or disabling (__DEBUG__ is set to false) the entire logging system.
    */
    static bool __DEBUG__;

    template <typename T>
    static void log(const std::string& origin, const T& what, uint16_t level, const std::string& type, const std::string& file="", const std::string& func="", const std::string& line="")
    {
        log(origin, std::to_string(what), level, type, file, func, line);
    }

    static void log(const std::string& origin, const char* what, uint16_t level, const std::string& type, const std::string& file="", const std::string& func="", const std::string& line="")
    {
        log(origin, std::string(what), level, type, file, func, line);
    }

    static void log(const std::string& origin, const std::string& what, uint16_t level, const std::string& type, const std::string& file="", const std::string& func="", const std::string& line="");
    static void set_log_file_path(const std::string& path);

    static std::string log_level(uint16_t log_level);
    static std::string time();

    static void quench(const std::string& target);
    static void unquench(const std::string& target);

    static std::vector<std::string> QUENCHED;
};


#endif //SITEST_LOG_HPP
