

#ifndef SI_SIGRUN_Job
#define SI_SIGRUN_Job

#include <sigrun/SIObject.hpp>
#include <functional>

struct JobDispatchArgs
{
    JobDispatchArgs():
            jobIndex(0),
            groupIndex(0)
    {

    }

    JobDispatchArgs(uint32_t ji, uint32_t gi):
            jobIndex(ji),
            groupIndex(gi)
    {

    }

    uint32_t jobIndex;
    uint32_t groupIndex;
};

class Job : public SIObject
{
    SIGRUN
public:
    enum PRIORITY
    {
        HIGH,
        NORMAL,
        LOW
    };

    Job(const std::function<void()>& job_func, const PRIORITY& priority=PRIORITY::NORMAL);
    Job(const std::function<void(const JobDispatchArgs& args)>& job_func, const PRIORITY& priority=PRIORITY::NORMAL);
    ~Job();

    void execute();
    void operator ()();

    void execute(const JobDispatchArgs& args);
    void operator ()(const JobDispatchArgs& args);

private:
    std::function<void()> d_job_function_execute;
    std::function<void(const JobDispatchArgs& args)> d_job_function_dispatch;
    PRIORITY d_priority;
};

#endif // SI_SIGRUN_Job