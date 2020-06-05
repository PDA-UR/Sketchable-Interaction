

#include "ExternalApplicationManager.hpp"
#include <QProcess>
#include <QObject>
#include <sigrun/log/Log.hpp>
#include <sigrun/context/Context.hpp>
#include <sigrun/SIConstants.hpp>

ExternalApplicationManager::ExternalApplicationManager(double process_winid_fetch_sleep_time_ms, double process_winid_fetch_timeout_ms):
    d_process_winid_fetch_sleep_time_ms(process_winid_fetch_sleep_time_ms),
    d_process_winid_fetch_timeout_ms(process_winid_fetch_timeout_ms),
    d_process_winid_fetch_iterations(process_winid_fetch_timeout_ms / process_winid_fetch_sleep_time_ms / 1000)
{

}

ExternalApplicationManager::~ExternalApplicationManager() = default;

void ExternalApplicationManager::launch_application(const std::string &file_region_uuid, const std::string &file_path, std::shared_ptr<Region>& container, const std::string &application_name)
{
    Context::SIContext()->job_system()->execute([&]
    {
        if(application_name.empty())
            launch_standard_application(file_region_uuid, file_path);
        else
        {
            WARN("SIGRun requires implementation of arbitrary invocation of external applications!");
        }
    });
}

void ExternalApplicationManager::launch_standard_application(const std::string &file_region_uuid, const std::string &file_path)
{
    QProcess::startDetached(SI_LINUX_XDG_OPEN.c_str(), QStringList() << file_path.c_str(), QString(""));
    process_wmctrl_command_output(generate_wmctrl_command_output(generate_wmctrl_command(file_path)), file_region_uuid, file_path);
}

void ExternalApplicationManager::terminate_application(const std::string &uuid)
{
    Context::SIContext()->unregister_external_application(uuid);
}

void ExternalApplicationManager::set_process_winid_fetch_sleep_time_ms(double time)
{
    d_process_winid_fetch_sleep_time_ms = time;
    d_process_winid_fetch_iterations = d_process_winid_fetch_timeout_ms / time / 1000.0;
}

void ExternalApplicationManager::set_process_winid_fetch_timeout_ms(double time)
{
    d_process_winid_fetch_timeout_ms = time;
    d_process_winid_fetch_iterations = d_process_winid_fetch_timeout_ms / d_process_winid_fetch_sleep_time_ms / 1000.0;
}

double ExternalApplicationManager::process_winid_fetch_sleep_time_ms()
{
    return d_process_winid_fetch_sleep_time_ms;
}

double ExternalApplicationManager::process_winid_fetch_timeout_ms()
{
    return d_process_winid_fetch_timeout_ms;
}

uint32_t ExternalApplicationManager::process_winid_fetch_iterations()
{
    return d_process_winid_fetch_iterations;
}

QString ExternalApplicationManager::generate_wmctrl_command(const std::string &file_path)
{
    return QString((SI_LINUX_WMCTRL +  " " + SI_LINUX_LP_FLAG + " " + SI_LINUX_PIPE + " " + SI_LINUX_GREP + " " + SI_LINUX_I_FLAG).c_str()) + " \'" + QString(file_path.substr(file_path.find_last_of('/') + 1, std::string::npos).c_str()) + "\'";
}

QString ExternalApplicationManager::generate_wmctrl_command_output(const QString &cmd)
{
    uint16_t i = 0;
    QProcess process;

    QString output = "";

    do
    {
        usleep(d_process_winid_fetch_sleep_time_ms * 1000);
        process.start(SI_LINUX_BASH.c_str(), QStringList() << SI_LINUX_C_FLAG.c_str() << cmd);
        process.waitForFinished();
        output = QString(process.readAllStandardOutput());
    } while(++i < d_process_winid_fetch_iterations && output.isEmpty());

    return output;
}

void ExternalApplicationManager::process_wmctrl_command_output(const QString &input, const std::string& file_region_uuid, const std::string& file_path)
{
    if(!input.isEmpty())
    {
        const auto& vec = input.split(' ', QString::SkipEmptyParts).toVector().toStdVector();

        QString window_name = QString(std::transform_reduce(vec.begin() + 4, vec.end(), std::string(""), [](const std::string& a, const std::string& b)
        {
            return a + " " + b;
        }, [](auto& qstr)
        {
            return qstr.toStdString();
        }).c_str());

        uint64_t winid = HEX_STRING_TO_UINT64(vec[0].toStdString());
        uint64_t _pid = std::stoul(vec[2].toStdString());

        if(winid)
            register_new_application_container(file_region_uuid, winid, _pid, window_name);
        else
            ERROR("Unable to find winid of default app of " + file_path);
    }
    else
        ERROR("Timeout for finding winid of default app of " + file_path);
}

void ExternalApplicationManager::register_new_application_container(const std::string &file_region_uuid, uint64_t winid, uint64_t pid, const QString& window_name)
{
    Context::SIContext()->register_new_application_container(winid, pid, window_name, file_region_uuid);
}
