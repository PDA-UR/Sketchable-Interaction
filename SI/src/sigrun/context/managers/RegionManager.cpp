

#include "RegionManager.hpp"
#include "../../log/Log.hpp"
#include <sigrun/context/Context.hpp>
#include <pysi/PySIEffect.hpp>
#include <sigrun/plugin/PythonInvoker.hpp>

RegionManager::RegionManager()
{SIGRUN

}

RegionManager::~RegionManager()
{
}

void RegionManager::add_region(const std::vector<glm::vec3> &contour, const bp::object& effect, int region_uuid)
{
    d_regions.push_back(std::shared_ptr<Region>(new Region(contour, effect)));
}

std::vector<std::shared_ptr<Region>> &RegionManager::regions()
{
    return d_regions;
}

void RegionManager::activate_mouse_region_button_down(int mouse_btn)
{
    HANDLE_PYTHON_CALL (
        for(auto& region: d_regions)
        {
            if(region->effect().effect_type() == SI_TYPE_MOUSE_CURSOR && !region->effect().has_mouse_pressed_capability(mouse_btn))
            {
                region->effect().set_mouse_pressed_capability(mouse_btn, true);

                switch(mouse_btn)
                {
                    case 0:
                    {
                        region->raw_effect().attr("left_mouse_clicked") = true;
                        region->raw_effect().attr("on_left_mouse_click")(true);
                        break;
                    }
                    case 1:
                    {
                        region->raw_effect().attr("right_mouse_clicked") = true;
                        region->raw_effect().attr("on_right_mouse_click")(true);
                        break;
                    }

                    case 2:
                    {
                        region->raw_effect().attr("middle_mouse_clicked") = true;
                        region->raw_effect().attr("on_middle_mouse_click")(true);
                        break;
                    }
                }
            }
        }
    )
}

void RegionManager::deactivate_mouse_region_button_down(int mouse_btn)
{
    HANDLE_PYTHON_CALL (
        for(auto& region: d_regions)
        {
            if(region->effect().effect_type() == SI_TYPE_MOUSE_CURSOR && region->effect().has_mouse_pressed_capability(mouse_btn))
            {
                region->effect().set_mouse_pressed_capability(mouse_btn, false);

                switch(mouse_btn)
                {
                    case 0:
                    {
                        region->raw_effect().attr("left_mouse_clicked") = false;
                        region->raw_effect().attr("on_left_mouse_click")(false);
                        break;
                    }
                    case 1:
                    {
                        region->raw_effect().attr("right_mouse_clicked") = false;
                        region->raw_effect().attr("on_right_mouse_click")(false);
                        break;
                    }

                    case 2:
                    {
                        region->raw_effect().attr("middle_mouse_clicked") = false;
                        region->raw_effect().attr("on_middle_mouse_click")(false);
                        break;
                    }
                }
            }
        }
    )
}

void RegionManager::set_partial_regions(std::map<std::string, std::vector<glm::vec3>>& partials)
{
    d_partial_regions = partials;
}

std::map<std::string, std::vector<glm::vec3>> &RegionManager::partial_regions()
{
    return d_partial_regions;
}

void RegionManager::update_via_mouse_input()
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
    for(auto& region: d_regions)
    {
        if (region->effect().effect_type() == SI_TYPE_MOUSE_CURSOR)
        {
            region->raw_effect().attr("mouse_wheel_angle_px") = angle_px;
            region->raw_effect().attr("mouse_wheel_angle_degrees") = angle_degrees;
        }
    }
}

void RegionManager::update()
{
    update_via_mouse_input();

    // required to avoid invalidation of vector iterator when adding a new object
    // therefore newly created regions are effective from the next frame on
    int size = d_regions.size();

    for(int i = 0; i < size; i++)
        d_regions[i]->update();
    //---------------------------------------------------------------------------
}
