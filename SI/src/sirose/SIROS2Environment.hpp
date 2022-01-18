

#ifndef SI_SIGRUN_SIROS2Environment
#define SI_SIGRUN_SIROS2Environment

#include <SI/SI.hpp>
#include "Subscriber.hpp"
#include <thread>

class SIROS2Environment: public IROS2Environment, public SIObject
{ SIROSE
public:
    SIROS2Environment();
    ~SIROS2Environment();

    void start(int argc, char** argv) override;
    void stop() override;

private:

    std::shared_ptr<Subscriber> sub;

    std::thread spin_thread;
};


#endif // SI_SIGRUN_SIROS2Environment