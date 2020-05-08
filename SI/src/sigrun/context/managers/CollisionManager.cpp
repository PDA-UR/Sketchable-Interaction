

#include "CollisionManager.hpp"
#include <sigrun/context/Context.hpp>
#include <execution>

namespace bp = boost::python;

CollisionManager::~CollisionManager() = default;

void CollisionManager::collide(std::vector<std::shared_ptr<Region>> &regions)
{
    int i = 0;

    std::for_each(std::execution::seq, regions.rbegin(), regions.rend(), [&](auto& a)
    {
        std::for_each(std::execution::seq, regions.rbegin() + (++i), regions.rend(), [&](auto& b)
        {
            if(has_capabilities_in_common(a, b))
            {
                if(collides_with_aabb(a, b))
                {
                    if(are_aabbs_equal(a, b) || collides_with_mask(a, b))
                    {
                        if(std::execution::par_unseq, std::find_if(d_cols.begin(), d_cols.end(), [&](auto& tup)
                        {
                            return std::get<0>(tup) == a->uuid() && std::get<1>(tup) == b->uuid();
                        }) != d_cols.end())
                            handle_event_continuous(a, b);
                        else
                            handle_event_enter(a, b);
                    }
                    else
                    {
                        if(std::find_if(std::execution::par_unseq, d_cols.begin(), d_cols.end(), [&](auto& tup)
                        {
                            return std::get<0>(tup) == a->uuid() && std::get<1>(tup) == b->uuid();
                        }) != d_cols.end())
                            handle_event_leave(a, b);
                    }
                }
                else
                {
                    if(std::find_if(std::execution::par_unseq, d_cols.begin(), d_cols.end(), [&](auto& tup)
                    {
                        return std::get<0>(tup) == a->uuid() && std::get<1>(tup) == b->uuid();
                    }) != d_cols.end())
                        handle_event_leave(a, b);
                }
            }
            else
            {
                if(std::find_if(std::execution::par_unseq, d_cols.begin(), d_cols.end(), [&](auto& tup)
                {
                    return std::get<0>(tup) == a->uuid() && std::get<1>(tup) == b->uuid();
                }) != d_cols.end())
                    handle_event_leave(a, b);
            }
        });
    });
}

void CollisionManager::handle_event_leave_on_deletion(std::shared_ptr<Region>& deleted_region)
{
    auto& regions = Context::SIContext()->region_manager()->regions();

    std::for_each(std::execution::seq, d_cols.begin(), d_cols.end(), [&](auto& tuple)
    {
        if(std::get<0>(tuple) == deleted_region->uuid())
        {
            auto it = std::find_if(std::execution::par_unseq, regions.begin(), regions.end(), [&](auto& region)
            {
                if(!region)
                    return false;

                return region->uuid() == std::get<1>(tuple);
            });

            if(it != regions.end())
                handle_event_leave(deleted_region, *it);

            return;
        }

        if(std::get<1>(tuple) == deleted_region->uuid())
        {
            auto it = std::find_if(std::execution::par_unseq, regions.begin(), regions.end(), [&](auto& region)
            {
                if(!region)
                    return false;

                return region->uuid() == std::get<0>(tuple);
            });

            if(it != regions.end())
                handle_event_leave(*it, deleted_region);

            return;
        }
    });
}

CollisionManager::CollisionManager() = default;

bool CollisionManager::collides_with_aabb(const std::shared_ptr<Region> &a, const std::shared_ptr<Region> &b)
{
    std::vector<glm::vec3> a_aabb(a->aabb().size());
    std::vector<glm::vec3> b_aabb(b->aabb().size());

    std::transform(std::execution::seq, a->aabb().begin(), a->aabb().end(), a_aabb.begin(), [&](const glm::vec3& p)
    {
        auto p_ = p * a->transform();

        return p_ /= p_.z;
    });

    std::transform(std::execution::seq, b->aabb().begin(), b->aabb().end(), b_aabb.begin(), [&](const glm::vec3& p)
    {
        auto p_ = p * b->transform();

        return p_ /= p_.z;
    });

    return a_aabb[0].x < b_aabb[3].x &&
           a_aabb[3].x > b_aabb[0].x &&
           a_aabb[0].y < b_aabb[1].y &&
           a_aabb[1].y > b_aabb[0].y;
}

bool CollisionManager::collides_with_mask(const std::shared_ptr<Region> &a, const std::shared_ptr<Region> &b)
{
    auto& a_aabb = a->aabb();
    auto& b_aabb = b->aabb();
    auto& a_mask = a->mask();
    auto& b_mask = b->mask();

    float area_a_aabb = (a_aabb[3].x - a_aabb[0].x) * (a_aabb[1].y - a_aabb[0].y);
    float area_b_aabb = (b_aabb[3].x - b_aabb[0].x) * (b_aabb[1].y - b_aabb[0].y);

    return area_a_aabb > area_b_aabb ?  std::find_if(std::execution::par_unseq, b->contour().begin(), b->contour().end(), [&](const glm::vec3& p)
                                        {
                                            glm::vec3 p__ = p * b->transform();
                                            return (*a_mask)[p__ /= p__.z];
                                        }) != b->contour().end()
                                        :
                                        std::find_if(std::execution::par_unseq, a->contour().begin(), a->contour().end(), [&](const glm::vec3& p)
                                        {
                                            glm::vec3 p__ = p * a->transform();
                                            return (*a_mask)[p__ /= p__.z];
                                        }) != a->contour().end();
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

    std::transform(std::execution::seq, a->aabb().begin(), a->aabb().end(), a_aabb.begin(), [&](const glm::vec3& p)
    {
        auto p_ = p * a->transform();

        return p_ /= p_.z;
    });

    std::transform(std::execution::seq, b->aabb().begin(), b->aabb().end(), b_aabb.begin(), [&](const glm::vec3& p)
    {
        auto p_ = p * b->transform();

        return p_ /= p_.z;
    });

    return std::equal(a_aabb.begin(), a_aabb.end(), b_aabb.begin());
}

void CollisionManager::handle_event_leave(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b)
{
    a->on_leave(b->effect());
    b->on_leave(a->effect());

    d_cols.erase(std::remove_if(std::execution::par_unseq, d_cols.begin(), d_cols.end(), [&](auto& tup)
    {
        return std::get<0>(tup) == a->uuid() && std::get<1>(tup) == b->uuid();
    }), d_cols.end());
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
