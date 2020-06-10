
#define BOOST_STACKTRACE_USE_BACKTRACE

#include <debug/Print.hpp>
#include "CrashDump.hpp"
#include <boost/stacktrace.hpp>
#include <boost/filesystem.hpp>
#include <iomanip>
#include <ctime>

#define NUM_LINES_STACKTRACE 15

#define PARENTFOLDERPATH std::string("si_crash_dumps")
#define PIPE_DEV_NULL std::string(" >> /dev/null")
#define DATEFORMATSTRING std::string("%d-%m-%Y--%H-%M-%S")
#define SIGRUN_CRASH_CORE_DUMP_FILENAME std::string("sigrun_crash_core_dump")
#define SIGRUN_CRASH_NAME_PREFIX std::string("sigrun_crash_")
#define STACKTRACE_NAME std::string("stacktrace")
#define TXT_FILE std::string("txt")

#define GCORE_COMMAND "gcore -a -o " + PARENTFOLDERPATH + "/" + SIGRUN_CRASH_NAME_PREFIX + datetime + "/" + SIGRUN_CRASH_CORE_DUMP_FILENAME + " " + std::to_string(getpid())

void CrashDump::dump_crash_information(int32_t signal)
{
    std::string msg = ERROR_COLOR(std::string("SIGRUN ") + Log::time() + " [ERROR] [FATAL] " + std::string(strsignal(signal)) + " (sginal: " + std::to_string(signal) + ")");
    Print::print(msg);

    std::string datetime = current_datetime_string();

    create_folder(PARENTFOLDERPATH);
    create_folder(PARENTFOLDERPATH + "/" + SIGRUN_CRASH_NAME_PREFIX + datetime);

    msg = ERROR_COLOR(std::string("SIGRUN ") + Log::time() + " [INFO] [CRASHDUMP] Generating Crash Report...");
    Print::print(msg);

    generate_core_dump(datetime);
    generate_stacktrace(datetime);
    zip_folder(datetime);
    delete_folder(datetime);

    msg = ERROR_COLOR(std::string("SIGRUN ") + Log::time() + " [INFO] [CRASHDUMP] Crash Report" + SIGRUN_CRASH_NAME_PREFIX + datetime + ".zip in " + boost::filesystem::current_path().string() + "/" + PARENTFOLDERPATH + " created!");
    Print::print(msg);
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
    oss << std::put_time(&tm, DATEFORMATSTRING.c_str());

    return oss.str();
}

void CrashDump::execute_linux_command(const std::string &cmd)
{
    system((cmd + PIPE_DEV_NULL).c_str());
}

void CrashDump::generate_core_dump(const std::string& datetime)
{
    execute_linux_command(GCORE_COMMAND);
}

void CrashDump::generate_stacktrace(const std::string& datetime)
{
    int32_t num_lines = NUM_LINES_STACKTRACE + 2; // ignore the first two lines which contain this file's information and sigaction

    const auto& stacktrace = boost::stacktrace::stacktrace();

    std::string m;
    for(int32_t i = 2; i < (stacktrace.size() > num_lines ? num_lines: stacktrace.size()); ++i)
        m += stacktrace[i].source_file() + " at " + stacktrace[i].name() + " in line " + std::to_string(stacktrace[i].source_line()) + "\n";

    std::ofstream log_file;
    log_file.open(PARENTFOLDERPATH + "/" + SIGRUN_CRASH_NAME_PREFIX + datetime + "/"+ SIGRUN_CRASH_NAME_PREFIX + STACKTRACE_NAME + "." + TXT_FILE, std::ios_base::out);
    log_file << m;
    log_file.close();
}

void CrashDump::zip_folder(const std::string &datetime)
{
    execute_linux_command("(cd " + PARENTFOLDERPATH + "; zip -r " + SIGRUN_CRASH_NAME_PREFIX + datetime + ".zip " + SIGRUN_CRASH_NAME_PREFIX + datetime + ")");
}

void CrashDump::delete_folder(const std::string &datetime)
{
    execute_linux_command("(cd " + PARENTFOLDERPATH + "; rm -rf " + SIGRUN_CRASH_NAME_PREFIX + datetime + ")");
}
