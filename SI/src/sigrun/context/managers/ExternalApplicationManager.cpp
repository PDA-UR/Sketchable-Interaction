

#include "ExternalApplicationManager.hpp"
#include <QProcess>
#include <QObject>
#include <sigrun/log/Log.hpp>
#include <sigrun/context/Context.hpp>

ExternalApplicationManager::ExternalApplicationManager(double process_winid_fetch_sleep_time_ms, double process_winid_fetch_timeout_ms):
    d_process_winid_fetch_sleep_time_ms(process_winid_fetch_sleep_time_ms),
    d_process_winid_fetch_timeout_ms(process_winid_fetch_timeout_ms),
    d_process_winid_fetch_iterations(process_winid_fetch_timeout_ms / process_winid_fetch_sleep_time_ms / 1000)
{

}

ExternalApplicationManager::~ExternalApplicationManager() = default;

void ExternalApplicationManager::launch_application(const std::string &uuid, const std::string &file_path, std::shared_ptr<Region>& reg, const std::string &application_name)
{
    if(application_name.empty())
        launch_standard_application(uuid, file_path, reg);
    else
    {
        WARN("SIGRun requires implementation of arbitrary invocation of external applications!");
    }
}

void ExternalApplicationManager::launch_standard_application(const std::string &uuid, const std::string &file_path, std::shared_ptr<Region>& reg)
{
    qint64 pid = 0;
    QProcess::startDetached("xdg-open", QStringList() << file_path.c_str(), QString(""), &pid);
    process_wmctrl_command_output(generate_wmctrl_command_output(generate_wmctrl_command(file_path)), uuid, file_path, reg, pid);
}

void ExternalApplicationManager::terminate_application(const std::string &uuid)
{
    INFO("TERMINATE REQUESTED! UNIMPLEMENTED");
}

void ExternalApplicationManager::set_process_winid_fetch_sleep_time_ms(double time)
{
    d_process_winid_fetch_sleep_time_ms = time;

    d_process_winid_fetch_iterations = d_process_winid_fetch_timeout_ms / d_process_winid_fetch_sleep_time_ms / 1000.0;
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
    return "wmctrl -lp | grep -i \'" + QString(file_path.substr(file_path.find_last_of('/') + 1, std::string::npos).c_str()) + "\'";
}

QString ExternalApplicationManager::generate_wmctrl_command_output(const QString &cmd)
{
    uint16_t i = 0;
    QProcess process;

    QString output = "";

    do
    {
        usleep(d_process_winid_fetch_sleep_time_ms * 1000);
        process.start("bash", QStringList() << "-c" << cmd);
        process.waitForFinished();
        output = QString(process.readAllStandardOutput());
    } while(++i < d_process_winid_fetch_iterations && output.isEmpty());

    return output;
}

void ExternalApplicationManager::process_wmctrl_command_output(const QString &input, const std::string& uuid, const std::string& file_path, std::shared_ptr<Region>& reg, uint64_t pid)
{
    if(!input.isEmpty())
    {
        uint64_t winid = HEX_STRING_TO_UINT64(input.split(' ', QString::SkipEmptyParts)[0].toStdString());

        if(winid)
        {
            QMainWindow* pMainWnd = retrieve_current_main_window();

            if(pMainWnd)
                register_new_application_container(pMainWnd, uuid, winid, reg, pid);
            else
                ERROR("Unable to find a main window as parent for the default app of " + file_path);
        }
        else
            ERROR("Unable to find winid of default app of " + file_path);
    }
    else
        ERROR("Timeout for finding winid of default app of " + file_path);
}

void ExternalApplicationManager::register_new_application_container(QMainWindow *parent, const std::string &source_uuid, uint64_t winid, std::shared_ptr<Region>& reg, uint64_t pid)
{
    QWidget* window = QWidget::createWindowContainer(QWindow::fromWinId(winid));
    window->setParent(parent);
    window->setGeometry(20, 20, Context::SIContext()->width() * 0.3 - 40, Context::SIContext()->height() * 0.3 - 40);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();

    Context::SIContext()->input_manager()->register_external_application(reg, window, pid);
}

QMainWindow* ExternalApplicationManager::retrieve_current_main_window()
{
    QMainWindow* pMainWnd;

    for(QWidget* pWidget : QApplication::topLevelWidgets())
    {
        pMainWnd = qobject_cast<QMainWindow*>(pWidget);

        if (pMainWnd)
            return pMainWnd;
    }

    return nullptr;
}
