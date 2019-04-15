
#ifndef ENGINE_SCRIPTING_SUPPORT_TEST_REGION_H
#define ENGINE_SCRIPTING_SUPPORT_TEST_REGION_H

#include <boost/python.hpp>

namespace bp = boost::python;


class region
{
public:
    region() = default;
    ~region() = default;

    virtual void on_enter() = 0;
    virtual void on_continuous() = 0;
    virtual void on_leave() = 0;

    void set_pyregion(bp::object pyobject);

    bp::object pyregion;
};

class SIRegion : public region
{
public:
    SIRegion() = default;
    ~SIRegion() = default;

    void on_enter() override;
    void on_continuous() override;
    void on_leave() override;
};


#endif //ENGINE_SCRIPTING_SUPPORT_TEST_REGION_H
