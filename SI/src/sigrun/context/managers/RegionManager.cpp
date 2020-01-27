

#include "RegionManager.hpp"
#include "../../log/Log.hpp"

RegionManager::RegionManager()
{SIGRUN

}

RegionManager::~RegionManager()
{
}

void RegionManager::add_region(const std::vector<glm::vec3> &contour, std::shared_ptr<bp::object> effect, int region_uuid)
{
    d_regions.emplace_back(std::make_shared<Region>(contour, effect));
}

std::vector<std::shared_ptr<Region>> &RegionManager::regions()
{
    return d_regions;
}

void RegionManager::activate_mouse_region_button_down(int mouse_btn)
{
    for(auto& region: d_regions)
    {
        if(region->effect().effect_type() == PySIEffect::EffectType::SI_MOUSE_CURSOR && !region->effect().has_mouse_pressed_capability(mouse_btn))
        {
            region->effect().set_mouse_pressed_capability(mouse_btn, true);

            switch(mouse_btn)
            {
                case 0: region->raw_effect().attr("left_mouse_clicked") = true; break;
                case 1: region->raw_effect().attr("right_mouse_clicked") = true; break;
                case 2: region->raw_effect().attr("middle_mouse_clicked") = true; break;

                region->update();
            }
        }
    }
}

void RegionManager::deactivate_mouse_region_button_down(int mouse_btn)
{
    for(auto& region: d_regions)
    {
        if(region->effect().effect_type() == PySIEffect::EffectType::SI_MOUSE_CURSOR && region->effect().has_mouse_pressed_capability(mouse_btn))
        {
            region->effect().set_mouse_pressed_capability(mouse_btn, false);

            switch(mouse_btn)
            {
                case 0: region->raw_effect().attr("left_mouse_clicked") = false; break;
                case 1: region->raw_effect().attr("right_mouse_clicked") = false; break;
                case 2: region->raw_effect().attr("middle_mouse_clicked") = false; break;

                region->update();
            }
        }
    }
}

void RegionManager::set_partial_region(const std::vector<glm::vec3> &partial)
{
    d_partial_region = partial;
}

std::vector<glm::vec3> &RegionManager::partial_region()
{
    return d_partial_region;
}

void RegionManager::update()
{

}
