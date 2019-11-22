

#ifndef SITEST_REGION_HPP
#define SITEST_REGION_HPP

#include <boost/python.hpp>

#include <memory>
#include <glm/glm.hpp>

#include "RegionMask.hpp"
#include "RegionTransform.hpp"
#include "sigrun/plugin/PythonInvoker.hpp"

namespace bp = boost::python;

class Region
{
public:
    Region(const std::vector<glm::vec3>& contour, const bp::object& effect);
    ~Region();

    const std::vector<glm::vec3>& aabb();
    const std::vector<glm::vec3>& contour();

    void set_aabb(const std::vector<glm::vec3>& contour);

    int on_enter(bp::object& other);
    int on_continuous(bp::object& other);
    int on_leave(bp::object& other);

private:
    std::vector<glm::vec3> d_contour;
    std::vector<glm::vec3> d_aabb;

    std::unique_ptr<RegionMask> uprm;
    std::unique_ptr<RegionTransform> uprt;
    std::unique_ptr<PythonInvoker> uppi;
    bp::object d_effect;
};


#endif //SITEST_REGION_HPP
