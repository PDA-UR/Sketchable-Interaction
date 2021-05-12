

#include "sigrun/plugin/PythonInvoker.hpp"
#include "sigrun/plugin/PythonGlobalInterpreterLockGuard.hpp"
#include "Region.hpp"

#include "RegionResampler.hpp"
#include <sigrun/log/Log.hpp>
#include <sigrun/util/UUID.hpp>
#include <sigrun/context/Context.hpp>
#include <execution>
#include <QDebug>

namespace bp = boost::python;

Region::Region(const std::vector<glm::vec3> &contour, const bp::object& effect, uint32_t mask_width, uint32_t mask_height, const bp::dict& kwargs):
    uprt(std::make_unique<RegionTransform>()),
    d_is_transformed(false),
    d_link_events(50),
    d_last_x(0),
    d_last_y(0),
    d_last_delta_x(0),
    d_last_delta_y(0)
{
    set_effect(contour, effect, std::string(_UUID_), kwargs);

    if(!mask_width && !mask_height)
    {
        mask_width = Context::SIContext()->width();
        mask_height = Context::SIContext()->height();
    }

    uprm = std::make_unique<RegionMask>(mask_width, mask_height, d_py_effect->contour());

    if(Context::SIContext() && Context::SIContext()->spatial_hash_grid())
        Context::SIContext()->spatial_hash_grid()->register_region(this);
}

Region::~Region() = default;

void Region::move()
{
    const int32_t& x = d_py_effect->x();
    const int32_t& y = d_py_effect->y();

    if (x == d_last_x && y == d_last_y)
        return;

    glm::vec2 center(d_last_x + (d_py_effect->aabb()[0].x + (d_py_effect->aabb()[3].x - d_py_effect->aabb()[0].x)) / 2, d_last_y + (d_py_effect->aabb()[0].y + (d_py_effect->aabb()[1].y - d_py_effect->aabb()[0].y)) / 2);

    uprt->update(glm::vec2(x, y), 0, 1, center);

    d_last_delta_x = x - d_last_x;
    d_last_delta_y = y - d_last_y;

    d_last_x = x;
    d_last_y = y;

    uprm->move(glm::vec2(d_last_delta_x, d_last_delta_y));

    d_is_transformed = true;
}

void Region::set_effect(const bp::object& effect, const bp::dict& kwargs)
{
    if (effect.is_none())
        return;

    HANDLE_PYTHON_CALL(PY_ERROR, "Fatal Error. Plugin broken",
        d_effect = std::make_shared<bp::object>(effect.attr(effect.attr(SI_INTERNAL_NAME))(d_py_effect->contour(), d_py_effect->uuid(), kwargs));

        d_py_effect = bp::extract<PySIEffect*>(*d_effect);
    )
}

void Region::set_effect(const std::vector<glm::vec3>& contour, const bp::object& effect, const std::string& uuid, const bp::dict& kwargs)
{
    if (effect.is_none())
        return;

    HANDLE_PYTHON_CALL(PY_ERROR, "Fatal Error. Plugin broken",
        d_effect = std::make_shared<bp::object>(effect.attr(effect.attr(SI_INTERNAL_NAME))(contour, uuid, kwargs));
        d_py_effect = bp::extract<PySIEffect*>(*d_effect);
     )
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

PySIEffect* Region::effect()
{
    return d_py_effect;
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
    return d_py_effect->aabb();
}

const std::vector<glm::vec3>& Region::contour()
{
    return d_py_effect->contour();
}

const std::string &Region::qml_path() const
{
    return d_py_effect->qml_path();
}

const glm::mat3x3& Region::transform() const
{
    return uprt->transform();
}

uint8_t Region::on_enter(PySIEffect* colliding_effect)
{
    return handle_collision_event(SI_COLLISION_EVENT_ON_ENTER, colliding_effect);
}

uint8_t Region::on_continuous(PySIEffect* colliding_effect)
{
    return handle_collision_event(SI_COLLISION_EVENT_ON_CONTINUOUS, colliding_effect);
}

uint8_t Region::on_leave(PySIEffect* colliding_effect)
{
    return handle_collision_event(SI_COLLISION_EVENT_ON_LEAVE, colliding_effect);
}

void Region::LINK_SLOT(const std::string& uuid_event, const std::string& uuid_sender, const std::string& source_cap, const bp::object& args)
{
    HANDLE_PYTHON_CALL(PY_WARNING, "Failed to extract data to pass to linking target. Desired action did not occur! (" + name() + ")",

        std::tuple<std::string, std::string> event = std::make_tuple(uuid_event, source_cap);

        if(is_link_event_registered(event))
            return;

        register_link_event(event);

        for(auto& [k, v]: d_py_effect->attr_link_recv()[source_cap])
        {
            if(uuid_sender.empty())
                v(*args);
            else
            {
                if(Context::SIContext()->linking_manager()->is_linked(uuid_sender, source_cap, uuid(), k, ILink::UD))
                {
                    if (args.is_none())
                        v();
                    else
                    {
                        if (bp::extract<bp::tuple>(args).check())
                            v(*args);
                        else
                        {
                            // this is weird, unsure whether its a hack or not; needs investigation when time
                            if(!bp::extract<bp::dict>(args).check())
                                v(args());
                        }
                    }
                }
            }

            if(d_py_effect->attr_link_emit().find(k) != d_py_effect->attr_link_emit().end())
                Q_EMIT LINK_SIGNAL(uuid_event, uuid(), k, d_py_effect->attr_link_emit()[k]());
        }
    )
}

void Region::REGION_DATA_CHANGED_SLOT(const QMap<QString, QVariant>& data)
{
    HANDLE_PYTHON_CALL(PY_WARNING, "Unable to reset \'has_data_changed\' flag. Therefore, region data which targets QML widgets can no longer be set (" + name() + ")",
        d_py_effect->d_data_changed = false;
    )
}

void Region::register_link_event(const std::string &uuid, const std::string &attribute)
{
    register_link_event(std::make_tuple(uuid, attribute));
}

void Region::register_link_event(const std::tuple<std::string, std::string>& link_event)
{
    if(std::get<0>(link_event) != "" && std::get<1>(link_event) != "")
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

const uint16_t Region::type() const
{
    return d_py_effect->effect_type();
}

const uint32_t Region::width() const
{
    return d_py_effect->width();
}

const uint32_t Region::height() const
{
    return d_py_effect->height();
}

const int32_t Region::last_delta_x() const
{
    return d_last_delta_x;
}

const int32_t Region::last_delta_y() const
{
    return d_last_delta_y;
}

void Region::update()
{
    d_is_transformed = false;

    if(d_py_effect->d_recompute_mask)
    {
        uprm = std::make_unique<RegionMask>(Context::SIContext()->width(), Context::SIContext()->height(), d_py_effect->contour());
        uprm->move(glm::vec2(d_last_x, d_last_y));

        HANDLE_PYTHON_CALL(PY_ERROR, "Fatal Error. Region Collision broken (" + name() + ")",
            d_py_effect->d_recompute_mask = false;
        )
    }

    move();

    if(Context::SIContext())
        Context::SIContext()->spatial_hash_grid()->update_region(this);

    process_canvas_specifics();
    process_linking_relationships();
}

int32_t Region::x()
{
    return d_last_x;
}

int32_t Region::y()
{
    return d_last_y;
}

void Region::process_canvas_specifics()
{
    if(d_py_effect->effect_type() == SI_TYPE_CANVAS)
    {
        Context::SIContext()->region_manager()->set_partial_regions(d_py_effect->partial_region_contours());

        if(!d_py_effect->regions_for_registration().empty())
        {
            for(const auto& candidate: d_py_effect->regions_for_registration())
            {
                Context::SIContext()->register_new_region(d_py_effect->partial_region_contours()[candidate], candidate);

                HANDLE_PYTHON_CALL(PY_ERROR, "Fatal Error while sketching! Cannot unassign current region drawing! (" + name() + " " + candidate + ")",
                    if(!d_py_effect->d_partial_regions.empty())
                        d_py_effect->d_partial_regions.erase(candidate);
                )
            }

            HANDLE_PYTHON_CALL(PY_ERROR, "Fatal Error while sketching! Unable to remove newly registered regions from the registration list! (" + name() + ")",
                d_py_effect->regions_for_registration().clear();
            )
        }
    }
}

void Region::process_linking_relationships()
{
    if(d_py_effect->effect_type() != SI_TYPE_CANVAS)
    {
        Context::SIContext()->linking_manager()->update_linking_candidates(d_py_effect->link_relations(), d_py_effect->uuid());
    }
}

const QMap<QString, QVariant>& Region::data() const
{
    return d_py_effect->data();
}

const glm::vec4& Region::color() const
{
    return d_py_effect->color();
}

bool Region::is_new()
{
    return d_is_new;
}

void Region::set_data(const QMap<QString, QVariant>& data)
{
    const auto &regions = Context::SIContext()->region_manager()->regions();

    auto it = std::find_if(regions.begin(), regions.end(), [&](const auto &r)
    {
        return r->uuid() == data["uuid"].toString().toStdString();
    });

    if (it != regions.end())
        it->get()->effect()->set_data(data);
}

void Region::set_is_new(bool toggle)
{
    d_is_new = toggle;
}

std::vector<int> &Region::grid_nodes()
{
    return d_grid_nodes;
}

glm::ivec4 &Region::grid_bounds()
{
    return d_grid_bounds;
}

uint8_t Region::handle_collision_event(const std::string &function_name, PySIEffect* colliding_effect)
{
    for(auto& [capability, emission_functions]: colliding_effect->cap_collision_emit())
    {
        HANDLE_PYTHON_CALL(PY_ERROR, "Fatal Error. Unable to perform collision event " + function_name + " (" + name() + "other: " + colliding_effect->name() + ")",
           if (!d_py_effect->cap_collision_recv().count(capability))
               continue;

           if(emission_functions[function_name].is_none())
               continue;

           const bp::object &t = emission_functions[function_name](*d_effect);

           if (t.is_none())
           {
               if(!d_py_effect->cap_collision_recv()[capability][function_name].is_none())
                   d_py_effect->cap_collision_recv()[capability][function_name]();
           }
           else
           {
               if(d_py_effect->cap_collision_recv()[capability][function_name].is_none())
                   continue;

               if (bp::extract<bp::tuple>(t).check())
                   d_py_effect->cap_collision_recv()[capability][function_name](*t);
               else
                   d_py_effect->cap_collision_recv()[capability][function_name](t);
           }
        )
    }

    return 0;
}
