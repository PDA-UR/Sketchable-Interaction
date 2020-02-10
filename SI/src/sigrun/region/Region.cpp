

#include "sigrun/plugin/PythonInvoker.hpp"
#include "Region.hpp"

#include "RegionResampler.hpp"
#include <sigrun/log/Log.hpp>
#include <sigrun/util/UUID.hpp>
#include <sigrun/context/Context.hpp>

namespace bp = boost::python;

Region::Region(const std::vector<glm::vec3> &contour, const bp::object& effect, int mask_width, int mask_height):
    uprt(std::make_unique<RegionTransform>()),
    d_is_transformed(false),
    d_link_events(20)
{SIGRUN
    HANDLE_PYTHON_CALL(
            d_effect = std::make_shared<bp::object>(bp::import("copy").attr("deepcopy")(effect));
    )

    qRegisterMetaType<bp::object>("bp::object");
    qRegisterMetaType<bp::tuple>("bp::tuple");

    d_uuid = std::string(_UUID_);
    d_effect->attr("_uuid") = d_uuid;

    HANDLE_PYTHON_CALL(
        d_py_effect = std::shared_ptr<PySIEffect>(new PySIEffect(bp::extract<PySIEffect>(*d_effect)));
    )

    RegionResampler::resample(d_contour, contour);

    set_aabb();

    if(mask_width == 0 && mask_height == 0)
    {
        mask_width = Context::SIContext()->width();
        mask_height = Context::SIContext()->height();
    }

    uprm = std::make_unique<RegionMask>(mask_width, mask_height, d_contour, d_aabb);

    d_name = d_py_effect->name();
    d_name = d_name.empty() ? "custom": d_name;

    d_width = d_py_effect->width();
    d_height = d_py_effect->height();

    d_qml_path_default = d_py_effect->qml_path();
    d_type = d_py_effect->effect_type();

    HANDLE_PYTHON_CALL (
            for(auto& [key, value]: d_py_effect->attr_link_emit())
                d_attributes_emit.push_back(key);

            for(auto& [key, value]: d_py_effect->attr_link_recv())
            {
                d_attributes_recv.insert({key, std::vector<std::string>()});

                for(auto& [key2, value2]: value)
                    d_attributes_recv[key].push_back(key2);
            }

            for(auto& [key, value]: d_py_effect->cap_collision_emit())
                d_collision_caps_emit.push_back(key);

            for(auto& [key, value]: d_py_effect->cap_collision_recv())
                d_collision_caps_recv.push_back(key);
    )
}

Region::~Region()= default;

void Region::move(int x, int y)
{
    if(x != 0 || y != 0)
    {
        int prev_x = d_aabb[0].x;
        int prev_y = d_aabb[0].y;

        glm::vec2 center(d_aabb[0].x + d_aabb[3].x - d_aabb[0].x, d_aabb[0].y + d_aabb[1].y - d_aabb[0].y);

        uprt->update(glm::vec2(x, y), 0, 1, center);

        set_aabb();

        int new_x = d_aabb[0].x;
        int new_y = d_aabb[0].y;

        int delta_x = new_x - prev_x;
        int delta_y = new_y - prev_y;

        uprm->move(glm::vec2(delta_x, delta_y));

        set_is_transformed(true);
    }
    else
        set_is_transformed(false);
}

bool Region::is_transformed() const
{
    return d_is_transformed;
}

void Region::set_is_transformed(bool b)
{
    d_is_transformed = b;
}

const std::string& Region::uuid() const
{
    return d_uuid;
}

PySIEffect &Region::effect()
{
    return *d_py_effect;
}

bp::object &Region::raw_effect()
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

const std::string &Region::qml_path() const
{
    return d_qml_path_default;
}

const glm::mat3x3& Region::transform() const
{
    return uprt->transform();
}

int Region::on_enter(PySIEffect& colliding_effect)
{
    return handle_collision_event("on_enter", colliding_effect);
}

int Region::on_continuous(PySIEffect& colliding_effect)
{
    return handle_collision_event("on_continuous", colliding_effect);
}

int Region::on_leave(PySIEffect& colliding_effect)
{
    return handle_collision_event("on_leave", colliding_effect);
}

void Region::LINK_SLOT(const std::string& uuid, const std::string& source_cap, const bp::tuple& args)
{
    auto event = std::make_tuple(uuid, source_cap);

    if(!is_link_event_registered(event))
    {
        register_link_event(event);

        for(auto& recv: d_attributes_recv[source_cap])
        {
            HANDLE_PYTHON_CALL(d_py_effect->attr_link_recv()[source_cap][recv](*args);)

            if(std::find(d_attributes_emit.begin(), d_attributes_emit.end(), recv) != d_attributes_emit.end())
            {
                HANDLE_PYTHON_CALL(
                    const bp::tuple &args2 = bp::extract<bp::tuple>(d_py_effect->attr_link_emit()[recv]());

                    Q_EMIT LINK_SIGNAL(uuid, recv, args2);
                )
            }
        }
    }
}

void Region::register_link_event(const std::string &uuid, const std::string &attribute)
{
    register_link_event(std::make_tuple(uuid, attribute));
}

void Region::register_link_event(const std::tuple<std::string, std::string>& link_event)
{
    d_link_events << link_event;
}

bool Region::is_link_event_registered(const std::string &uuid, const std::string &attribute)
{
    return is_link_event_registered(std::make_tuple(uuid, attribute));
}

bool Region::is_link_event_registered(const std::tuple<std::string, std::string> &link_event)
{
    return d_link_events & link_event;
}

void Region::set_name(const std::string &name)
{
    d_name = name;
}

const std::string &Region::name() const
{
    return d_name;
}

const int Region::type() const
{
    return d_type;
}

const int Region::width() const
{
    return d_width;
}

const int Region::height() const
{
    return d_height;
}

void Region::update()
{
    HANDLE_PYTHON_CALL(d_py_effect = std::shared_ptr<PySIEffect>(new PySIEffect(bp::extract<PySIEffect>(*d_effect)));)

    // apply new data

    int x = d_py_effect->x();
    int y = d_py_effect->y();

    move(x, y);

    d_attributes_emit.clear();
    d_attributes_recv.clear();
    d_collision_caps_emit.clear();
    d_collision_caps_recv.clear();

    HANDLE_PYTHON_CALL (
            for(auto& [key, value]: d_py_effect->attr_link_emit())
                d_attributes_emit.push_back(key);

            for(auto& [key, value]: d_py_effect->attr_link_recv())
            {
                d_attributes_recv.insert({key, std::vector<std::string>()});

                for(auto& [key2, value2]: value)
                    d_attributes_recv[key].push_back(key2);
            }

            for(auto& [key, value]: d_py_effect->cap_collision_emit())
                d_collision_caps_emit.push_back(key);

            for(auto& [key, value]: d_py_effect->cap_collision_recv())
                d_collision_caps_recv.push_back(key);
    )

    if(d_py_effect->effect_type() == SI_TYPE_CANVAS)
    {
        Context::SIContext()->region_manager()->set_partial_regions(d_py_effect->partial_region_contours());

        if(!d_py_effect->regions_for_registration().empty())
        {
            for(auto& candidate: d_py_effect->regions_for_registration())
            {
                Context::SIContext()->register_new_region(d_py_effect->partial_region_contours()[candidate], candidate);

                HANDLE_PYTHON_CALL(
                        bp::object temp = d_effect->attr("__partial_regions__");
                        bp::delitem(temp, bp::object(candidate));
                        d_effect->attr("__partial_regions__") = temp;
                )
            }

            HANDLE_PYTHON_CALL (d_effect->attr("__regions_for_registration__") = bp::list();)
        }
    }
    else
        Context::SIContext()->update_linking_relations(d_py_effect->link_relations(), d_uuid);
}

const QMap<QString, QVariant>& Region::data() const
{
    return d_py_effect->data();
}

void Region::set_color(const glm::vec4 &color)
{
    d_py_effect->__set_color__(std::vector<int>{(int) color.r, (int) color.g, (int) color.b, (int) color.a});
}

const glm::vec4 Region::color() const
{
    auto& color = d_py_effect->__color__();

    return glm::vec4(color[0], color[1], color[2], color[3]);
}

const std::vector<std::string> &Region::collision_caps_emit() const
{
    return d_collision_caps_emit;
}

const std::vector<std::string> &Region::collision_caps_recv() const
{
    return d_collision_caps_recv;
}

int Region::handle_collision_event(const std::string &function_name, PySIEffect &colliding_effect)
{
    HANDLE_PYTHON_CALL(
        for (auto&[key, value]: colliding_effect.cap_collision_emit())
        {
            if (std::find(d_collision_caps_recv.begin(), d_collision_caps_recv.end(), key) != d_collision_caps_recv.end())
            {
                const bp::object &t = colliding_effect.cap_collision_emit()[key][function_name](*d_effect);

                if (t.is_none())
                    return bp::extract<int>(d_py_effect->cap_collision_recv()[key][function_name]());
                else
                {
                    if (bp::extract<bp::tuple>(t).check())
                        return bp::extract<int>(d_py_effect->cap_collision_recv()[key][function_name](*t));
                    else
                        return bp::extract<int>(d_py_effect->cap_collision_recv()[key][function_name](t));
                }
            }
        }
    )

    return 2;
}
