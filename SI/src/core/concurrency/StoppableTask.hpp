

#ifndef SITEST_STOPPABLETASK_HPP
#define SITEST_STOPPABLETASK_HPP


#include <future>

class StoppableTask
{
public:
    StoppableTask();
    explicit StoppableTask(StoppableTask&& obj);
    StoppableTask& operator =(StoppableTask&& obj);

    virtual void run() = 0;
    void operator ()();

    void set_is_running(bool is_running);

    bool is_stop_requested();

    void stop();

private:
    std::promise<void> d_exit_signal;
    std::future<void> d_future_object;

protected:
    bool d_is_running = false;
};


#endif //SITEST_STOPPABLETASK_HPP
