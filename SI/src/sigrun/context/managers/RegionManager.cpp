

#include "RegionManager.hpp"
#include "../../log/Log.hpp"
#include <sigrun/context/Context.hpp>
#include <pysi/PySIEffect.hpp>
#include <sigrun/plugin/PythonInvoker.hpp>
#include <memory>
#include <algorithm>
#include <execution>

RegionManager::RegionManager()
{

}

RegionManager::~RegionManager()
{
}

void RegionManager::add_region(const std::vector<glm::vec3> &contour, const bp::object &effect, uint32_t region_uuid, const bp::dict& kwargs)
{
    d_regions.push_back(std::make_shared<Region>(contour, effect, 0, 0, kwargs));
}

std::vector<std::shared_ptr<Region>> &RegionManager::regions()
{
    return d_regions;
}

void RegionManager::activate_mouse_region_button_down(uint32_t mouse_btn)
{
    auto it = std::find_if(d_regions.begin(), d_regions.end(), [&](auto& region)
    {
        return region->effect()->effect_type() == SI_TYPE_MOUSE_CURSOR && !region->effect()->has_mouse_pressed_capability(mouse_btn);
    });

    if(it != d_regions.end())
    {
        it->get()->effect()->set_mouse_pressed_capability(mouse_btn, true);

        switch(mouse_btn)
        {
            case SI_LEFT_MOUSE_BUTTON:
            {
                HANDLE_PYTHON_CALL(PY_ERROR, "Cannot foward left mouse click to plugin " + it->get()->name(),
                    it->get()->raw_effect().attr("left_mouse_clicked") = true;
                    it->get()->raw_effect().attr("on_left_mouse_click")(true);
                )
                break;
            }
            case SI_RIGHT_MOUSE_BUTTON:
            {
                HANDLE_PYTHON_CALL(PY_WARNING, "Cannot foward right mouse click to plugin " + it->get()->name(),
                    it->get()->raw_effect().attr("right_mouse_clicked") = true;
                    it->get()->raw_effect().attr("on_right_mouse_click")(true);
                )
                break;
            }

            case SI_MIDDLE_MOUSE_BUTTON:
            {
                HANDLE_PYTHON_CALL(PY_WARNING, "Cannot foward middle mouse click to plugin " + it->get()->name(),
                    it->get()->raw_effect().attr("middle_mouse_clicked") = true;
                    it->get()->raw_effect().attr("on_middle_mouse_click")(true);
                )
                break;
            }
        }
    }
}

void RegionManager::deactivate_mouse_region_button_down(uint32_t mouse_btn)
{
    auto it = std::find_if(d_regions.begin(), d_regions.end(), [&](auto& region)
    {
        return region->effect()->effect_type() == SI_TYPE_MOUSE_CURSOR && region->effect()->has_mouse_pressed_capability(mouse_btn);
    });

    if(it != d_regions.end())
    {
        it->get()->effect()->set_mouse_pressed_capability(mouse_btn, false);

        switch(mouse_btn)
        {
            case SI_LEFT_MOUSE_BUTTON:
            {
                HANDLE_PYTHON_CALL(PY_WARNING, "Cannot foward left mouse release to plugin " + it->get()->name(),
                    it->get()->raw_effect().attr("left_mouse_clicked") = false;
                    it->get()->raw_effect().attr("on_left_mouse_click")(false);
                )
                break;
            }
            case SI_RIGHT_MOUSE_BUTTON:
            {
                HANDLE_PYTHON_CALL(PY_WARNING, "Cannot foward right mouse release to plugin " + it->get()->name(),
                    it->get()->raw_effect().attr("right_mouse_clicked") = false;
                    it->get()->raw_effect().attr("on_right_mouse_click")(false);
                )
                break;
            }

            case SI_MIDDLE_MOUSE_BUTTON:
            {
                HANDLE_PYTHON_CALL(PY_WARNING, "Cannot foward middle mouse release to plugin " + it->get()->name(),
                    it->get()->raw_effect().attr("middle_mouse_clicked") = false;
                    it->get()->raw_effect().attr("on_middle_mouse_click")(false);
                )
                break;
            }
        }
    }
}

void RegionManager::set_partial_regions(const std::map<std::string, std::vector<glm::vec3>>& partials)
{
    d_partial_regions = partials;
}

std::map<std::string, std::vector<glm::vec3>> &RegionManager::partial_regions()
{
    return d_partial_regions;
}

void RegionManager::update_mouse_inputs()
{
    if(Context::SIContext()->input_manager()->is_mouse_down(SI_LEFT_MOUSE_BUTTON))
        activate_mouse_region_button_down(SI_LEFT_MOUSE_BUTTON);
    else
        deactivate_mouse_region_button_down(SI_LEFT_MOUSE_BUTTON);

    if(Context::SIContext()->input_manager()->is_mouse_down(SI_RIGHT_MOUSE_BUTTON))
        activate_mouse_region_button_down(SI_RIGHT_MOUSE_BUTTON);
    else
        deactivate_mouse_region_button_down(SI_RIGHT_MOUSE_BUTTON);

    if(Context::SIContext()->input_manager()->is_mouse_down(SI_MIDDLE_MOUSE_BUTTON))
        activate_mouse_region_button_down(SI_MIDDLE_MOUSE_BUTTON);
    else
        deactivate_mouse_region_button_down(SI_MIDDLE_MOUSE_BUTTON);

    auto wheel_angles = Context::SIContext()->input_manager()->mouse_wheel_angles();

    toggle_mouse_region_wheel_scrolled(wheel_angles.px, wheel_angles.degrees);
}

void RegionManager::toggle_mouse_region_wheel_scrolled(float angle_px, float angle_degrees)
{
    auto it = std::find_if(d_regions.begin(), d_regions.end(), [angle_px, angle_degrees](auto& region)
    {
        return region->effect()->effect_type() == SI_TYPE_MOUSE_CURSOR;
    });

    if(it != d_regions.end())
    {
        HANDLE_PYTHON_CALL(PY_ERROR, "Cannot foward mouse wheel to plugin " + it->get()->name(),
            it->get()->raw_effect().attr("mouse_wheel_angle_px") = angle_px;
            it->get()->raw_effect().attr("mouse_wheel_angle_degrees") = angle_degrees;
        )
    }
}

void RegionManager::update_region_deletions()
{
    d_regions.erase(std::remove_if(d_regions.begin(), d_regions.end(), [&](std::shared_ptr<Region>& region) -> bool
    {
        if(!region->effect()->is_flagged_for_deletion())
            return false;

        Context::SIContext()->collision_manager()->handle_event_leave_on_deletion(region);
        Context::SIContext()->linking_manager()->remove_all_partaking_linking_relations(region->uuid());

        return true;
    }), d_regions.end());
}

void RegionManager::update_regions()
{
    int32_t size = d_regions.size();
    for (int32_t k = 0; k < size; ++k)
    {
        int32_t i = size - k - 1;
        if(!d_regions[i]->effect()->is_flagged_for_deletion())
        {
            if (d_regions[i]->is_new())
                d_regions[i]->set_is_new(false);
            else
                d_regions[i]->update();
        }
    }
}

void RegionManager::update()
{
    update_mouse_inputs();
    update_region_deletions();
    update_regions();
}
