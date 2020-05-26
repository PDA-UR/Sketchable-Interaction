

#ifndef SI_SIGRUN_JobSystem
#define SI_SIGRUN_JobSystem

#include <sigrun/SIObject.hpp>
#include "Job.hpp"
#include <condition_variable>
#include <thread>
#include <memory>
#include <sigrun/parallel/helpers/ThreadSafeRingBuffer.hpp>


template <typename JobFuncType, int pool_size>
class JobSystem : public SIObject
{ SIGRUN

public:
    ~JobSystem() = default;

    JobSystem():
        d_num_hardware_threads(std::thread::hardware_concurrency() - 1),
        d_job_buffer(std::make_unique<ThreadSafeRingBuffer<std::shared_ptr<Job>, pool_size>>())
    {
        DEBUG(std::thread::hardware_concurrency());

        d_finished_label.store(0);

        for(uint32_t i = 0; i < d_num_hardware_threads; ++i)
        {
            std::thread worker([&]
            {
                std::shared_ptr<Job> job;

                while (true)
                {
                    if(d_job_buffer->pop_front(job))
                    {
                        (*job)();
                        d_finished_label.fetch_add(1);
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

    void execute(const std::shared_ptr<Job>& job)
    {
        ++d_current_label;

        while (!d_job_buffer->push_back(job))
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

    void dispatch(uint32_t job_count, uint32_t group_size, const std::shared_ptr<Job>& job)
    {
        if(job_count == 0 || group_size == 0)
            return;

        const uint32_t group_count = (job_count + group_size - 1) / group_size;

        d_current_label += group_count;

        for(uint32_t i = 0; i < group_count; ++i)
        {
            const auto& job_group = std::make_shared<Job>([job_count, group_size, job, i]()
            {
                const uint32_t group_job_offset = i * group_size;
                const uint32_t group_job_end = std::min(group_job_offset + group_size, job_count);

                JobDispatchArgs args;
                args.groupIndex = i;

                for (uint32_t k = group_job_offset; k < group_job_end; ++k)
                {
                    args.jobIndex = k;
                    (*job)(args);
                }
            });

            while(!d_job_buffer->push_back(job_group))
                poll();

            d_wake_condition.notify_one();
        }
    }

private:
    std::condition_variable d_wake_condition;

    std::mutex d_wake_mutex;

    std::unique_ptr<ThreadSafeRingBuffer<std::shared_ptr<Job>, pool_size>> d_job_buffer;

    uint64_t d_current_label = 0;

    std::atomic<uint64_t> d_finished_label;

    uint32_t d_num_hardware_threads = 0;
};

#endif // SI_SIGRUN_JobSystem