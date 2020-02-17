

#include "CollisionManager.hpp"
#include <sigrun/context/Context.hpp>

namespace bp = boost::python;

CollisionManager::~CollisionManager()
{

}

void CollisionManager::collide(std::vector<std::shared_ptr<Region>> &regions)
{
    for(int i = 0; i < regions.size(); ++i)
    {
        auto& a = regions[i];

        for(int k = i + 1; k < regions.size(); ++k)
        {
            auto& b = regions[k];
            auto tuple = std::make_tuple(a->uuid(), b->uuid());

            if(a->is_transformed() || b->is_transformed())
            {
                if(has_capabilities_in_common(a, b))
                {
                    if(collides_with_aabb(a, b))
                    {
                        if(are_aabbs_equal(a, b) || collides_with_mask(a, b))
                        {
                            if(d_collision_map.find(tuple) != d_collision_map.end())
                                handle_event_continuous(a, b);
                            else
                                handle_event_enter(a, b, tuple);
                        }
                        else
                        {
                            if(d_collision_map.find(tuple) != d_collision_map.end())
                                handle_event_leave(a, b, tuple);
                        }
                    }
                    else
                    {
                        if(d_collision_map.find(tuple) != d_collision_map.end())
                            handle_event_leave(a, b, tuple);
                    }
                }
                else
                {
                    if(d_collision_map.find(tuple) != d_collision_map.end())
                    {
                        handle_event_leave(a, b, tuple);
                    }
                }
            }
            else
            {
                if(d_collision_map.find(tuple) != d_collision_map.end())
                {
                    if(has_capabilities_in_common(a, b))
                        handle_event_continuous(a, b);
                    else
                        handle_event_leave(a, b, tuple);
                }
            }
        }
    }
}

CollisionManager::CollisionManager()
{SIGRUN
    collides_with = [&](const std::shared_ptr<Region> &a, const std::shared_ptr<Region> &b)
    {
        return collides_with_mask(a, b);
    };
}

bool CollisionManager::collides_with_aabb(const std::shared_ptr<Region> &a, const std::shared_ptr<Region> &b)
{
    auto& a_aabb = a->aabb();
    auto& b_aabb = b->aabb();

    return a_aabb[0].x < b_aabb[3].x &&
           a_aabb[3].x > b_aabb[0].x &&
           a_aabb[0].y < b_aabb[1].y &&
           a_aabb[1].y > b_aabb[0].y;
}

bool CollisionManager::is_aabb_enveloped(const std::shared_ptr<Region>& enveloper, const std::shared_ptr<Region>& enveloped)
{
    auto& a_aabb = enveloper->aabb();
    auto& b_aabb = enveloped->aabb();


    // claculate areas of aabbs in px^2
    float area_a_aabb = (a_aabb[3].x - a_aabb[0].x) * (a_aabb[1].y - a_aabb[0].y);
    float area_b_aabb = (b_aabb[3].x - b_aabb[0].x) * (b_aabb[1].y - b_aabb[0].y);

    // check which area is bigger or whether they are equal in size
    if(area_a_aabb > area_b_aabb)
    {
        bool is_tlc_enveloped = a_aabb[0].x <= b_aabb[0].x && a_aabb[0].y <= b_aabb[0].y;
        bool is_blc_enveloped = a_aabb[1].x <= b_aabb[1].x && a_aabb[1].y >= b_aabb[1].y;
        bool is_brc_enveloped = a_aabb[2].x >= b_aabb[2].x && a_aabb[2].y >= b_aabb[2].y;
        bool is_trc_enveloped = a_aabb[3].x >= b_aabb[3].x && a_aabb[3].y <= b_aabb[3].y;

        return is_tlc_enveloped && is_blc_enveloped && is_brc_enveloped && is_trc_enveloped;
    }

    return false;
}

bool CollisionManager::collides_with_mask(const std::shared_ptr<Region> &a, const std::shared_ptr<Region> &b)
{
    auto& a_mask = a->mask();
    auto& b_mask = b->mask();

    if(is_aabb_enveloped(a, b))
    {
        for (const glm::vec3 &p : b->contour())
        {
            glm::vec3 p__ = p * b->transform();
            p__ /= p__.z;

            if ((*a_mask)[p__])
                return true;
        }

    }else if(is_aabb_enveloped(b, a))
    {
        for (const glm::vec3 &p : a->contour())
        {
            glm::vec3 p__ = p * a->transform();
            p__ /= p__.z;

            if ((*b_mask)[p__])
                return true;
        }
    }

    return false;
}

bool CollisionManager::has_capabilities_in_common(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b)
{
    const std::map<std::string, std::map<std::string, bp::object>>& a_recv = a->effect().cap_collision_recv();
    const std::map<std::string, std::map<std::string, bp::object>>& a_emit = a->effect().cap_collision_emit();
    const std::map<std::string, std::map<std::string, bp::object>>& b_recv = b->effect().cap_collision_recv();
    const std::map<std::string, std::map<std::string, bp::object>>& b_emit = b->effect().cap_collision_emit();

    for(auto& [key, value]: a_emit)
        if(b_recv.find(key) != b_recv.end())
            return true;

    for(auto& [key, value]: b_emit)
        if(a_recv.find(key) != a_recv.end())
            return true;

    return false;
}

bool CollisionManager::are_aabbs_equal(const std::shared_ptr<Region> &a, const std::shared_ptr<Region> &b)
{
    bool are_aabb_same_size_and_spot = true;

    for(int i = 0; i < a->aabb().size(); i++)
        if(!(are_aabb_same_size_and_spot &= (a->aabb()[i] == b->aabb()[i])))
            return false;

    return true;
}

void CollisionManager::handle_event_leave(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b, const std::tuple<std::string, std::string> &tuple)
{
    a->on_leave(b->effect());
    b->on_leave(a->effect());

    d_collision_map.erase(tuple);
}

void CollisionManager::handle_event_continuous(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b)
{
    a->on_continuous(b->effect());
    b->on_continuous(a->effect());
}

void CollisionManager::handle_event_enter(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b, const std::tuple<std::string, std::string> &tuple)
{
    a->on_enter(b->effect());
    b->on_enter(a->effect());

    d_collision_map[tuple] = true;
}
