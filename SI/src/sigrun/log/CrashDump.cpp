
#define BOOST_STACKTRACE_USE_BACKTRACE

#include <debug/Print.hpp>
#include "CrashDump.hpp"
#include <boost/stacktrace.hpp>
#include <boost/filesystem.hpp>
#include <iomanip>
#include <ctime>

#define NUM_LINES_STACKTRACE 15

void CrashDump::dump_crash_information(int32_t signal)
{
    int32_t num_lines = NUM_LINES_STACKTRACE + 2; // ignore the first two lines which contain this file's information and sigaction

    const auto& stacktrace = boost::stacktrace::stacktrace();

    std::string msg;
    for(int32_t i = 2; i < (stacktrace.size() > num_lines ? num_lines: stacktrace.size()); ++i)
        msg += stacktrace[i].source_file() + " at " + stacktrace[i].name() + " in line " + std::to_string(stacktrace[i].source_line()) + "\n";

    std::ofstream log_file;

    if (!boost::filesystem::exists(boost::filesystem::path("crash_dumps")))
        boost::filesystem::create_directory("crash_dumps");

    time_t t = std::time(nullptr);
    tm tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y--%H-%M-%S");

    log_file.open("crash_dumps/si_crash_dump_" + oss.str() + ".txt", std::ios_base::out);
    log_file << msg;
    log_file.close();

    exit(signal);
}