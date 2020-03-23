

#include "RegionManager.hpp"
#include "../../log/Log.hpp"
#include <sigrun/context/Context.hpp>
#include <pysi/PySIEffect.hpp>
#include <sigrun/plugin/PythonInvoker.hpp>

RegionManager::RegionManager()
{SIGRUN
    d_region_insertion_queries.reserve(15);
}

RegionManager::~RegionManager()
{
}

void RegionManager::query_region_insertion(const std::vector<glm::vec3> &contour, const bp::object& effect, std::shared_ptr<Region>& parent, const bp::dict& kwargs, const std::string& attrib_sender, const std::string& attrib_recv)
{
    d_region_insertion_queries.emplace_back(contour, effect, kwargs, parent, attrib_sender, attrib_recv);
}

void RegionManager::add_region(const std::vector<glm::vec3> &contour, const bp::object &effect, int region_uuid, const bp::dict& kwargs)
{
    d_regions.push_back(std::shared_ptr<Region>(new Region(contour, effect, 0, 0, kwargs)));
}

void RegionManager::delete_region(const std::string &deletion_candidate_uuid)
{

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

bool RegionManager::update_region_deletions(int deletion_index)
{
    if(d_regions[deletion_index]->effect().is_flagged_for_deletion())
    {
        Context::SIContext()->remove_all_partaking_linking_relations(d_regions[deletion_index]->uuid());
        d_regions.erase(d_regions.begin() + deletion_index);

        return true;
    }

    return false;
}


void RegionManager::update()
{
    update_via_mouse_input();

    int size = d_regions.size();

    for(int i = size - 1; i > -1; --i)
    {
        if(update_region_deletions(i))
            continue;

        d_regions[i]->update();
    }

    for(auto& t: d_region_insertion_queries)
        add_region(std::get<0>(t), std::get<1>(t), 0, std::get<2>(t));

    for(int i = 0; i < d_region_insertion_queries.size(); ++i)
    {
        auto& query = d_region_insertion_queries[i];
        auto& region = d_regions[d_regions.size() - d_region_insertion_queries.size() + i];

        if(std::get<3>(query).get())
            std::get<3>(query)->raw_effect().attr("children").attr("append")(region->raw_effect());
    }

    d_region_insertion_queries.clear();
}
