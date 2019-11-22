

#include "sigrun/plugin/PythonInvoker.hpp"
#include "Region.hpp"

Region::Region(const std::vector<glm::vec3> &contour, const bp::object& effect):
    d_contour(contour),
    d_effect(effect),
    uprt(std::make_unique<RegionTransform>()),
    uppi(std::make_unique<PythonInvoker>())
{
    set_aabb(contour);

    uprm = std::make_unique<RegionMask>(1920, 1080, d_contour, d_aabb);
}

Region::~Region()
{

}

const std::vector<glm::vec3>& Region::aabb()
{
    return d_aabb;
}

const std::vector<glm::vec3>& Region::contour()
{
    return d_contour;
}

void Region::set_aabb(const std::vector<glm::vec3> &contour)
{
    int x_min = std::numeric_limits<int>::max();
    int x_max = std::numeric_limits<int>::min();
    int y_min = std::numeric_limits<int>::max();
    int y_max = std::numeric_limits<int>::min();

    for (auto &vertex : contour)
    {
        x_max = vertex.x > x_max ? vertex.x : x_max;
        y_max = vertex.y > y_max ? vertex.y : y_max;
        x_min = vertex.x < x_min ? vertex.x : x_min;
        y_min = vertex.y < y_min ? vertex.y : y_min;
    }

    glm::vec3 tlc(x_min, y_min, 1), blc(x_min, y_max, 1), brc(x_max, y_max, 1), trc(x_max, y_min, 1);

    d_aabb = std::vector<glm::vec3>
    {
        tlc, blc, brc, trc
    };
}

int Region::on_enter(bp::object& other)
{
    int fail = uppi->invoke_collision_event_function(d_effect, other, "on_enter");

    if(!fail)
    {
        // update here
        // requires data structure or defined variables exposed via module which can be set
    }

    return fail;
}

int Region::on_continuous(bp::object& other)
{

    int fail =  uppi->invoke_collision_event_function(d_effect, other, "on_continuous");

    if(!fail)
    {
        // update here
    }

    return fail;
}

int Region::on_leave(bp::object& other)
{
    int fail =  uppi->invoke_collision_event_function(d_effect, other, "on_leave");

    if(!fail)
    {
        // update here
    }

    return fail;
}
