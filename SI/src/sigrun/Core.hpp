

#ifndef SI_SIGRUN_CORE_HPP
#define SI_SIGRUN_CORE_HPP

#include <boost/python.hpp>
#include <memory>
#include <unordered_map>
#include <sigrun/rendering/IRenderEngine.hpp>
#include <sigrun/context/Context.hpp>
#include "SIObject.hpp"

/**
\brief namespace shortening for python object integration
*/
namespace bp = boost::python;

/**
\class Core derived from SIObject
\brief Central Core class registered as SIObject
\details
    This class initiates all subsystems required for the SIGRun environment.
    This class collects all available pulgins first.
    Second, it launches the SI context and other subsystems.
    This class is registered as SIObject meta type.
    This class conctructor is declared private to disable use by external application programmers.
    Therefore, the friend keyword is used to internally expose the class.
*/
class Core: public SIObject
{ SIGRUN
public:
    ~Core();

    void start(char** argv, int argc, IRenderEngine* ire);
    void stop();

protected:
    Core();

    void retrieve_available_plugins(std::unordered_map<std::string, std::unique_ptr<bp::object>>& plugins, const std::string& plugin_path);

    friend class SIGRun;
    friend class SIGRunTest;
    friend class SIGRunCoreTest;

protected:
};
#endif //SI_SIGRUN_CORE_HPP
