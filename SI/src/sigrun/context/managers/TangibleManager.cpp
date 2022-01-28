

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
    if(!(MESSAGE_VALID(p_message)))
        return;

    PythonGlobalInterpreterLockGuard g;

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
    if(p_message && !p_message->is_alive())
    {
        remove(p_message->id());
        return;
    }

    if(!(MESSAGE_VALID(p_message)))
        return;

    PythonGlobalInterpreterLockGuard g;

    std::shared_ptr<Region> r = associated_region(p_message->id());

    if(!r.get())
        return;

    std::vector<glm::vec3> contour;
    correct_shape(contour, p_message->shape(), p_message->tracker_dimensions());

    bp::dict kwargs;
    kwargs["contour"] = contour;
    kwargs["original_contour"] = p_message->shape();

    r->raw_effect().attr("__update__")(kwargs);
}

std::shared_ptr<Region> TangibleManager::associated_region(int id)
{
    auto& regions = Context::SIContext()->region_manager()->regions();

    auto it = std::find_if(regions.begin(), regions.end(), [id](std::shared_ptr<Region>& region)
    {
        if (!PyObject_HasAttrString(region->raw_effect().ptr(), "object_id"))
            return false;

        int object_id = bp::extract<int>(region->raw_effect().attr("object_id"));

        return object_id == id;
    });

    return it == regions.end() ? nullptr: *it;
}

void TangibleManager::remove(int id)
{
    PythonGlobalInterpreterLockGuard g;

    d_tangible_ids.erase(std::remove_if(d_tangible_ids.begin(), d_tangible_ids.end(), [&](int other)
    {
        if(other == id)
        {
            std::shared_ptr<Region> r = associated_region(id);

            if(r.get())
            {
                r->effect()->__signal_deletion__();
                return true;
            }
        }

        return false;
    }), d_tangible_ids.end());
}

const std::vector<int> &TangibleManager::tangible_ids()
{
    return d_tangible_ids;
}