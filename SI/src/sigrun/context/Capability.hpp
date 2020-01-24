

#ifndef SITEST_CAPABILITY_HPP
#define SITEST_CAPABILITY_HPP


#include <string>
#include <map>
#include <memory>
#include <sigrun/region/Region.hpp>
#include <sigrun/SIObject.hpp>

class Capability: SIObject
{
public:
    Capability();
    ~Capability();

    const std::map<std::string, int>& capabilities() const;

    void add_capabilities(const bp::object& o);
    void add_capability(const std::string& name);
    void remove_capability(const std::string& name);

    int num_capabilities() const;
    int consecutive_capability_id() const;

    static std::string __test1__;
    static std::string __test2__;

private:
    std::map<std::string, int> d_capabilities;
    unsigned int d_num_capabilities;
    unsigned int d_consecutive_capability_id;



    friend class CollisionManager;
};


#endif //SITEST_CAPABILITY_HPP