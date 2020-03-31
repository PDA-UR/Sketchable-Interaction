

#ifndef SI_SIGRUN_ExternalApplicationManager
#define SI_SIGRUN_ExternalApplicationManager

#include <sigrun/SIObject.hpp>
#include <sigrun/region/Region.hpp>
#include <unordered_map>
#include <QWidget>
#include <QString>
#include <QMainWindow>
#include <QWindow>
#include <QApplication>
#include <sigrun/context/managers/helpers/input/ExternalObject.hpp>

#define DEFAULT_PROCESS_WINID_FETCH_SLEEP_TIME_MS (1.0 / 60.0)
#define DEFAULT_PROCESS_WINID_FETCH_TIMEOUT_MS 1000.0

class ExternalApplicationManager: public SIObject
{ SIGRUN
public:
    ExternalApplicationManager(double process_winid_fetch_sleep_time_ms=DEFAULT_PROCESS_WINID_FETCH_SLEEP_TIME_MS, double process_winid_fetch_timeout_ms=DEFAULT_PROCESS_WINID_FETCH_TIMEOUT_MS);
    ~ExternalApplicationManager();

    void launch_application(const std::string& uuid, const std::string& file_path, std::shared_ptr<Region>& reg, const std::string& application_name="");
    void launch_standard_application(const std::string& uuid, const std::string& file_path, std::shared_ptr<Region>& reg);
    void terminate_application(const std::string& uuid);

    void set_process_winid_fetch_sleep_time_ms(double time);
    void set_process_winid_fetch_timeout_ms(double time);

    double process_winid_fetch_sleep_time_ms();
    double process_winid_fetch_timeout_ms();
    uint32_t process_winid_fetch_iterations();

private:
    QString generate_wmctrl_command(const std::string& file_path);
    QString generate_wmctrl_command_output(const QString& cmd);

    void process_wmctrl_command_output(const QString& input, const std::string& uuid, const std::string& file_path, std::shared_ptr<Region>& reg, uint64_t pid);
    void register_new_application_container(QMainWindow* parent, const std::string& source_uuid, uint64_t winid, std::shared_ptr<Region>& reg, uint64_t pid);

    QMainWindow* retrieve_current_main_window();

    double d_process_winid_fetch_sleep_time_ms;
    double d_process_winid_fetch_timeout_ms;
    uint32_t d_process_winid_fetch_iterations;
};

#endif // SI_SIGRUN_ExternalApplicationManager