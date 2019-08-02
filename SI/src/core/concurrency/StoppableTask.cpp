

#include "StoppableTask.hpp"

StoppableTask::StoppableTask() : d_future_object(d_exit_signal.get_future())
{

}

StoppableTask::StoppableTask(StoppableTask &&obj) : d_exit_signal(std::move(obj.d_exit_signal)), d_future_object(std::move(obj.d_future_object))
{

}

StoppableTask &StoppableTask::operator=(StoppableTask &&obj)
{
    d_exit_signal = std::move(obj.d_exit_signal);
    d_future_object = std::move(obj.d_future_object);

    return *this;
}

void StoppableTask::operator()()
{
    run();
}

void StoppableTask::set_is_running(bool is_running)
{
    d_is_running = is_running;
}

bool StoppableTask::is_stop_requested()
{
    d_is_running = d_future_object.wait_for(std::chrono::milliseconds(0)) != std::future_status::timeout;
    return d_is_running;
}

void StoppableTask::stop()
{
    d_exit_signal.set_value();
}
