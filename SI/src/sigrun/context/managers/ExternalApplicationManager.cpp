

#include "ExternalApplicationManager.hpp"
#include <QProcess>

ExternalApplicationManager::ExternalApplicationManager(double process_winid_fetch_sleep_time_ms, double process_winid_fetch_timeout_ms):
    d_process_winid_fetch_sleep_time_ms(process_winid_fetch_sleep_time_ms),
    d_process_winid_fetch_timeout_ms(process_winid_fetch_timeout_ms),
    d_process_winid_fetch_iterations(process_winid_fetch_timeout_ms / process_winid_fetch_sleep_time_ms / 1000)
{SIGRUN

}

ExternalApplicationManager::~ExternalApplicationManager() = default;

void ExternalApplicationManager::launch_application(const std::string &uuid, const std::string &file_path, const std::string &application_name)
{
    if(application_name.empty())
        launch_standard_application(uuid, file_path);
    else
    {
        WARN("SIGRun requires implementation of arbitrary invocation of external applications!");
    }
}

void ExternalApplicationManager::launch_standard_application(const std::string &uuid, const std::string &file_path)
{
    QProcess::startDetached("xdg-open", QStringList() << file_path.c_str(), QString(""));

    process_wmctrl_command_output(generate_wmctrl_command_output(generate_wmctrl_command(file_path)), uuid, file_path);
}

void ExternalApplicationManager::terminate_application(const std::string &uuid)
{
    if(MAP_HAS_KEY(d_file_uuid_to_embedded_app, uuid))
    {
        d_file_uuid_to_embedded_app[uuid]->close();
        delete d_file_uuid_to_embedded_app[uuid];
        d_file_uuid_to_embedded_app.erase(uuid);
    }
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

void ExternalApplicationManager::process_wmctrl_command_output(const QString &input, const std::string& uuid, const std::string& file_path)
{
    if(!input.isEmpty())
    {
        uint64_t winid = HEX_STRING_TO_UINT64(input.split(' ', QString::SkipEmptyParts)[0].toStdString());

        if(winid)
        {
            QMainWindow* pMainWnd = retrieve_current_main_window();

            if(pMainWnd)
                register_new_application_container(pMainWnd, uuid, winid);
            {
                ERROR("Unable to find a main window as parent for the default app of " + file_path);
            }
        }
        else
        {
            ERROR("Unable to find winid of default app of " + file_path);
        }
    }
    else
    {
        ERROR("Timeout for finding winid of default app of " + file_path);
    }
}

void ExternalApplicationManager::register_new_application_container(QMainWindow *parent, const std::string &source_uuid, uint64_t winid)
{
    d_file_uuid_to_embedded_app[source_uuid]= QWidget::createWindowContainer(QWindow::fromWinId(winid));
    d_file_uuid_to_embedded_app[source_uuid]->setParent(parent);
    d_file_uuid_to_embedded_app[source_uuid]->show();
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
