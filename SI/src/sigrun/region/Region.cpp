

#include "sigrun/plugin/PythonInvoker.hpp"
#include "Region.hpp"

#include "RegionResampler.hpp"
#include <sigrun/log/Log.hpp>
#include <sigrun/util/UUID.hpp>
#include <sigrun/context/Context.hpp>

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
    qRegisterMetaType<bp::object>("bp::object");
    qRegisterMetaType<bp::tuple>("bp::tuple");

    HANDLE_PYTHON_CALL(
        d_effect = std::make_shared<bp::object>(bp::import("copy").attr("deepcopy")(effect));
        d_effect->attr("__init__")(contour, std::string(_UUID_), kwargs);

        d_py_effect = std::shared_ptr<PySIEffect>(new PySIEffect(bp::extract<PySIEffect>(*d_effect)));
    )

    if(!mask_width && !mask_height)
    {
        mask_width = Context::SIContext()->width();
        mask_height = Context::SIContext()->height();
    }

    uprm = std::make_unique<RegionMask>(mask_width, mask_height, d_py_effect->contour(), d_py_effect->aabb());
}

Region::~Region()= default;

void Region::move()
{
    const int32_t& x = d_py_effect->x();
    const int32_t& y = d_py_effect->y();

    if(x != d_last_x || y != d_last_y)
    {
        glm::vec2 center(d_last_x + (d_py_effect->aabb()[0].x + (d_py_effect->aabb()[3].x - d_py_effect->aabb()[0].x)) / 2, d_last_y + (d_py_effect->aabb()[0].y + (d_py_effect->aabb()[1].y - d_py_effect->aabb()[0].y)) / 2);

        uprt->update(glm::vec2(x, y), 0, 1, center);

        d_last_delta_x = x - d_last_x;
        d_last_delta_y = y - d_last_y;

        d_last_x = x;
        d_last_y = y;

        uprm->move(glm::vec2(d_last_delta_x, d_last_delta_y));

        d_is_transformed = true;
    }
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

uint8_t Region::on_enter(PySIEffect& colliding_effect)
{
    return handle_collision_event(SI_COLLISION_EVENT_ON_ENTER, colliding_effect);
}

uint8_t Region::on_continuous(PySIEffect& colliding_effect)
{
    return handle_collision_event(SI_COLLISION_EVENT_ON_CONTINUOUS, colliding_effect);
}

uint8_t Region::on_leave(PySIEffect& colliding_effect)
{
    return handle_collision_event(SI_COLLISION_EVENT_ON_LEAVE, colliding_effect);
}

void Region::LINK_SLOT(const std::string& uuid_event, const std::string& uuid_sender, const std::string& source_cap, const bp::tuple& args)
{
    std::tuple<std::string, std::string> event = std::make_tuple(uuid_event, source_cap);

    if(!is_link_event_registered(event))
    {
        register_link_event(event);

        std::for_each(std::execution::par_unseq, d_py_effect->attr_link_recv()[source_cap].begin(), d_py_effect->attr_link_recv()[source_cap].end(), [&](auto& pair)
        {
            HANDLE_PYTHON_CALL(
                if(uuid_sender.empty())
                {
                    pair.second(*args);

                    if(d_py_effect->attr_link_emit().find(pair.first) != d_py_effect->attr_link_emit().end())
                        Q_EMIT LINK_SIGNAL(uuid_event, this->uuid(), pair.first, bp::extract<bp::tuple>(d_py_effect->attr_link_emit()[pair.first]()));
                }
                else
                {
                    if(Context::SIContext()->linking_manager()->is_linked(uuid_sender, source_cap, this->uuid(), pair.first, ILink::UD))
                    {
                        pair.second(*args);

                        if(d_py_effect->attr_link_emit().find(pair.first) != d_py_effect->attr_link_emit().end())
                            Q_EMIT LINK_SIGNAL(uuid_event, this->uuid(), pair.first, bp::extract<bp::tuple>(d_py_effect->attr_link_emit()[pair.first]()));
                    }
                }
            )
        });
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

    HANDLE_PYTHON_CALL(d_py_effect = std::shared_ptr<PySIEffect>(new PySIEffect(bp::extract<PySIEffect>(*d_effect)));)

    if(d_py_effect->d_recompute_mask)
    {
        uprm = std::make_unique<RegionMask>(Context::SIContext()->width(), Context::SIContext()->height(), d_py_effect->contour(), d_py_effect->aabb());
        d_effect->attr("__recompute_collision_mask__") = false;
    }

    move();

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
            std::transform(std::execution::par_unseq, d_py_effect->regions_for_registration().begin(), d_py_effect->regions_for_registration().end(), d_py_effect->regions_for_registration().begin(), [&](auto& candidate)
            {
                Context::SIContext()->register_new_region(d_py_effect->partial_region_contours()[candidate], candidate);

                if(bp::len(d_effect->attr("__partial_regions__")))
                    HANDLE_PYTHON_CALL(bp::delitem(d_effect->attr("__partial_regions__"), bp::object(candidate));)

                return candidate;
            });

            HANDLE_PYTHON_CALL(d_effect->attr("__registered_regions__").attr("clear")();)
        }
    }
}

void Region::process_linking_relationships()
{
    if(d_py_effect->effect_type() != SI_TYPE_CANVAS)
        Context::SIContext()->linking_manager()->update_linking_candidates(d_py_effect->link_relations(), d_py_effect->uuid());
}

const QMap<QString, QVariant>& Region::data() const
{
    return d_py_effect->data();
}

const glm::vec4& Region::color() const
{
    return d_py_effect->color();
}

uint8_t Region::handle_collision_event(const std::string &function_name, PySIEffect &colliding_effect)
{
    std::for_each(std::execution::par_unseq, colliding_effect.cap_collision_emit().begin(), colliding_effect.cap_collision_emit().end(), [&](auto& pair)
    {
        HANDLE_PYTHON_CALL(

        const std::string& capability = pair.first;
        auto& emission_functions = pair.second;

        if (d_py_effect->cap_collision_recv().find(capability) != d_py_effect->cap_collision_recv().end())
        {
            if(!emission_functions[function_name].is_none())
            {
                const bp::object &t = emission_functions[function_name](*d_effect);

                if (t.is_none())
                {
                    if(!d_py_effect->cap_collision_recv()[capability][function_name].is_none())
                        d_py_effect->cap_collision_recv()[capability][function_name]();
                }
                else
                {
                    if(!d_py_effect->cap_collision_recv()[capability][function_name].is_none())
                    {
                        if (bp::extract<bp::tuple>(t).check())
                            d_py_effect->cap_collision_recv()[capability][function_name](*t);
                        else
                            d_py_effect->cap_collision_recv()[capability][function_name](t);
                    }
                }
            }
        }
        )
    });

    return 0;
}
