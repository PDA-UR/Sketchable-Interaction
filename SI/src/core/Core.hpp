

#ifndef SI_CORE_HPP
#define SI_CORE_HPP


#include <resource/Scripting.hpp>

class Core
{
public:
    Core() = default;
    ~Core() = default;

    void initialize();
    void start();
    void stop();

private:
};


#endif //SI_CORE_HPP
