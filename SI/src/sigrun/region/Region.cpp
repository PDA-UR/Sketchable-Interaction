

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
    qRegisterMetaType<bp::object>("bp::object");
    qRegisterMetaType<bp::tuple>("bp::tuple");

    RegionResampler::resample(d_contour, contour);

    set_aabb();

    d_last_x = 0;
    d_last_y = 0;

    HANDLE_PYTHON_CALL(
            d_effect = std::make_shared<bp::object>(bp::import("copy").attr("deepcopy")(effect));
            d_effect->attr("__init__")(d_contour, d_aabb, std::string(_UUID_));
            d_py_effect = std::shared_ptr<PySIEffect>(new PySIEffect(bp::extract<PySIEffect>(*d_effect)));
    )

    if(mask_width == 0 && mask_height == 0)
    {
        mask_width = Context::SIContext()->width();
        mask_height = Context::SIContext()->height();
    }

    uprm = std::make_unique<RegionMask>(mask_width, mask_height, d_contour, d_aabb);
}

Region::~Region()= default;

void Region::move(int x, int y)
{
    if(x != 0 || y != 0)
    {
        glm::vec2 center(d_last_x + d_aabb[0].x + (d_aabb[3].x - d_aabb[0].x), d_last_y + d_aabb[0].y + (d_aabb[1].y - d_aabb[0].y));

        uprt->update(glm::vec2(x, y), 0, 1, center);

        int delta_x = x - d_last_x;
        int delta_y = y - d_last_y;

        d_last_x = x;
        d_last_y = y;

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
    return d_py_effect->uuid();
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

    for (auto &v: d_contour)
    {
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
    return d_py_effect->qml_path();
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

        for(auto& [key, value]: d_py_effect->attr_link_recv()[source_cap])
        {
            HANDLE_PYTHON_CALL(
                d_py_effect->attr_link_recv()[source_cap][key](*args);

                if(d_py_effect->attr_link_emit().find(key) != d_py_effect->attr_link_emit().end())
                    Q_EMIT LINK_SIGNAL(uuid, key, bp::extract<bp::tuple>(d_py_effect->attr_link_emit()[key]()));
            )
        }
    }
}

void Region::REGION_DATA_CHANGED_SLOT(const QMap<QString, QVariant>& data)
{
    d_effect->attr("has_data_changed") = false;
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

const std::string &Region::name() const
{
    return d_py_effect->name();
}

const int Region::type() const
{
    return d_py_effect->effect_type();
}

const int Region::width() const
{
    return d_py_effect->width();
}

const int Region::height() const
{
    return d_py_effect->height();
}

void Region::update()
{
    HANDLE_PYTHON_CALL(d_py_effect = std::shared_ptr<PySIEffect>(new PySIEffect(bp::extract<PySIEffect>(*d_effect)));)

    if(d_py_effect->has_shape_changed())
    {
//        d_effect->attr("has_shape_changed") = false;

//        d_contour = d_py_effect->contour();

//        set_aabb();

//        d_effect->attr("aabb") = d_aabb;

//        uprm->rebuild(d_contour, d_aabb);
//        DEBUG("HERE");
    }

    move(d_py_effect->x(), d_py_effect->y());

    process_canvas_specifics();
    process_linking_relationships();
}

void Region::process_canvas_specifics()
{
    if(d_py_effect->effect_type() == SI_TYPE_CANVAS)
    {
        Context::SIContext()->region_manager()->set_partial_regions(d_py_effect->partial_region_contours());

        if(!d_py_effect->regions_for_registration().empty())
        {
            for(auto& candidate: d_py_effect->regions_for_registration())
            {
                Context::SIContext()->register_new_region(d_py_effect->partial_region_contours()[candidate], candidate);

                HANDLE_PYTHON_CALL(bp::delitem(d_effect->attr("__partial_regions__"), bp::object(candidate));)
            }

            HANDLE_PYTHON_CALL(d_effect->attr("registered_regions").attr("clear")();)
        }
    }
}

void Region::process_linking_relationships()
{
    if(d_py_effect->effect_type() != SI_TYPE_CANVAS)
        Context::SIContext()->update_linking_relations(d_py_effect->link_relations(), d_py_effect->uuid());
}

const QMap<QString, QVariant>& Region::data() const
{
    return d_py_effect->data();
}

const glm::vec4 Region::color() const
{
    return d_py_effect->color();
}

int Region::handle_collision_event(const std::string &function_name, PySIEffect &colliding_effect)
{
    HANDLE_PYTHON_CALL(
        for (auto&[key, value]: colliding_effect.cap_collision_emit())
        {
            if (d_py_effect->cap_collision_recv().find(key) != d_py_effect->cap_collision_recv().end())
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
