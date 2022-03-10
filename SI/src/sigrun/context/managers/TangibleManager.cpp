

#include <sigrun/context/Context.hpp>
#include "TangibleManager.hpp"

TangibleManager::TangibleManager() = default;
TangibleManager::~TangibleManager() = default;

void TangibleManager::receive(const TangibleObjectMessage* p_message)
{
    if(std::find_if(d_tangible_ids.begin(), d_tangible_ids.end(), [&](int id)
    {
        return id == p_message->id();
    }) == d_tangible_ids.end())
        add_tangible(p_message);
    else
        update_tangible(p_message);
}

void TangibleManager::add_tangible(const TangibleObjectMessage *p_message)
{
    PythonGlobalInterpreterLockGuard g;

    if(!(MESSAGE_VALID(p_message)))
        return;

    d_tangible_ids.push_back(p_message->id());

    std::vector<glm::vec3> contour;
    correct_shape(contour, p_message->shape(), p_message->tracker_dimensions());
    const std::vector<glm::vec3>& original_contour = p_message->shape();

    bp::dict kwargs;

    kwargs["id"] = p_message->id();

    if(p_message->has_links())
        add_links_to_kwargs(kwargs, p_message->links());

    kwargs["contour"] = contour;
    kwargs["original_contour"] = p_message->shape();
    kwargs["click"] = p_message->is_click();
    kwargs["drag"] = p_message->is_drag();
    kwargs["x"] = p_message->x() / p_message->tracker_dimension_x() * Context::SIContext()->width();
    kwargs["y"] = p_message->y()/ p_message->tracker_dimension_y() * Context::SIContext()->height();
    kwargs["alive"] = p_message->is_alive();
    kwargs["touch"] = p_message->is_touch();
    kwargs["color"] = p_message->color();
    kwargs["tx"] = p_message->tracker_dimension_x();
    kwargs["ty"] = p_message->tracker_dimension_y();

    Context::SIContext()->register_new_region_via_name(contour, p_message->plugin_identifier(), false, kwargs);
}

void TangibleManager::correct_shape(std::vector<glm::vec3> &out, const std::vector<glm::vec3> &in, const glm::vec2& tracker_dimensions)
{
    out.resize(in.size());

    for(int i = 0; i < in.size(); ++i)
    {
        const glm::vec3& p = in[i];

        out[i].x = p.x / tracker_dimensions.x * Context::SIContext()->width();
        out[i].y = p.y / tracker_dimensions.y * Context::SIContext()->height();
    }
}

void TangibleManager::add_links_to_kwargs(bp::dict &kwargs, const std::vector<int> &links)
{
    bp::list l;
    for(int link: links)
        l.append(link);

    kwargs["links"] = l;
}

void TangibleManager::update_tangible(const TangibleObjectMessage* p_message)
{
    PythonGlobalInterpreterLockGuard g;

    if(!(MESSAGE_VALID(p_message)))
        return;

    std::vector<glm::vec3> contour;
    correct_shape(contour, p_message->shape(), p_message->tracker_dimensions());

    bp::dict kwargs;
    kwargs["contour"] = contour;
    kwargs["original_contour"] = p_message->shape();
    kwargs["click"] = p_message->is_click();
    kwargs["drag"] = p_message->is_drag();
    kwargs["x"] = p_message->x() / p_message->tracker_dimension_x() * Context::SIContext()->width();
    kwargs["y"] = p_message->y()/ p_message->tracker_dimension_y() * Context::SIContext()->height();
    kwargs["alive"] = p_message->is_alive();
    kwargs["touch"] = p_message->is_touch();
    kwargs["color"] = p_message->color();
    kwargs["tx"] = p_message->tracker_dimension_x();
    kwargs["ty"] = p_message->tracker_dimension_y();
    std::shared_ptr<Region> r = associated_region(p_message->id());
    if(r)
        r->raw_effect().attr("__update__")(kwargs);

    if(p_message && !p_message->is_alive())
        remove(p_message->id());
}

std::shared_ptr<Region> TangibleManager::associated_region(int id)
{
    PythonGlobalInterpreterLockGuard g;

    auto& regions = Context::SIContext()->region_manager()->regions();

    for(auto& region: regions)
    {
        bool has_attr = PyObject_HasAttrString(region->raw_effect().ptr(), "object_id");

        if (!has_attr)
            continue;

        int object_id = bp::extract<int>(region->raw_effect().attr("object_id"));

        if(object_id == id)
            return region;
    }

    return nullptr;
}

void TangibleManager::remove(int id)
{
    PythonGlobalInterpreterLockGuard g;

    d_tangible_ids.erase(std::remove_if(d_tangible_ids.begin(), d_tangible_ids.end(), [&](int other)
    {
        if(other == id)
            return associated_region(id).get() != nullptr;

        return false;
    }), d_tangible_ids.end());
}

const std::vector<int> &TangibleManager::tangible_ids()
{
    return d_tangible_ids;
}