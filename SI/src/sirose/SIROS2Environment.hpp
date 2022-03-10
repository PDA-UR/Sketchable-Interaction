

#ifndef SI_SIGRUN_SIROS2Environment
#define SI_SIGRUN_SIROS2Environment

#include <SI/SI.hpp>
#include "Subscriber.hpp"
#include "Publisher.hpp"
#include <thread>

class SIROS2Environment: public IPhysicalEnvironment, public SIObject
{ SIROSE
public:
    SIROS2Environment();
    ~SIROS2Environment();

    void start(int argc, char** argv) override;
    void stop() override;

    void send(const std::string &msg) const override;

private:

    std::shared_ptr<Subscriber> sub;
    std::shared_ptr<Publisher> pub;
};


#endif // SI_SIGRUN_SIROS2Environment