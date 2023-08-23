

#include "RegionManager.hpp"
#include "../../log/Log.hpp"
#include <sigrun/context/Context.hpp>
#include <pysi/PySIEffect.hpp>
#include <sigrun/plugin/PythonInvoker.hpp>
#include <memory>
#include <algorithm>
#include <execution>

RegionManager::RegionManager() = default;
RegionManager::~RegionManager() = default;

void RegionManager::add_region(const std::vector<glm::vec3> &contour, const bp::object &effect, const bp::dict& kwargs)
{
    d_regions.push_back(std::make_shared<Region>(contour, effect, 0, 0, kwargs));
    d_regions.back()->set_is_new(false);
    d_regions.back()->update();
}

void RegionManager::add_region(const bp::object &o, const bp::dict &qml)
{
    d_regions.push_back(std::make_shared<Region>(o, qml));
    d_regions.back()->set_is_new(false);
    d_regions.back()->update();
}

std::vector<std::shared_ptr<Region>> &RegionManager::regions()
{
    return d_regions;
}

void RegionManager::toggle_mouse_region_double_click(Region* cursor, bool toggle)
{
    if(toggle == d_previous_double_click_toggle)
        return;

    d_previous_double_click_toggle = toggle;

    for(auto& region: d_regions)
    {
        if(region->effect()->effect_type() == SI_TYPE_MOUSE_CURSOR)
        {
            region->effect()->set_mouse_pressed_capability(SI_DOUBLE_CLICK, toggle);
            region->raw_effect().attr("double_clicked") = toggle;
            region->raw_effect().attr("on_double_click")(toggle);
            break;
        }
    }
}

void RegionManager::activate_mouse_region_button_down(Region* cursor, uint32_t mouse_btn)
{
    cursor->effect()->set_mouse_pressed_capability(mouse_btn, true);

    switch(mouse_btn)
    {
        case SI_LEFT_MOUSE_BUTTON:
        {
            HANDLE_PYTHON_CALL(PY_ERROR, "Cannot foward left mouse click to plugin " + cursor->name(),
                               cursor->raw_effect().attr("left_mouse_clicked") = true;
                                       cursor->raw_effect().attr("on_left_mouse_click")(true);
            )
            break;
        }
        case SI_RIGHT_MOUSE_BUTTON:
        {
            HANDLE_PYTHON_CALL(PY_WARNING, "Cannot foward right mouse click to plugin " + cursor->name(),
                               cursor->raw_effect().attr("right_mouse_clicked") = true;
                                       cursor->raw_effect().attr("on_right_mouse_click")(true);
            )
            break;
        }

        case SI_MIDDLE_MOUSE_BUTTON:
        {
            HANDLE_PYTHON_CALL(PY_WARNING, "Cannot foward middle mouse click to plugin " + cursor->name(),
                               cursor->raw_effect().attr("middle_mouse_clicked") = true;
                                       cursor->raw_effect().attr("on_middle_mouse_click")(true);
            )
            break;
        }
    }

//    for(auto& region: d_regions)
//    {
//        if(region->effect()->effect_type() == SI_TYPE_MOUSE_CURSOR && !region->effect()->has_mouse_pressed_capability(mouse_btn))
//        {
//            region->effect()->set_mouse_pressed_capability(mouse_btn, true);
//
//            switch(mouse_btn)
//            {
//                case SI_LEFT_MOUSE_BUTTON:
//                {
//                    HANDLE_PYTHON_CALL(PY_ERROR, "Cannot foward left mouse click to plugin " + region->name(),
//                                       region->raw_effect().attr("left_mouse_clicked") = true;
//                                               region->raw_effect().attr("on_left_mouse_click")(true);
//                    )
//                    break;
//                }
//                case SI_RIGHT_MOUSE_BUTTON:
//                {
//                    HANDLE_PYTHON_CALL(PY_WARNING, "Cannot foward right mouse click to plugin " + region->name(),
//                                       region->raw_effect().attr("right_mouse_clicked") = true;
//                                       region->raw_effect().attr("on_right_mouse_click")(true);
//                    )
//                    break;
//                }
//
//                case SI_MIDDLE_MOUSE_BUTTON:
//                {
//                    HANDLE_PYTHON_CALL(PY_WARNING, "Cannot foward middle mouse click to plugin " + region->name(),
//                                       region->raw_effect().attr("middle_mouse_clicked") = true;
//                                       region->raw_effect().attr("on_middle_mouse_click")(true);
//                    )
//                    break;
//                }
//            }
//
//            break;
//        }
//    }
}

void RegionManager::deactivate_mouse_region_button_down(Region* cursor, uint32_t mouse_btn)
{
    cursor->effect()->set_mouse_pressed_capability(mouse_btn, false);

    switch(mouse_btn)
    {
        case SI_LEFT_MOUSE_BUTTON:
        {
            HANDLE_PYTHON_CALL(PY_WARNING, "Cannot foward left mouse release to plugin " + cursor->name(),
                               cursor->raw_effect().attr("left_mouse_clicked") = false;
                                       cursor->raw_effect().attr("on_left_mouse_click")(false);
            )
            break;
        }
        case SI_RIGHT_MOUSE_BUTTON:
        {
            HANDLE_PYTHON_CALL(PY_WARNING, "Cannot foward right mouse release to plugin " + cursor->name(),
                               cursor->raw_effect().attr("right_mouse_clicked") = false;
                                       cursor->raw_effect().attr("on_right_mouse_click")(false);
            )
            break;
        }

        case SI_MIDDLE_MOUSE_BUTTON:
        {
            HANDLE_PYTHON_CALL(PY_WARNING, "Cannot foward middle mouse release to plugin " + cursor->name(),
                               cursor->raw_effect().attr("middle_mouse_clicked") = false;
                                       cursor->raw_effect().attr("on_middle_mouse_click")(false);
            )
            break;
        }
    }

//    for(auto& region: d_regions)
//    {
//        if(region->effect()->effect_type() == SI_TYPE_MOUSE_CURSOR && region->effect()->has_mouse_pressed_capability(mouse_btn))
//        {
//            region->effect()->set_mouse_pressed_capability(mouse_btn, false);
//
//            switch(mouse_btn)
//            {
//                case SI_LEFT_MOUSE_BUTTON:
//                {
//                    HANDLE_PYTHON_CALL(PY_WARNING, "Cannot foward left mouse release to plugin " + region->name(),
//                                       region->raw_effect().attr("left_mouse_clicked") = false;
//                                       region->raw_effect().attr("on_left_mouse_click")(false);
//                    )
//                    break;
//                }
//                case SI_RIGHT_MOUSE_BUTTON:
//                {
//                    HANDLE_PYTHON_CALL(PY_WARNING, "Cannot foward right mouse release to plugin " + region->name(),
//                                       region->raw_effect().attr("right_mouse_clicked") = false;
//                                       region->raw_effect().attr("on_right_mouse_click")(false);
//                    )
//                    break;
//                }
//
//                case SI_MIDDLE_MOUSE_BUTTON:
//                {
//                    HANDLE_PYTHON_CALL(PY_WARNING, "Cannot foward middle mouse release to plugin " + region->name(),
//                                       region->raw_effect().attr("middle_mouse_clicked") = false;
//                                       region->raw_effect().attr("on_middle_mouse_click")(false);
//                    )
//                    break;
//                }
//            }
//
//            break;
//        }
//    }
}

void RegionManager::set_partial_regions(const std::unordered_map<std::string, std::vector<glm::vec3>>& partials)
{
    d_partial_regions = partials;
}

std::unordered_map<std::string, std::vector<glm::vec3>> &RegionManager::partial_regions()
{
    return d_partial_regions;
}

void RegionManager::update_key_inputs()
{
//    for(auto& region: d_regions)
//    {
//        if(region->effect()->effect_type() == SI_TYPE_MOUSE_CURSOR)
//        {
//            uint8_t mid = bp::extract<uint8_t>(region->raw_effect().attr("id"));



//            bool lctrl = Context::SIContext()->input_manager()->is_key_down(SI_KEY_CTRL_L);
//            bool rctrl = Context::SIContext()->input_manager()->is_key_down(SI_KEY_CTRL_R);
//
//            if(lctrl || rctrl)
//            {
//                region->raw_effect().attr("ctrl_pressed") = true;
//                region->raw_effect().attr("on_ctrl_pressed")(true);
//            }
//            else
//            {
//                region->raw_effect().attr("ctrl_pressed") = false;
//                region->raw_effect().attr("on_ctrl_pressed")(false);
//            }
//        }
//    }
}

void RegionManager::update_mouse_inputs()
{
    for (auto &region: d_regions)
    {
        if (region->effect()->effect_type() == SI_TYPE_MOUSE_CURSOR)
        {
            uint8_t mid = bp::extract<uint8_t>(region->raw_effect().attr("id"));

            if (Context::SIContext()->input_manager()->is_mouse_down(SI_LEFT_MOUSE_BUTTON, mid))
            {
                activate_mouse_region_button_down(region.get(), SI_LEFT_MOUSE_BUTTON);
            }
            else
            {
                deactivate_mouse_region_button_down(region.get(), SI_LEFT_MOUSE_BUTTON);
            }

            if (Context::SIContext()->input_manager()->is_mouse_down(SI_RIGHT_MOUSE_BUTTON, mid))
            {
                activate_mouse_region_button_down(region.get(), SI_RIGHT_MOUSE_BUTTON);
            }
            else
            {
                deactivate_mouse_region_button_down(region.get(), SI_RIGHT_MOUSE_BUTTON);
            }

            if (Context::SIContext()->input_manager()->is_mouse_down(SI_MIDDLE_MOUSE_BUTTON, mid))
            {
                activate_mouse_region_button_down(region.get(), SI_MIDDLE_MOUSE_BUTTON);
            }
            else
            {
                deactivate_mouse_region_button_down(region.get(), SI_MIDDLE_MOUSE_BUTTON);
            }

//                if (Context::SIContext()->input_manager()->is_double_click())
//                    toggle_mouse_region_double_click(cursor, true);
//                else
//                    toggle_mouse_region_double_click(cursor, false);
//
//                auto wheel_angles = Context::SIContext()->input_manager()->mouse_wheel_angles();
//
//                toggle_mouse_region_wheel_scrolled(cursor, wheel_angles.px, wheel_angles.degrees);
        }
    }
}

void RegionManager::toggle_mouse_region_wheel_scrolled(Region* cursor, float angle_px, float angle_degrees)
{
    for(auto& region: d_regions)
    {
        if(region->effect()->effect_type() == SI_TYPE_MOUSE_CURSOR)
        {
            region->effect()->mouse_wheel_angle_px = angle_px;
            region->effect()->mouse_wheel_angle_degrees = angle_degrees;
            break;
        }
    }
}

void RegionManager::update_region_deletions()
{
    for(int i = d_regions.size() - 1; i >= 0; --i)
    {
        if(d_regions[i]->effect()->is_flagged_for_deletion())
        {
            Context::SIContext()->collision_manager()->handle_event_leave_on_deletion(d_regions[i].get());
            Context::SIContext()->linking_manager()->remove_all_partaking_linking_relations(d_regions[i]->uuid());

            d_regions.erase(d_regions.begin() + i);
        }
    }
}

void RegionManager::update_regions()
{
    int32_t size = d_regions.size();

    for (int32_t i = size - 1; i > -1; --i)
    {
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
    update_key_inputs();
    update_mouse_inputs();
    update_region_deletions();
    update_regions();
}
