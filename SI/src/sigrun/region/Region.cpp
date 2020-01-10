

#include "sigrun/plugin/PythonInvoker.hpp"
#include "Region.hpp"

#include <utility>
#include "RegionResampler.hpp"
#include <sigrun/log/Log.hpp>
#include <sigrun/context/Link.hpp>
#include <sigrun/util/UUID.hpp>

namespace bp = boost::python;

Region::Region(const std::vector<glm::vec3> &contour, std::shared_ptr<bp::object> effect):
    d_effect(std::move(effect)),
    uprt(std::make_unique<RegionTransform>()),
    uppi(std::make_unique<PythonInvoker>()),
    d_is_transformed(true) // should be false
{SIGRUN
    RegionResampler::resample(d_contour, contour);

    set_aabb();

    uprm = std::make_unique<RegionMask>(1920, 1080, d_contour, d_aabb);

    d_uuid = std::string(UUID::uuid());

    d_name = PythonInvoker().invoke_extract_attribute<std::string>(*d_effect, "name");
    d_name = d_name == "" ? "custom": d_name;

    d_texture_path_default = PythonInvoker().invoke_extract_attribute<std::string>(*d_effect, "texture_path");
    d_texture_path_default = d_texture_path_default == "" ? "src/siren/res/textures/placeholder.png": d_texture_path_default;
}

Region::~Region()
{

}

void Region::move(int x, int y)
{
    uprm->move(glm::vec2(x, y));
    uprt->update(glm::vec2(x, y));
    set_aabb();
}

const bool Region::is_transformed() const
{
    return d_is_transformed;
}

void Region::set_is_transformed(bool b)
{
    d_is_transformed = b;
}

const std::string Region::uuid() const
{
    return d_uuid;
}

bp::object& Region::effect()
{
    return *d_effect;
}

const std::unique_ptr<RegionMask> &Region::mask() const
{
    return uprm;
}

const std::vector<glm::vec3>& Region::aabb()
{
    return d_aabb;
}

const std::vector<glm::vec3>& Region::contour()
{
    return d_contour;
}

void Region::set_aabb()
{
    int x_min = std::numeric_limits<int>::max();
    int x_max = std::numeric_limits<int>::min();
    int y_min = std::numeric_limits<int>::max();
    int y_max = std::numeric_limits<int>::min();

    for (auto &vertex : d_contour)
    {
        glm::vec3 v = vertex * transform();
        v /= v.z;

        x_max = v.x > x_max ? v.x : x_max;
        y_max = v.y > y_max ? v.y : y_max;
        x_min = v.x < x_min ? v.x : x_min;
        y_min = v.y < y_min ? v.y : y_min;
    }

    glm::vec3 tlc(x_min, y_min, 1), blc(x_min, y_max, 1), brc(x_max, y_max, 1), trc(x_max, y_min, 1);

    d_aabb = std::vector<glm::vec3>
    {
        tlc, blc, brc , trc
    };
}

const std::string &Region::texture_path() const
{
    return d_texture_path_default;
}

const glm::mat3x3& Region::transform() const
{
    return uprt->transform();
}

int Region::on_enter(bp::object& colliding_effect)
{
    // call unique ptr to PythonInvoker object
    int success = uppi->invoke_collision_event_function(*d_effect, colliding_effect, "on_enter");

    if(success)
    {
        // region update functionality
    }

    return success;
}

int Region::on_continuous(bp::object& other)
{

    int success =  uppi->invoke_collision_event_function(*d_effect, other, "on_continuous");

    if(success)
    {
        // update here
    }

    return success;
}

int Region::on_leave(bp::object& other)
{
    int success =  uppi->invoke_collision_event_function(*d_effect, other, "on_leave");

    if(success)
    {
        // update here
    }

    return success;
}

void Region::__set_position__(int x, int y, const std::string& event_uuid)
{
    // safeguards for runtime

    if (std::find(d_link_events.begin(), d_link_events.end(), event_uuid) == d_link_events.end())
    {
        // needs to call function from plugin in the end

        __x__ = x;
        __y__ = y;

        d_link_events.push_back(event_uuid);

        Q_EMIT __position__(x, y, event_uuid);
    }
    else
        d_link_events.erase(std::find(d_link_events.begin(), d_link_events.end(), event_uuid));
}
