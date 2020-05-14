

#include <sigrun/context/Context.hpp>
#include "PySIStartup.hpp"

PySIStartup::PySIStartup() = default;
PySIStartup::~PySIStartup() = default;

bp::tuple PySIStartup::context_dimensions()
{
    return bp::make_tuple(Context::SIContext()->width(), Context::SIContext()->height());
}

void PySIStartup::create_region_by_id(const bp::list &contour, int effect_type, bp::dict &kwargs)
{
    std::vector<glm::vec3> _contour;
    _contour.reserve(bp::len(contour));

       for(uint32_t i = 0; i < bp::len(contour); ++i)
       {
           float x = bp::extract<float>(contour[i][0]);
           float y = bp::extract<float>(contour[i][1]);

           _contour.emplace_back(x, y, 1);
       }

    Context::SIContext()->register_new_region_via_type(_contour, effect_type, kwargs);
}
