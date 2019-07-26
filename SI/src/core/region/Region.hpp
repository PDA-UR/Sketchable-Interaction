

#ifndef SI_REGION_HPP
#define SI_REGION_HPP

#include <boost/python.hpp>

namespace bp = boost::python;

class Region
{
public:
    Region();
    explicit Region(int id);
    ~Region();

    void set_effect(bp::object effect);

    const std::string& name() const;
    const int id() const;

    void on_enter();
    void on_continuous();
    void on_leave();

private:
    bp::object d_effect;
    std::string d_name;
    int d_id;
};


#endif //SI_REGION_HPP
