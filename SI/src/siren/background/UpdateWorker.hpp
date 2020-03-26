

#ifndef SITEST_UPDATEWORKER_HPP
#define SITEST_UPDATEWORKER_HPP

#include <QObject>
#include <SI/SI.hpp>
#include <QTimer>
#include <QElapsedTimer>

class UpdateWorker: public QObject, public SIObject
{ Q_OBJECT SIREN
public:
    explicit UpdateWorker(uint32_t target_fps=60);
    ~UpdateWorker();

    void start();
    Q_SLOT void stop();
    void pause();
    void resume();

    bool is_running();
    uint32_t fps() const;

    Q_SIGNAL void running_changed();
    Q_SIGNAL void updated(double delta, uint32_t fps);
    Q_SIGNAL void finished();

protected:
private:
    Q_SLOT void do_update();
    void set_running(bool running);

    bool d_is_running;
    uint32_t d_fps;
    QTimer* d_timer;
    QElapsedTimer* d_elapsed_timer;
    double d_remainder;
    double d_simulated_time;
};


#endif //SITEST_UPDATEWORKER_HPP
