

#include <sigrun/context/Context.hpp>
#include "TangibleManager.hpp"

TangibleManager::TangibleManager()
{

}

TangibleManager::~TangibleManager()
{

}

void TangibleManager::update(SITUIOObject *tobj)
{
    int s_id = tobj->s_id();

    if(std::find_if(d_tangible_ids.begin(), d_tangible_ids.end(), [&s_id](int id)
    {
        return id == s_id;
    }) == d_tangible_ids.end())
        add_tangible(tobj);
    else
        update_tangible(tobj);
}

void TangibleManager::remove(int s_id)
{
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

void TangibleManager::add_tangible(SITUIOObject *tobj)
{
    d_tangible_ids.push_back(tobj->s_id());

    if (tobj->has_outer_counter_geometry_component() && tobj->has_symbol_component() && tobj->has_token_component())
    {
        bp::dict kwargs;
//        Py_IncRef(kwargs.ptr());
        kwargs["s_id"] = tobj->s_id();
        kwargs["angle"] = tobj->token_component()->angle();

        Context::SIContext()->register_new_region_via_name(tobj->outer_contour_geometry_component()->contour(), tobj->symbol_component()->data(), false, kwargs);
    }
    else if (tobj->has_bounds_component() && tobj->has_symbol_component() && tobj->has_token_component())
    {
        bp::dict kwargs;
//        Py_IncRef(kwargs.ptr());
        kwargs["s_id"] = tobj->s_id();
        kwargs["angle"] = tobj->token_component()->angle();

        std::vector<glm::vec3> contour =
        {
            glm::vec3(tobj->bounds_component()->x_pos(), tobj->bounds_component()->y_pos(), 1),
            glm::vec3(tobj->bounds_component()->x_pos(), tobj->bounds_component()->y_pos() + tobj->bounds_component()->height(), 1),
            glm::vec3(tobj->bounds_component()->x_pos() + tobj->bounds_component()->width(), tobj->bounds_component()->y_pos() + tobj->bounds_component()->height(), 1),
            glm::vec3(tobj->bounds_component()->x_pos() + tobj->bounds_component()->width(), tobj->bounds_component()->y_pos(), 1)
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
void TangibleManager::update_tangible(SITUIOObject *tobj)
{
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
    auto& regions = Context::SIContext()->region_manager()->regions();

    auto it = std::find_if(regions.begin(), regions.end(), [s_id](std::shared_ptr<Region>& region)
    {
        if(PyObject_HasAttrString(region->raw_effect().ptr(), "s_id"))
        {
            int rs_id = bp::extract<int>(region->raw_effect().attr("s_id"));

            return rs_id == s_id;
        }

        return false;
    });

    return it == regions.end() ? nullptr: (*it).get();
}
