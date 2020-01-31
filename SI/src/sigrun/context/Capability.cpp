

#include "Capability.hpp"
#include <pysi/SuperEffect.hpp>
#include <memory>
#include <sigrun/plugin/PythonInvoker.hpp>

namespace bp = boost::python;

std::string Capability::__test1__ = "TEST1";
std::string Capability::__test2__ = "TEST2";

Capability::Capability():
    d_consecutive_capability_id(0),
    d_num_capabilities(0)
{SIGRUN
}

Capability::~Capability()
{

}

const std::map<std::string, int> &Capability::capabilities() const
{
    return d_capabilities;
}

void Capability::add_capabilities(const bp::object &o)
{
    if(!o.is_none())
    {
        HANDLE_PYTHON_CALL(
                auto obj = std::make_shared<PySIEffect>(bp::extract<PySIEffect>(o));

                obj->cap_collision_emit();
                obj->cap_collision_recv();

                for(auto& [key, value]: obj->cap_collision_emit())
                    add_capability(key);

                for(auto& [key, value]: obj->cap_collision_recv())
                    add_capability(key);
        )
    }
}

void Capability::add_capability(const std::string &name)
{
    if(d_capabilities.find(name) == d_capabilities.end())
    {
        d_capabilities.insert({ name, ++d_consecutive_capability_id });
        ++d_num_capabilities;
    }
}

void Capability::remove_capability(const std::string &name)
{
    if(d_capabilities.find(name) != d_capabilities.end())
    {
        d_capabilities.erase(name);
        --d_num_capabilities;
    }
}

int Capability::num_capabilities() const
{
    return d_num_capabilities;
}

int Capability::consecutive_capability_id() const
{
    return d_consecutive_capability_id;
}
