//
// Created by juergen on 05/04/19.
//

#include "region.h"

namespace bp = boost::python;

void SIRegion::on_enter()
{
    pyregion.attr("on_enter")();
}

void SIRegion::on_continuous()
{
    pyregion.attr("on_continuous")();
}

void SIRegion::on_leave()
{
    pyregion.attr("on_leave")();
}


void region::set_pyregion(bp::object pyobject)
{
    pyregion = pyobject;
}

class PyRegion : public region, public bp::wrapper<region>
{
    void on_enter() override
    {
        get_override("on_enter")();
    }

    void on_continuous() override
    {
        get_override("on_continuous")();
    }

    void on_leave() override
    {
        get_override("on_leave")();
    }
};

BOOST_PYTHON_MODULE(libengine_scripting_lib)
{
    bp::class_<PyRegion, boost::noncopyable>("Region", bp::init<>())
            .def("on_enter", &region::on_enter)
            .def("on_continuous", &region::on_continuous)
            .def("on_leave", &region::on_leave)
            ;
}