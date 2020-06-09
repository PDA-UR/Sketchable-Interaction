

#ifndef SI_SIGRUN_CrashDump
#define SI_SIGRUN_CrashDump


class CrashDump
{

public:
    static void dump_crash_information(int32_t signal);

private:
    static std::string current_datetime_string();

    static void create_folder(const std::string& path);
    static void execute_linux_command(const std::string& cmd);
    static void generate_core_dump(const std::string& datetime);
    static void generate_stacktrace(const std::string& datetime);
    static void zip_folder(const std::string& datetime);
    static void delete_folder(const std::string& datetime);
};

#endif // SI_SIGRUN_CrashDump