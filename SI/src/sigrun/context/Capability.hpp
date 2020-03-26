

#ifndef SITEST_CAPABILITY_HPP
#define SITEST_CAPABILITY_HPP


#include <string>
#include <map>
#include <memory>
#include <sigrun/SIObject.hpp>
#include <boost/python.hpp>

namespace bp = boost::python;

class Capability: public SIObject
{ SIGRUN
public:
    Capability();
    ~Capability();

    const std::map<std::string, uint64_t>& capabilities() const;

    void add_capabilities(const bp::object& o);
    void add_capability(const std::string& name);
    void remove_capability(const std::string& name);

    uint32_t num_capabilities() const;
    uint64_t consecutive_capability_id() const;

    static std::string __test1__;
    static std::string __test2__;

private:
    std::map<std::string, uint64_t> d_capabilities;
    uint32_t d_num_capabilities;
    uint64_t d_consecutive_capability_id;
};


#endif //SITEST_CAPABILITY_HPP
