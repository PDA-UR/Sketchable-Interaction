

#include "CollisionManager.hpp"
#include <sigrun/context/Context.hpp>
#include <execution>

namespace bp = boost::python;

CollisionManager::~CollisionManager() = default;

void CollisionManager::collide(std::vector<std::shared_ptr<Region>> &regions)
{
    for(int32_t i = regions.size() - 1; i >= 0; --i)
    {
        for(int32_t k = i - 1; k >= 0; --k)
        {
            if (has_capabilities_in_common(regions[i], regions[k]))
            {
                if (collides_with_aabb(regions[i]->aabb(), regions[i]->x(), regions[i]->y(), regions[k]->aabb(), regions[k]->x(), regions[k]->y()))
                {
                    if (collides_with_mask(regions[i], regions[k]))
                    {
                        if (std::find_if(d_cols.begin(), d_cols.end(), [&](auto &tup)
                        {
                            return std::get<0>(tup) == regions[i]->uuid() && std::get<1>(tup) == regions[k]->uuid();
                        }) != d_cols.end())
                            handle_event_continuous(regions[i], regions[k]);
                        else
                            handle_event_enter(regions[i], regions[k]);
                    }
                    else
                    {
                        if (std::find_if(d_cols.begin(), d_cols.end(), [&](auto &tup)
                        {
                            return std::get<0>(tup) == regions[i]->uuid() && std::get<1>(tup) == regions[k]->uuid();
                        }) != d_cols.end())
                        {
                            handle_event_leave(regions[i], regions[k]);

                            d_cols.erase(std::remove_if(d_cols.begin(), d_cols.end(), [&](auto& tup)
                            {
                                return std::get<0>(tup) == regions[i]->uuid() && std::get<1>(tup) == regions[k]->uuid();
                            }), d_cols.end());
                        }
                    }
                }
                else
                {
                    if (std::find_if(d_cols.begin(), d_cols.end(), [&](auto &tup)
                    {
                        return std::get<0>(tup) == regions[i]->uuid() && std::get<1>(tup) == regions[k]->uuid();
                    }) != d_cols.end())
                    {
                        d_cols.erase(std::remove_if(d_cols.begin(), d_cols.end(), [&](auto& tup)
                        {
                            return std::get<0>(tup) == regions[i]->uuid() && std::get<1>(tup) == regions[k]->uuid();
                        }), d_cols.end());

                        handle_event_leave(regions[i], regions[k]);
                    }
                }
            }
            else
            {
                if (std::find_if(d_cols.begin(), d_cols.end(), [&](auto &tup)
                {
                    return std::get<0>(tup) == regions[i]->uuid() && std::get<1>(tup) == regions[k]->uuid();
                }) != d_cols.end())
                {
                    handle_event_leave(regions[i], regions[k]);

                    d_cols.erase(std::remove_if(d_cols.begin(), d_cols.end(), [&](auto& tup)
                    {
                        return std::get<0>(tup) == regions[i]->uuid() && std::get<1>(tup) == regions[k]->uuid();
                    }), d_cols.end());
                }
            }
        }
    }
}

void CollisionManager::handle_event_leave_on_deletion(std::shared_ptr<Region>& deleted_region)
{
    auto& regions = Context::SIContext()->region_manager()->regions();

    d_cols.erase(std::remove_if(d_cols.begin(), d_cols.end(), [&](auto& tuple)
    {
        if(std::get<0>(tuple) == deleted_region->uuid())
        {
            auto it = std::find_if(regions.begin(), regions.end(), [&](auto& region)
            {
                if(!region)
                    return false;

                return region->uuid() == std::get<1>(tuple);
            });

            if(it != regions.end())
            {
                handle_event_leave(deleted_region, *it);
                return true;
            }

            return false;
        }

        if(std::get<1>(tuple) == deleted_region->uuid())
        {
            auto it = std::find_if(regions.begin(), regions.end(), [&](auto& region)
            {
                if(!region)
                    return false;

                return region->uuid() == std::get<0>(tuple);
            });

            if(it != regions.end())
            {
                handle_event_leave(*it, deleted_region);
                return true;
            }
        }

        return false;
    }), d_cols.end());
}

CollisionManager::CollisionManager() = default;

bool CollisionManager::collides_with_aabb(const std::vector<glm::vec3>& a_aabb, int32_t ax, int32_t ay, const std::vector<glm::vec3>& b_aabb, int32_t bx, int32_t by)
{
    return (a_aabb[0].x + ax) < (b_aabb[3].x + bx) &&
           (a_aabb[3].x + ax) > (b_aabb[0].x + bx) &&
           (a_aabb[0].y + ay) < (b_aabb[1].y + by) &&
           (a_aabb[1].y + ay) > (b_aabb[0].y + by);
}

bool CollisionManager::collides_with_mask(const std::shared_ptr<Region> &a, const std::shared_ptr<Region> &b)
{
    auto& a_aabb = a->aabb();
    auto& b_aabb = b->aabb();
    auto& a_mask = a->mask();
    auto& b_mask = b->mask();

    float area_a_aabb = (a_aabb[3].x - a_aabb[0].x) * (a_aabb[1].y - a_aabb[0].y);
    float area_b_aabb = (b_aabb[3].x - b_aabb[0].x) * (b_aabb[1].y - b_aabb[0].y);

    if(area_a_aabb > area_b_aabb)
    {
        for(const auto& p: b->contour())
            if((*a_mask)[glm::vec3(p.x + b->x(), p.y + b->y(), 1)])
                return true;
    }
    else
    {
        for(const auto& p: a->contour())
            if((*b_mask)[glm::vec3(p.x + a->x(), p.y + a->y(), 1)])
                return true;
    }

    return false;
}

bool CollisionManager::has_capabilities_in_common(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b)
{
    const std::map<std::string, std::map<std::string, bp::object>>& a_recv = a->effect()->cap_collision_recv();
    const std::map<std::string, std::map<std::string, bp::object>>& a_emit = a->effect()->cap_collision_emit();
    const std::map<std::string, std::map<std::string, bp::object>>& b_recv = b->effect()->cap_collision_recv();
    const std::map<std::string, std::map<std::string, bp::object>>& b_emit = b->effect()->cap_collision_emit();

    for(const auto& [key, value]: a_emit)
        if(b_recv.find(key) != b_recv.end())
            return true;

    for(const auto& [key, value]: b_emit)
        if(a_recv.find(key) != a_recv.end())
            return true;

    return false;
}

void CollisionManager::handle_event_leave(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b)
{
    a->on_leave(b->effect());
    b->on_leave(a->effect());
}

void CollisionManager::handle_event_continuous(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b)
{
    a->on_continuous(b->effect());
    b->on_continuous(a->effect());
}

void CollisionManager::handle_event_enter(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b)
{
    a->on_enter(b->effect());
    b->on_enter(a->effect());

    d_cols.emplace_back(a->uuid(), b->uuid());
}
