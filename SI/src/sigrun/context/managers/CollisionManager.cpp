

#include "CollisionManager.hpp"
#include <sigrun/context/Context.hpp>
#include <execution>

namespace bp = boost::python;

CollisionManager::~CollisionManager()
{

}

void CollisionManager::collide(std::vector<std::shared_ptr<Region>> &regions)
{
    for(auto& [key, value]: d_collision_map)
        value = false;

    for(int32_t i = regions.size() - 1; i > -1; --i)
    {
        auto& a = regions[i];

        for(int32_t k = i - 1; k > -1; --k)
        {
            auto& b = regions[k];
            auto tuple = std::make_tuple(a->uuid(), b->uuid());

            if(has_capabilities_in_common(a, b))
            {
                if(a->is_transformed() || b->is_transformed())
                {
                    if(collides_with_aabb(a, b))
                    {
                        if(are_aabbs_equal(a, b) || is_aabb_enveloped(a, b) || is_aabb_enveloped(b, a) || collides_with_mask(a, b))
                            d_collision_map.find(tuple) != d_collision_map.end() ? handle_event_continuous(a, b, tuple) : handle_event_enter(a, b, tuple);
                        else
                            if(d_collision_map.find(tuple) != d_collision_map.end())
                                handle_event_leave(a, b, tuple);
                    }
                    else
                        if(d_collision_map.find(tuple) != d_collision_map.end())
                            handle_event_leave(a, b, tuple);
                }
                else
                {
                    if(d_collision_map.find(tuple) != d_collision_map.end())
                        has_capabilities_in_common(a, b) ? handle_event_continuous(a, b, tuple) : handle_event_leave(a, b, tuple);
                }
            }
            else
                if(d_collision_map.find(tuple) != d_collision_map.end())
                    handle_event_leave(a, b, tuple);
        }
    }

    for(auto it = d_collision_map.begin(); it != d_collision_map.end();)
        it->second ? ++it : d_collision_map.erase(it++);
}

CollisionManager::CollisionManager() = default;

bool CollisionManager::collides_with_aabb(const std::shared_ptr<Region> &a, const std::shared_ptr<Region> &b)
{
    std::vector<glm::vec3> a_aabb(a->aabb().size());
    std::vector<glm::vec3> b_aabb(b->aabb().size());

    std::transform(std::execution::par, a->aabb().begin(), a->aabb().end(), a_aabb.begin(), [&](const glm::vec3& p)
    {
        auto p_ = p * a->transform();

        return p_ /= p_.z;
    });

    std::transform(std::execution::par, b->aabb().begin(), b->aabb().end(), b_aabb.begin(), [&](const glm::vec3& p)
    {
        auto p_ = p * b->transform();

        return p_ /= p_.z;
    });

    return a_aabb[0].x < b_aabb[3].x &&
           a_aabb[3].x > b_aabb[0].x &&
           a_aabb[0].y < b_aabb[1].y &&
           a_aabb[1].y > b_aabb[0].y;
}

bool CollisionManager::is_aabb_enveloped(const std::shared_ptr<Region>& enveloper, const std::shared_ptr<Region>& enveloped)
{
    auto& _a_aabb = enveloper->aabb();
    auto& _b_aabb = enveloped->aabb();

    std::vector<glm::vec3> a_aabb(_a_aabb.size());
    std::vector<glm::vec3> b_aabb(_b_aabb.size());

    std::transform(std::execution::par, _a_aabb.begin(), _a_aabb.end(), a_aabb.begin(), [&](const glm::vec3& p)
    {
        auto p_ = p * enveloper->transform();

        return p_ /= p_.z;
    });

    std::transform(std::execution::par, _b_aabb.begin(), _b_aabb.end(), b_aabb.begin(), [&](const glm::vec3& p)
    {
        auto p_ = p * enveloped->transform();

        return p_ /= p_.z;
    });

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
    auto& a_aabb = a->aabb();
    auto& b_aabb = b->aabb();
    auto& a_mask = a->mask();
    auto& b_mask = b->mask();

    float area_a_aabb = (a_aabb[3].x - a_aabb[0].x) * (a_aabb[1].y - a_aabb[0].y);
    float area_b_aabb = (b_aabb[3].x - b_aabb[0].x) * (b_aabb[1].y - b_aabb[0].y);

    if(area_a_aabb > area_b_aabb)
    {
        return std::find_if(std::execution::par_unseq, b->contour().begin(), b->contour().end(), [&](const glm::vec3& p)
        {
            glm::vec3 p__ = p * b->transform();
            return (*a_mask)[p__ /= p__.z];
        }) != b->contour().end();
    }
    else
    {
        return std::find_if(std::execution::par_unseq, a->contour().begin(), a->contour().end(), [&](const glm::vec3& p)
        {
            glm::vec3 p__ = p * a->transform();
            return (*a_mask)[p__ /= p__.z];
        }) != a->contour().end();
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

    std::vector<glm::vec3> a_aabb(a->aabb().size());
    std::vector<glm::vec3> b_aabb(b->aabb().size());

    std::transform(std::execution::par, a->aabb().begin(), a->aabb().end(), a_aabb.begin(), [&](const glm::vec3& p)
    {
        auto p_ = p * a->transform();

        return p_ /= p_.z;
    });

    std::transform(std::execution::par, b->aabb().begin(), b->aabb().end(), b_aabb.begin(), [&](const glm::vec3& p)
    {
        auto p_ = p * b->transform();

        return p_ /= p_.z;
    });

    for(uint32_t i = 0; i < a_aabb.size(); i++)
        if(!(are_aabb_same_size_and_spot &= (a_aabb[i] == b_aabb[i])))
            return false;

    return true;
}

void CollisionManager::handle_event_leave(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b, const std::tuple<std::string, std::string> &tuple)
{
    a->on_leave(b->effect());
    b->on_leave(a->effect());

    d_collision_map.erase(tuple);
}

void CollisionManager::handle_event_continuous(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b, const std::tuple<std::string, std::string>& tuple)
{
    a->on_continuous(b->effect());
    b->on_continuous(a->effect());

    d_collision_map[tuple] = true;
}

void CollisionManager::handle_event_enter(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b, const std::tuple<std::string, std::string> &tuple)
{
    a->on_enter(b->effect());
    b->on_enter(a->effect());

    d_collision_map[tuple] = true;
}
