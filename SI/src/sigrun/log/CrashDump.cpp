
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
    std::string datetime = current_datetime_string();

    create_folder("crash_dumps");
    create_folder("crash_dumps/sigrun_crash_" + datetime);
    execute_linux_command("gcore -a -o crash_dumps/sigrun_crash_" + datetime + "/sigrun_crash_core_dump %d");
    generate_stacktrace(datetime);
    exit(signal);
}

void CrashDump::create_folder(const std::string &path)
{
    if (!boost::filesystem::exists(boost::filesystem::path(path)))
        boost::filesystem::create_directory(path);
}

std::string CrashDump::current_datetime_string()
{
    time_t t = std::time(nullptr);
    tm tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y--%H-%M-%S");

    return oss.str();
}

void CrashDump::execute_linux_command(const std::string &cmd)
{
    char command[1024];
    sprintf(command, (cmd + " >> /dev/null").c_str(), getpid());
    system(command);
}

void CrashDump::generate_stacktrace(const std::string& datetime)
{
    int32_t num_lines = NUM_LINES_STACKTRACE + 2; // ignore the first two lines which contain this file's information and sigaction

    const auto& stacktrace = boost::stacktrace::stacktrace();

    std::string msg;
    for(int32_t i = 2; i < (stacktrace.size() > num_lines ? num_lines: stacktrace.size()); ++i)
        msg += stacktrace[i].source_file() + " at " + stacktrace[i].name() + " in line " + std::to_string(stacktrace[i].source_line()) + "\n";

    std::ofstream log_file;
    log_file.open("crash_dumps/sigrun_crash_" + datetime + "/sigrun_crash_stacktrace.txt", std::ios_base::out);
    log_file << msg;
    log_file.close();
}
