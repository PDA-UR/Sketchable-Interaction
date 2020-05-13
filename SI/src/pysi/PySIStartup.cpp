

#include <sigrun/context/Context.hpp>
#include "PySIStartup.hpp"

PySIStartup::PySIStartup() = default;
PySIStartup::~PySIStartup() = default;

namespace PySIStart
{
    int test()
    {
        Context::SIContext()->test();
        return Context::SIContext()->width();
    }
}
