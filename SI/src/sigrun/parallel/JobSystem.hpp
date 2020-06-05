

#ifndef SI_SIGRUN_JobSystem
#define SI_SIGRUN_JobSystem

#include <sigrun/SIObject.hpp>
#include <sigrun/plugin/PythonGlobalInterpreterLockGuard.hpp>
#include "Job.hpp"
#include <condition_variable>
#include <thread>
#include <functional>
#include <memory>
#include <sigrun/parallel/helpers/ThreadSafeRingBuffer.hpp>


template <typename JobFuncType, int pool_size>
class JobSystem : public SIObject
{ SIGRUN

public:
    ~JobSystem() = default;

    JobSystem():
        d_num_hardware_threads(std::thread::hardware_concurrency()),
        d_job_buffer(std::make_unique<ThreadSafeRingBuffer<Job*, pool_size>>())
    {
        d_finished_label.store(0);

        for(uint32_t i = 0; i < d_num_hardware_threads; ++i)
        {
            std::thread worker([&]
            {
                Job* job;

                while (!d_is_close_requested)
                {
                    if(d_job_buffer->pop_front(job))
                    {
                        (*job)();
                        d_finished_label.fetch_add(1);

                        delete job;
                        job = nullptr;
                    }
                    else
                    {
                        std::unique_lock<std::mutex> lock(d_wake_mutex);
                        d_wake_condition.wait(lock);
                    }
                }
            });

            worker.detach();
        }
    }

    void poll()
    {
        d_wake_condition.notify_one();
        std::this_thread::yield();
    }

    void stop()
    {
        d_is_close_requested = true;
    }

    void execute(const std::function<void()>& func)
    {
        ++d_current_label;

        while (!d_job_buffer->push_back(new Job(func)))
            poll();

        d_wake_condition.notify_one();
    }

    inline bool is_busy()
    {
        return d_finished_label.load() < d_current_label;
    }

    void wait()
    {
        while (is_busy())
            poll();
    }

    void dispatch(uint32_t job_count, uint32_t group_size, const std::function<void(const JobDispatchArgs& args)>& func)
    {
        if(job_count == 0 || group_size == 0)
            return;

        const uint32_t group_count = (job_count + group_size - 1) / group_size;

        d_current_label += group_count;

        for(uint32_t i = 0; i < group_count; ++i)
        {
            const auto& job_group = new Job([job_count, group_size, func, i]()
            {
                const uint32_t group_job_offset = i * group_size;
                const uint32_t group_job_end = std::min(group_job_offset + group_size, job_count);

                JobDispatchArgs args;
                args.groupIndex = i;

                for (uint32_t k = group_job_offset; k < group_job_end; ++k)
                {
                    args.jobIndex = k;
                    func(args);
                }
            });

            while(!d_job_buffer->push_back(job_group))
                poll();

            d_wake_condition.notify_one();
        }
    }

private:
    bool d_is_close_requested = false;
    std::condition_variable d_wake_condition;

    std::mutex d_wake_mutex;

    std::unique_ptr<ThreadSafeRingBuffer<Job*, pool_size>> d_job_buffer;

    uint64_t d_current_label = 0;

    std::atomic<uint64_t> d_finished_label;

    uint32_t d_num_hardware_threads = 0;
};

#endif // SI_SIGRUN_JobSystem