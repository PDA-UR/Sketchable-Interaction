
#include <chrono>
#include <debug/Print.hpp>
#include <iomanip>
#include "Log.hpp"

std::ofstream Log::log_file;
const std::string Log::PATH_DEFAULT = "log/silog.txt";
std::string Log::log_file_path = Log::PATH_DEFAULT;
bool Log::__DEBUG__ = false;
int Log::SHOW = -1;

void Log::log(const std::string &what, int level, int logging_flags, const std::string& type, const std::string& file,
              const std::string& func, const std::string& line)
{
    if(!Log::__DEBUG__ || Log::SHOW == Log::SHOW_TYPE::HIDDEN || logging_flags == Log::MODE::NONE)
        return;

    Log::SHOW |= Log::SHOW_TYPE::UNDEFINED | Log::SHOW_TYPE::ERROR;

    if (level & Log::SHOW)
    {
        std::string message =
                "SIGRun\t" + Log::time() + "\t" + Log::log_level(level) + " [" + type + "] " + what + ".\t" + file +
                "\t" + func + "\t" + line;

        if (logging_flags & Log::MODE::FILE)
        {
            if (!log_file.is_open())
            {
                log_file.open(log_file_path, std::ios_base::app);
                log_file << message << "\n";
                log_file.close();
            }
        }

        if (logging_flags & Log::MODE::CONSOLE)
        {
            switch (level)
            {
                case Log::LOG_LEVEL::INFO_LEVEL:
                    message = INFO_COLOR(message);
                    break;

                case Log::LOG_LEVEL::WARN_LEVEL:
                    message = WARN_COLOR(message);
                    break;

                case Log::LOG_LEVEL::ERROR_LEVEL:
                    message = ERROR_COLOR(message);
                    break;

                case Log::LOG_LEVEL::DEBUG_LEVEL:
                    message = DEBUG_COLOR(message);
                    break;

                default:
                    message = UNDEFINED_COLOR(message);
                    break;
            }

            Print::print(message);
        }
    }
}

void Log::set_log_file_path(const std::string &path)
{
    Log::log_file_path = path;
}

std::string Log::log_level(int log_level)
{
    switch (log_level)
    {
        case Log::LOG_LEVEL::INFO_LEVEL:
            return "[INFO]";
        case Log::LOG_LEVEL::WARN_LEVEL:
            return "[WARN]";
        case Log::LOG_LEVEL::ERROR_LEVEL:
            return "[ERROR]";
        case Log::LOG_LEVEL::DEBUG_LEVEL:
            return "[DEBUG]";
        default:
            return "[UNDEFINED]";
    }
}

std::string Log::time()
{
    tm localTime;
    std::chrono::system_clock::time_point t = std::chrono::system_clock::now();
    time_t now = std::chrono::system_clock::to_time_t(t);
    localtime_r(&now, &localTime);

    const std::chrono::duration<double> tse = t.time_since_epoch();
    std::chrono::seconds::rep milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(tse).count() % 1000;

    std::ostringstream oss;

    oss << (1900 + localTime.tm_year) << '-'
              << std::setfill('0') << std::setw(2) << (localTime.tm_mon + 1) << '-'
              << std::setfill('0') << std::setw(2) << localTime.tm_mday << ' '
              << std::setfill('0') << std::setw(2) << localTime.tm_hour << ':'
              << std::setfill('0') << std::setw(2) << localTime.tm_min << ':'
              << std::setfill('0') << std::setw(2) << localTime.tm_sec << '.'
              << std::setfill('0') << std::setw(3) << milliseconds;

    return oss.str();
}
