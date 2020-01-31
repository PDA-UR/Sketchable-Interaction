
#include "UpdateWorker.hpp"
#include <QApplication>

UpdateWorker::UpdateWorker(int target_fps): d_is_running(false), d_fps(target_fps), d_remainder(0.0), d_simulated_time(0.0)
{
    d_timer = new QTimer(this);
    d_elapsed_timer = new QElapsedTimer();
}

UpdateWorker::~UpdateWorker()
{
    delete d_timer;
    d_timer = nullptr;

    d_elapsed_timer->invalidate();
    delete d_elapsed_timer;
    d_elapsed_timer = nullptr;
}

void UpdateWorker::start()
{
    connect(d_timer, SIGNAL(timeout()), this, SLOT(do_update()));
    connect(QApplication::instance(), SIGNAL(aboutToQuit()), this, SLOT(stop()));
    set_running(true);
    d_timer->start(5);
    d_elapsed_timer->restart();
}

void UpdateWorker::stop()
{
    d_simulated_time = 0;
    d_remainder = 0;
    set_running(false);

    d_timer->disconnect(this);

    Q_EMIT finished();
}

void UpdateWorker::pause()
{
    if(!d_is_running)
        return;

    d_timer->disconnect(this);
    set_running(false);
}

void UpdateWorker::resume()
{
    if(d_is_running)
        return;

    d_elapsed_timer->start();
    connect(d_timer, SIGNAL(timeout()), this, SLOT(do_update()));

    set_running(true);
}


bool UpdateWorker::is_running()
{
    return d_is_running;
}

int UpdateWorker::fps() const
{
    return d_fps;
}

void UpdateWorker::do_update()
{
    static const double delta = 1.0 / d_fps;

    double secondsSinceLastUpdate = d_elapsed_timer->restart() * 0.001;
    d_remainder += secondsSinceLastUpdate;

    while(d_remainder > 0)
    {
        Q_EMIT updated(delta, fps());

        d_remainder -= delta;
        d_simulated_time += delta;
    }
}

void UpdateWorker::set_running(bool running)
{
    if(running == d_is_running)
        return;

    d_is_running = running;

    Q_EMIT running_changed();
}
