

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
    qRegisterMetaType<bp::object>("bp::object");
    qRegisterMetaType<bp::list>("bp::list");

    RegionResampler::resample(d_contour, contour);

    set_aabb();

    uprm = std::make_unique<RegionMask>(1920, 1080, d_contour, d_aabb);

    d_uuid = std::string(UUID::uuid());

    d_name = PythonInvoker().invoke_extract_attribute<std::string>(*d_effect, "name");
    d_name = d_name == "" ? "custom": d_name;

    d_texture_path_default = PythonInvoker().invoke_extract_attribute<std::string>(*d_effect, "texture_path");
    d_texture_path_default = d_texture_path_default == "" ? "src/siren/res/textures/placeholder.png": d_texture_path_default;


    if(!d_effect->is_none())
    {
        const bp::dict& de = bp::extract<bp::dict>(d_effect->attr("cap_link_emit"));
        const bp::list& items_de = de.keys();

        for(int i = 0; i < bp::len(items_de); ++i)
            d_attributes_emit.push_back(bp::extract<std::string>(items_de[i])());



        const bp::dict& dr = bp::extract<bp::dict>(d_effect->attr("cap_link_recv"));
        const bp::list& items_dr = dr.keys();

        for(int i = 0; i < bp::len(items_dr); i++)
        {
            const bp::object& key = items_dr[i];
            const bp::dict& inner_dr = bp::extract<bp::dict>(dr[key]);
            const bp::list& items_inner_dr = inner_dr.keys();

            const std::string key_str = bp::extract<std::string>(key)();

            d_attributes_recv.insert({key_str, std::vector<std::string>()});

            for(int k = 0; k < bp::len(items_inner_dr); k++)
                d_attributes_recv[key_str].push_back(bp::extract<std::string>(items_inner_dr[k])());
        }

        Print::print("emits:");
        for(auto& se: d_attributes_emit)
        {
            Print::print(se);
        }

        Print::print("\t");

        Print::print("receives:");
        for(auto& [key, val]: d_attributes_recv)
        {
            Print::print(key);
            Print::print(val);
        }
    }
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

void Region::LINK_SLOT(const std::string& uuid, const std::string& source_cap, const std::string& dest_cap, const bp::list& py_list)
{
    // check if region is eligible to receive link data

    auto tuple = std::make_tuple(uuid, dest_cap);

    if (std::find(d_link_events.begin(), d_link_events.end(), tuple) == d_link_events.end())
    {
        register_link_event(tuple);
//        uppi->invoke_linking_event_function(*d_effect, cap, py_list);

        d_effect->attr("cap_link_recv")[source_cap][dest_cap](py_list);

        // retrieve updated args for py_list and overwrite py_list
        // fire new signal according to changed values

        Q_EMIT LINK_SIGNAL(uuid, source_cap, dest_cap, py_list);
    }
    else
    {
        d_link_events.erase(std::find(d_link_events.begin(), d_link_events.end(), tuple));
    }
}

const std::vector<std::tuple<std::string, std::string>>& Region::link_events() const
{
    return d_link_events;
}

void Region::register_link_event(const std::string &uuid, const std::string &dest_attribute)
{
    register_link_event(std::make_tuple(uuid, dest_attribute));
}

void Region::register_link_event(const std::tuple<std::string, std::string>& link_event)
{
    if (std::find(d_link_events.begin(), d_link_events.end(), link_event) == d_link_events.end())
    {
        d_link_events.push_back(link_event);
    }
}
