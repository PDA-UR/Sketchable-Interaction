

#ifndef SI_SIGRUN_CORE_HPP
#define SI_SIGRUN_CORE_HPP

class Core
{
public:
    ~Core();

private:
    Core();

    void start();

    friend class SIGRun;
    friend class SIGRunTest;
    friend class SIGRunCoreTest;
protected:
};
#endif //SI_SIGRUN_CORE_HPP
