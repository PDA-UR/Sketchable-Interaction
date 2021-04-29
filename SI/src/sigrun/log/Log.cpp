
#include <chrono>
#include <debug/Print.hpp>
#include <iomanip>
#include "Log.hpp"

std::ofstream Log::log_file;
const std::string Log::PATH_DEFAULT = "log/silog.txt";
std::string Log::log_file_path = Log::PATH_DEFAULT;
bool Log::__DEBUG__ = false;
int16_t Log::SHOW = -1;
uint16_t Log::WHERE = 0;
std::vector<std::string> Log::QUENCHED = std::vector<std::string>();
std::vector<std::string> Log::MESSAGES = std::vector<std::string>();

/**
\brief central logging function outputting log messages according to its params
\details
    This is the central logging function of SIGRun.
    It requires to be called from class which are registered as SIObject.
    The parameters of this function, besides what (log message), configure the way the message is outputted.
    This static method is easier accessible via the shortcut macros.

@param what a std::string containing the log message
@param level an integer containing the id of the desired tag
@param logging_flags an integer containing where the log message is to be outputted
@param type a std::string containing the description of the functions caller via an SIObject
@param file a std::string containing the name of the file in which the log call is implemented
@param func a std::string containing the name of the function in which the log call was issued
@param line a std::string containing the number of the line of the file in which the log call is implemented

@see DEBUG(what, log_mode)
@see WARN(what, log_mode)
@see ERROR(what, log_mode)
@see INFO(what, log_mode)
@see UNDEFINED(what, log_mode)

@see SIObject
*/
void Log::log(const std::string& origin, const std::string &what, uint16_t level, const std::string& type, const std::string& file,
              const std::string& func, const std::string& line)
{
    Log::SHOW |= Log::SHOW_TYPE::UNDEFINED | Log::SHOW_TYPE::ERROR;

    if(!Log::__DEBUG__ || Log::WHERE == Log::MODE::NONE || !(level & Log::SHOW))
        return;

    for(auto& s: QUENCHED)
        if(type.find(s) != std::string::npos)
            return;

    std::string message = origin + "\t" + Log::time() + "\t" + Log::log_level(level) + " [" + type + "] " + what + ".\t" + file + "\t" + func + "\t" + line;

    if (Log::WHERE & Log::MODE::FILE)
    {
        if (!log_file.is_open())
        {
            log_file.open(log_file_path, std::ios_base::app);
            log_file << message << "\n";
            log_file.close();
        }
    }

    if (Log::WHERE & Log::MODE::CONSOLE)
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
        MESSAGES.push_back(message);
    }
}

/**
\brief
\details set the path of the file for logging output
    Set the value of the static variable log_file_path to the value of the given parameter to specifiy the file path of the log output.

@param path a std::string containing the desired file path for logging to files
*/
void Log::set_log_file_path(const std::string &path)
{
    Log::log_file_path = path;
}

/**
\brief return the level of a log message as tag according to its id
\details
    Retrieves the level of a log message according to the value of the parameter which is compared to the Log::LOG_LEVEL enum/bitfield.

@param log_level an integer containing the id of the desired tag

@return a std::string which contains a human readable version of the desired tag
*/
std::string Log::log_level(uint16_t log_level)
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

/**
\brief return current system time with milliseconds precision
\details
    Compute current system time with milliseconds precision.
    Format the date data to yyyy-MM-dd hh:mm:ss.<milliseconds>.
    Concatenate the date data to a std::string.

@return a std::string containing the formatted date data
*/
std::string Log::time()
{
    tm localTime;
    std::chrono::system_clock::time_point t = std::chrono::system_clock::now();
    time_t now = std::chrono::system_clock::to_time_t(t);
    localtime_r(&now, &localTime);

    std::chrono::seconds::rep milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(t.time_since_epoch()).count() % 1000;

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

void Log::quench(const std::string& target)
{
    QUENCHED.push_back(target);
}

void Log::unquench(const std::string& target)
{
    auto it = std::find(QUENCHED.begin(), QUENCHED.end(), target);

    if(it != QUENCHED.end())
        QUENCHED.erase(it);
}

const std::vector<std::string>& Log::messages()
{
    return MESSAGES;
}
