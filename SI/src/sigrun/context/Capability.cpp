

#include "Capability.hpp"

namespace bp = boost::python;

Capability::Capability():
    d_consecutive_capability_id(0),
    d_num_capabilities(0)
{

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
    const bp::dict& recv = bp::extract<bp::dict>(o.attr("cap_recv"));
    const bp::dict& emit = bp::extract<bp::dict>(o.attr("cap_emit"));

    const bp::list& items_recv = recv.items();
    for(int i = 0; i < bp::len(items_recv); ++i)
        add_capability(bp::extract<std::string>(items_recv[i][0]));

    const bp::list& items_emit = emit.items();

    for(int i = 0; i < bp::len(items_emit); ++i)
        add_capability(bp::extract<std::string>(items_emit[i][0]));
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
