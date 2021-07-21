

#include <sigrun/context/Context.hpp>
#include "TangibleManager.hpp"

TangibleManager::TangibleManager()
{

}

TangibleManager::~TangibleManager()
{

}

void TangibleManager::update(SITUIOObject *tobj, int sw, int sh)
{
    int s_id = tobj->s_id();

    if(std::find_if(d_tangible_ids.begin(), d_tangible_ids.end(), [&s_id](int id)
    {
        return id == s_id;
    }) == d_tangible_ids.end())
        add_tangible(tobj, sw, sh);
    else
        update_tangible(tobj, sw, sh);
}

void TangibleManager::remove(int s_id)
{
    PythonGlobalInterpreterLockGuard g;

    d_tangible_ids.erase(std::remove_if(d_tangible_ids.begin(), d_tangible_ids.end(), [&](int id)
    {
        if(id == s_id)
        {
            Region* r = associated_region(s_id);

            if(r)
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

void TangibleManager::add_tangible(SITUIOObject *tobj, int sw, int sh)
{
    PythonGlobalInterpreterLockGuard g;

    d_tangible_ids.push_back(tobj->s_id());

    int w = Context::SIContext()->width();
    int h = Context::SIContext()->height();

    if (tobj->has_outer_counter_geometry_component() && tobj->has_symbol_component() && tobj->has_token_component())
    {
        bp::dict kwargs;
        kwargs["s_id"] = tobj->s_id();
        kwargs["angle"] = tobj->token_component()->angle();

        std::vector<glm::vec3> contour = tobj->outer_contour_geometry_component()->contour();

        for(auto& p: contour)
        {
            p.x = p.x / sw * w;
            p.y = p.y / sh * h;
        }

        if(tobj->has_linking_association_component())
        {
            bp::list l;

            const auto& v = tobj->link_association()->link_associations();

            for(auto i: v)
                l.append(i);

            kwargs["links"] = l;
        }

        Context::SIContext()->register_new_region_via_name(tobj->outer_contour_geometry_component()->contour(), tobj->symbol_component()->data(), false, kwargs);
    }
    else if (tobj->has_bounds_component() && tobj->has_symbol_component() && tobj->has_token_component())
    {
        bp::dict kwargs;
        kwargs["s_id"] = tobj->s_id();
        kwargs["angle"] = tobj->token_component()->angle();

        std::vector<glm::vec3> contour =
        {
            glm::vec3(tobj->bounds_component()->x_pos() / sw *  w, tobj->bounds_component()->y_pos() / sh * h, 1),
            glm::vec3(tobj->bounds_component()->x_pos() / sw *  w, (tobj->bounds_component()->y_pos() + tobj->bounds_component()->height()) / sh * h, 1),
            glm::vec3((tobj->bounds_component()->x_pos() + tobj->bounds_component()->width()) / sw *  w, (tobj->bounds_component()->y_pos() + tobj->bounds_component()->height()) / sh * h, 1),
            glm::vec3((tobj->bounds_component()->x_pos() + tobj->bounds_component()->width()) / sw *  w, tobj->bounds_component()->y_pos()  / sh * h, 1)
        };

        Context::SIContext()->register_new_region_via_name(contour, tobj->symbol_component()->data(), false, kwargs);
    }
    else if(tobj->has_symbol_component() && tobj->has_pointer_component())
    {
        bp::dict kwargs;
        kwargs["s_id"] = tobj->s_id();
        kwargs["angle"] = tobj->pointer_component()->angle();

        std::vector<glm::vec3> contour =
        {
            glm::vec3(tobj->pointer_component()->x_pos() / sw *  w, tobj->pointer_component()->y_pos() / sh * h, 1),
            glm::vec3(tobj->pointer_component()->x_pos() / sw *  w, (tobj->pointer_component()->y_pos() + 20) / sh * h, 1),
            glm::vec3((tobj->pointer_component()->x_pos() + 20) / sw *  w, (tobj->pointer_component()->y_pos() + 20) / sh * h, 1),
            glm::vec3((tobj->pointer_component()->x_pos() + 20) / sw *  w, tobj->pointer_component()->y_pos()  / sh * h, 1)
        };

        Context::SIContext()->register_new_region_via_name(contour, tobj->symbol_component()->data(), false, kwargs);
    }
    else
    {
        WARN("SIGRun can only register combined messages of (sym & tok & ocg) or (sym & tok & bnd) in order to create and assign an interactive region to the tracked tangible.");
        WARN("SIGRun will always use ocg data instead of bnd data, if both are provided by a tracker for the same s_id.");
        WARN("If ocg data is sent, then the accompanying token message must feature an angle as well as s_id, tu_id, and c_id.");
        WARN("If bnd data is sent, then the accompanying token message should feature the contour's angle and must feature s_id, tu_id, and c_id.");
        WARN("Although, the bnd message also has a variable for the angle, sending it via tok should unify implementation work. The angle property of a bnd message will be used as fallback if the token is unspecified.");
        WARN("Therefore, it is the sending application's responsibility to track the angle of tracked tangibles and their respective contours.");
    }
}

// here some issues may occur
// It is irrelevant whether the region contour is bnd or ocg data.
// For each new TUIO frame, each region must perform a full update of its contour, aabb, and collision mask
// This is due to users having three dimensions of freedom when interacting with a tangible: move and rotate.
// The rotation is the issue here.
// This may hurt performance a lot.

// reset the associated regions contour and trigger all accompanying calculations

/* what does a tracker do?
 * some sort of computer vision computation for detecting tangibles per frame
 * detect which type of tangible it is and whether that particular one was tracked before
 * store all detected tangibles in a data structure
 * send the type, contour, etc. via UDP according to the TUIO2 protocol
 *
 * therefore, each contour gets re-computed per frame regardless
 * SIGRun can reject the new contour if the tangible did not move significantly or was rotated below a threshold
 * this is currently not implemented
 */
void TangibleManager::update_tangible(SITUIOObject *tobj, int sw, int sh)
{
    PythonGlobalInterpreterLockGuard g;

    int w = Context::SIContext()->width();
    int h = Context::SIContext()->height();

    if (tobj->has_outer_counter_geometry_component() && tobj->has_symbol_component() && tobj->has_token_component())
    {
        Region* r = associated_region(tobj->s_id());

        if(r)
            r->effect()->set_shape(tobj->outer_contour_geometry_component()->contour());
    }
    else if (tobj->has_bounds_component() && tobj->has_symbol_component() && tobj->has_token_component())
    {
        Region* r = associated_region(tobj->s_id());

        if(r)
        {
            std::vector<glm::vec3> contour =
            {
                glm::vec3(tobj->bounds_component()->x_pos(), tobj->bounds_component()->y_pos(), 1),
                glm::vec3(tobj->bounds_component()->x_pos(), tobj->bounds_component()->y_pos() + tobj->bounds_component()->height(), 1),
                glm::vec3(tobj->bounds_component()->x_pos() + tobj->bounds_component()->width(), tobj->bounds_component()->y_pos() + tobj->bounds_component()->height(), 1),
                glm::vec3(tobj->bounds_component()->x_pos() + tobj->bounds_component()->width(), tobj->bounds_component()->y_pos(), 1)
            };

            r->effect()->set_shape(contour);
        }
    }
    else if(tobj->has_symbol_component() && tobj->has_pointer_component())
    {
        Region* r = associated_region(tobj->s_id());

        if(r)
        {
            std::vector<glm::vec3> contour =
            {
                glm::vec3(tobj->pointer_component()->x_pos() / sw *  w, tobj->pointer_component()->y_pos() / sh * h, 1),
                glm::vec3(tobj->pointer_component()->x_pos() / sw *  w, (tobj->pointer_component()->y_pos() + 20) / sh * h, 1),
                glm::vec3((tobj->pointer_component()->x_pos() + 20) / sw *  w, (tobj->pointer_component()->y_pos() + 20) / sh * h, 1),
                glm::vec3((tobj->pointer_component()->x_pos() + 20) / sw *  w, tobj->pointer_component()->y_pos()  / sh * h, 1)
            };

            r->effect()->set_shape(contour);
        }
    }
    else
    {
        WARN("SIGRun can only register combined messages of (sym & tok & ocg) or (sym & tok & bnd) in order to create and assign an interactive region to the tracked tangible.");
        WARN("SIGRun will always use ocg data instead of bnd data, if both are provided by a tracker for the same s_id.");
        WARN("If ocg data is sent, then the accompanying token message must feature an angle as well as s_id, tu_id, and c_id.");
        WARN("If bnd data is sent, then the accompanying token message should feature the contour's angle and must feature s_id, tu_id, and c_id.");
        WARN("Although, the bnd message also has a variable for the angle, sending it via tok should unify implementation work. The angle property of a bnd message will be used as fallback if the token is unspecified.");
        WARN("Therefore, it is the sending application's responsibility to track the angle of tracked tangibles and their respective contours.");
    }
}



Region *TangibleManager::associated_region(int s_id)
{
    PythonGlobalInterpreterLockGuard g;

    std::shared_ptr<Region> r = associated_region_sp(s_id);
    return r == nullptr ? nullptr: r.get();
}

std::shared_ptr<Region> TangibleManager::associated_region_sp(int s_id)
{
    PythonGlobalInterpreterLockGuard g;

    auto& regions = Context::SIContext()->region_manager()->regions();

    auto it = std::find_if(regions.begin(), regions.end(), [s_id](std::shared_ptr<Region>& region)
    {
        if (!PyObject_HasAttrString(region->raw_effect().ptr(), "s_id"))
            return false;

        int rs_id = bp::extract<int>(region->raw_effect().attr("s_id"));

        return rs_id == s_id;

    });

    return it == regions.end() ? nullptr: *it;
}

void TangibleManager::manage_requested_linking_relationships(int s_id, bool is_linked, int *linked, int size)
{
    std::shared_ptr<Region> root = associated_region_sp(s_id);

    for(int i = 0; i < size; ++i)
    {
        std::shared_ptr<Region> other = associated_region_sp(linked[i]);



//        Context::SIContext()->linking_manager()->add_link(root, SI_CAPABILITY_LINK_POSITION, other, SI_CAPABILITY_LINK_POSITION, ILink::UD);
//        Context::SIContext()->linking_manager()->add_link(other, SI_CAPABILITY_LINK_POSITION, root, SI_CAPABILITY_LINK_POSITION, ILink::UD);
    }
}
