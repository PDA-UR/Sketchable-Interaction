

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

    bp::dict kwargs;

    kwargs["id"] = p_message->id();

    if(p_message->has_links())
        add_links_to_kwargs(kwargs, p_message->links());

    Context::SIContext()->register_new_region_via_name(contour, p_message->plugin_identifier(), false, kwargs);

//        if(contour.size() == 4)
//        {
//            const glm::vec3& tlc = contour[0];
//            const glm::vec3& blc = contour[1];
//            const glm::vec3& trc = contour[3];
//
//            glm::vec3 p = trc - tlc;
//            glm::vec3 q = blc - tlc;
//            glm::vec2 xa = glm::normalize(p);
//            glm::vec2 ya = glm::normalize(q);
//
//            float width = glm::length(p);
//            float height = glm::length(q);
//
//            kwargs["x"] = tlc.x;
//            kwargs["y"] = tlc.y;
//            kwargs["x_axis"] = bp::list(bp::make_tuple(xa.x, xa.y));
//            kwargs["y_axis"] = bp::list(bp::make_tuple(ya.x, ya.y));
//            kwargs["width"] = width;
//            kwargs["height"] = height;
//            kwargs["orig_x"] = tobj->outer_contour_geometry_component()->contour()[0].x;
//            kwargs["orig_y"] = tobj->outer_contour_geometry_component()->contour()[0].y;
//            kwargs["orig_width"] = glm::length(tobj->outer_contour_geometry_component()->contour()[3] - tobj->outer_contour_geometry_component()->contour()[0]);
//            kwargs["orig_height"] = glm::length(tobj->outer_contour_geometry_component()->contour()[1] - tobj->outer_contour_geometry_component()->contour()[0]);
//        }
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

//            if(contour.size() == 4)
//            {
//                const glm::vec3& tlc = contour[0];
//                const glm::vec3& blc = contour[1];
//                const glm::vec3& trc = contour[3];
//
//                glm::vec3 p = trc - tlc;
//                glm::vec3 q = blc - tlc;
//                glm::vec2 xa = glm::normalize(p);
//                glm::vec2 ya = glm::normalize(q);
//
//                float width = glm::length(p);
//                float height = glm::length(q);
//
//                kwargs["x"] = tlc.x;
//                kwargs["y"] = tlc.y;
//                kwargs["x_axis"] = bp::list(bp::make_tuple(xa.x, xa.y));
//                kwargs["y_axis"] = bp::list(bp::make_tuple(ya.x, ya.y));
//                kwargs["width"] = width;
//                kwargs["height"] = height;
//                kwargs["orig_x"] = tobj->outer_contour_geometry_component()->contour()[0].x;
//                kwargs["orig_y"] = tobj->outer_contour_geometry_component()->contour()[0].y;
//                kwargs["orig_width"] = glm::length(tobj->outer_contour_geometry_component()->contour()[3] - tobj->outer_contour_geometry_component()->contour()[0]);
//                kwargs["orig_height"] = glm::length(tobj->outer_contour_geometry_component()->contour()[1] - tobj->outer_contour_geometry_component()->contour()[0]);
//            }
//    else if(tobj->has_symbol_component() && tobj->has_pointer_component()) // touch / drag
//    {
//        // ptr
//        // press: -1 <=> hover, 0 <=> touch, 1 <=> drag
//        // tu_id: t part <=> 1 for right index finger, 6 for left index finger
//
//        Region* r = associated_region(tobj->s_id());
//
//        if(r)
//        {
//            std::vector<glm::vec3> contour =
//            {
//                glm::vec3(tobj->pointer_component()->x_pos() / sw *  w, tobj->pointer_component()->y_pos() / sh * h, 1),
//                glm::vec3(tobj->pointer_component()->x_pos() / sw *  w, (tobj->pointer_component()->y_pos() + 20) / sh * h, 1),
//                glm::vec3((tobj->pointer_component()->x_pos() + 20) / sw *  w, (tobj->pointer_component()->y_pos() + 20) / sh * h, 1),
//                glm::vec3((tobj->pointer_component()->x_pos() + 20) / sw *  w, tobj->pointer_component()->y_pos()  / sh * h, 1)
//            };
//
//            bp::dict update;
//            update["press"] = tobj->pointer_component()->press();
//            update["finger"] = tobj->pointer_component()->press();
//
//            int tx = tobj->pointer_component()->x_pos() / sw *  w;
//            int ty = tobj->pointer_component()->y_pos() / sh * h;
//
//            r->raw_effect().attr("last_x") = r->effect()->d_x;
//            r->raw_effect().attr("last_y") = r->effect()->d_y;
//            r->effect()->d_x = tx - r->aabb()[0].x;
//            r->effect()->d_y = ty - r->aabb()[0].y;
//
//            r->raw_effect().attr("__update__")(update);
//        }
//    }
//    else
//    {
//        WARN("SIGRun can only register combined messages of (sym & tok & ocg) or (sym & tok & bnd) in order to create and assign an interactive region to the tracked tangible.");
//        WARN("SIGRun will always use ocg data instead of bnd data, if both are provided by a tracker for the same s_id.");
//        WARN("If ocg data is sent, then the accompanying token message must feature an angle as well as s_id, tu_id, and c_id.");
//        WARN("If bnd data is sent, then the accompanying token message should feature the contour's angle and must feature s_id, tu_id, and c_id.");
//        WARN("Although, the bnd message also has a variable for the angle, sending it via tok should unify implementation work. The angle property of a bnd message will be used as fallback if the token is unspecified.");
//        WARN("Therefore, it is the sending application's responsibility to track the angle of tracked tangibles and their respective contours.");
//    }
//}

void TangibleManager::manage_requested_linking_relationships(int s_id, bool is_linked, int *linked, int size)
{
    std::shared_ptr<Region> root = associated_region(s_id);

    for(int i = 0; i < size; ++i)
    {
        std::shared_ptr<Region> other = associated_region(linked[i]);

        Context::SIContext()->linking_manager()->add_link(root, SI_CAPABILITY_LINK_POSITION, other, SI_CAPABILITY_LINK_POSITION, ILink::UD);
        Context::SIContext()->linking_manager()->add_link(other, SI_CAPABILITY_LINK_POSITION, root, SI_CAPABILITY_LINK_POSITION, ILink::UD);
    }
}
