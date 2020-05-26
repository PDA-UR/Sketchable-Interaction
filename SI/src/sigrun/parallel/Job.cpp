

#include "Job.hpp"

Job::Job(const std::function<void()> &job_func, const Job::PRIORITY &priority):
    d_job_function_execute(job_func),
    d_priority(priority)
{

}

Job::Job(const std::function<void(const JobDispatchArgs &)> &job_func, const Job::PRIORITY &priority):
    d_job_function_dispatch(job_func),
    d_priority(priority)
{

}

Job::~Job() = default;

void Job::execute()
{
    d_job_function_execute();
}

void Job::operator()()
{
    execute();
}

void Job::execute(const JobDispatchArgs &args)
{
    d_job_function_dispatch(args);
}

void Job::operator()(const JobDispatchArgs &args)
{
    execute(args);
}
