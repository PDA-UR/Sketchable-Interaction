

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
    d_region_insertion_queries.reserve(15);
}

RegionManager::~RegionManager()
{
}

void RegionManager::query_region_insertion(const std::vector<glm::vec3> &contour, const bp::object& effect, std::shared_ptr<Region>& parent, const bp::dict& kwargs, const std::string& attrib_sender, const std::string& attrib_recv)
{
    d_region_insertion_queries.emplace_back(contour, effect, kwargs, parent, attrib_sender, attrib_recv);
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
    std::transform(std::execution::par_unseq, d_regions.begin(), d_regions.end(), d_regions.begin(), [mouse_btn](auto& region)
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

        return region;
    });
}

void RegionManager::deactivate_mouse_region_button_down(uint32_t mouse_btn)
{
    std::transform(std::execution::par, d_regions.begin(), d_regions.end(), d_regions.begin(), [mouse_btn](auto& region)
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

        return region;
    });
}

void RegionManager::set_partial_regions(std::map<std::string, std::vector<glm::vec3>>& partials)
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
    std::transform(std::execution::par_unseq, d_regions.begin(), d_regions.end(), d_regions.begin(), [angle_px, angle_degrees](auto& region)
    {
        if (region->effect().effect_type() == SI_TYPE_MOUSE_CURSOR)
        {
            region->raw_effect().attr("mouse_wheel_angle_px") = angle_px;
            region->raw_effect().attr("mouse_wheel_angle_degrees") = angle_degrees;
        }

        return region;
    });
}

void RegionManager::update_region_deletions()
{
    d_regions.erase(std::remove_if(d_regions.begin(), d_regions.end(), [&](auto& region) -> bool
    {
        if(!region->effect().is_flagged_for_deletion())
            return false;

        Context::SIContext()->linking_manager()->remove_all_partaking_linking_relations(region->uuid());

        return true;
    }), d_regions.end());
}

void RegionManager::update_region_insertions()
{
    std::transform(d_region_insertion_queries.begin(), d_region_insertion_queries.end(), std::back_inserter(d_regions), [](const auto& query) -> std::shared_ptr<Region>
    {
        auto region = std::make_shared<Region>(std::get<0>(query), std::get<1>(query), 0, 0, std::get<2>(query));

        if(std::get<3>(query).get())
            std::get<3>(query)->raw_effect().attr("children").attr("append")(region->raw_effect());

        return region;
    });

    d_region_insertion_queries.clear();
}

void RegionManager::update_regions()
{
    std::transform(d_regions.rbegin(), d_regions.rend(), d_regions.rbegin(), [](auto& region) -> std::shared_ptr<Region>
    {
        region->update();
        return region;
    });
}

void RegionManager::update()
{
    update_mouse_inputs();
    update_region_deletions();
    update_region_insertions();
    update_regions();
}

std::shared_ptr<Region>& RegionManager::region_by_uuid(const std::string& uuid)
{
    return *std::find_if(std::execution::par_unseq, d_regions.begin(), d_regions.end(), [&](auto& region)
    {
        return region->uuid() == uuid;
    });
}

