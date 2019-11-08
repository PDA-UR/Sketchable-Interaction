

#ifndef SI_SIGRUN_CORE_HPP
#define SI_SIGRUN_CORE_HPP

#include <boost/python.hpp>
#include <memory>
#include <unordered_map>

#include "MetaType.hpp"

namespace bp = boost::python;

class Core: public SIMetaType
{
public:
    ~Core();

    void start();
    void stop();
    void retrieve_available_plugins(std::unordered_map<std::string, std::shared_ptr<bp::object>>& plugins, const std::string& plugin_path);

private:
    Core();

    friend class SIGRun;
    friend class SIGRunTest;
    friend class SIGRunCoreTest;


protected:
};
#endif //SI_SIGRUN_CORE_HPP
