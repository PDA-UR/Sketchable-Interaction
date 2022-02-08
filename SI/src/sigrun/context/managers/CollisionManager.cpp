

#include "CollisionManager.hpp"
#include <sigrun/context/Context.hpp>
#include <execution>

#if !defined(Q_MOC_RUN)
#include <tbb/parallel_for.h>
#endif

namespace bp = boost::python;

CollisionManager::CollisionManager() = default;
CollisionManager::~CollisionManager() = default;

void CollisionManager::collide(std::vector<std::shared_ptr<Region>> &regions)
{
    tbb::concurrent_vector<std::tuple<Region*, Region*, bool>> collisions;

    perform_collision_check(collisions, regions);
    perform_collision_events(collisions);
    remove_dead_collision_events();
}

void CollisionManager::perform_collision_check(tbb::concurrent_vector<std::tuple<Region*, Region*, bool>> &out, const std::vector<std::shared_ptr<Region>>& in)
{
//    for(int i = 0; i < in.size() - 1; ++i)
//    {
//        for(int k = i + 1; k < in.size(); ++k)
//        {
//            if(Context::SIContext()->spatial_hash_grid()->has_shared_cell(in[i].get(), in[k].get()))
//            {
//                if (collides_with_aabb(in[i].get(), in[k].get()))
//                {
//                    if (collides_with_mask(in[i], in[k]))
//                    {
//                        out.emplace_back(in[i].get(), in[k].get(), has_capabilities_in_common(in[i], in[k]));
//                        continue;
//                    }
//                }
//            }
//
//            out.emplace_back(in[i].get(), in[k].get(), false);
//        }
//    }

    tbb::parallel_for(tbb::blocked_range<uint32_t>(0, in.size() - 1), [&](const tbb::blocked_range<uint32_t>& r)
    {
        for(auto i = r.begin(); i != r.end(); ++i)
        {
            tbb::parallel_for(tbb::blocked_range<uint32_t>(i + 1, in.size()), [&](const tbb::blocked_range<uint32_t>& r2)
            {
                for(auto k = r2.begin(); k != r2.end(); ++k)
                {
//                    if(Context::SIContext() && Context::SIContext()->spatial_hash_grid()->has_shared_cell(in[i].get(), in[k].get()))
//                    {
                        if(has_capabilities_in_common(in[i], in[k]))
                        {
                            if (collides_with_aabb(in[i].get(), in[k].get()))
                            {
                                if (collides_with_mask(in[i], in[k]))
                                {
                                    out.emplace_back(in[i].get(), in[k].get(), true);
                                    continue;
                                }
                            }
                        }
//                    }

                    out.emplace_back(in[i].get(), in[k].get(), false);
                }
            });
        }
    });

    std::unordered_map<Region*, std::vector<std::string>> current_collisions;

    for(std::tuple<Region*, Region*, bool> tuple: out)
    {
        if(std::get<2>(tuple))
        {
            const auto& a = std::get<0>(tuple);
            const auto& b = std::get<1>(tuple);

            current_collisions[a].push_back(b->uuid());
            current_collisions[b].push_back(a->uuid());
        }
    }

    for(auto& [k, v]: current_collisions)
        k->effect()->set_collisions(v);
}

void CollisionManager::perform_collision_events(tbb::concurrent_vector<std::tuple<Region*, Region*, bool>> &in)
{
    for(auto elemit = in.rbegin(); elemit != in.rend(); ++elemit)
    {
        auto a = std::get<0>(*elemit);
        auto b = std::get<1>(*elemit);

        bool is_colliding = std::get<2>(*elemit);

        auto it = std::find_if(d_cols.begin(), d_cols.end(), [&](auto &tup)
        {
            return a && b && (std::get<0>(tup) == a->uuid()) && (std::get<1>(tup) == b->uuid());
        });

        if(it != d_cols.end())
        {
            if(is_colliding)
                handle_event_continuous(a, b);
            else
            {
                handle_event_leave(a, b);
                std::get<2>(*it) = false;
            }
        }
        else
        {
            if(is_colliding)
            {
                handle_event_enter(a, b);
                d_cols.emplace_back(a->uuid(), b->uuid(), true);
            }
        }
    }
}

void CollisionManager::remove_dead_collision_events()
{
    d_cols.erase(std::remove_if(std::execution::par_unseq, d_cols.begin(), d_cols.end(), [&](auto &tup)
    {
        return std::get<2>(tup) == false;
    }), d_cols.end());
}

void CollisionManager::handle_event_leave_on_deletion(Region* deleted_region)
{
    auto& regions = Context::SIContext()->region_manager()->regions();

    d_cols.erase(std::remove_if(d_cols.begin(), d_cols.end(), [&](auto& tuple)
    {
        if(std::get<0>(tuple) == deleted_region->uuid() || std::get<1>(tuple) == deleted_region->uuid())
        {
            auto it = std::find_if(regions.begin(), regions.end(), [&](auto& region)
            {
                return region && (region->uuid() == std::get<1>(tuple) || region->uuid() == std::get<0>(tuple));
            });

            if(it != regions.end())
            {
                handle_event_leave(it->get(), deleted_region);
                return true;
            }
        }

        return false;
    }), d_cols.end());
}

bool CollisionManager::collides_with_aabb(Region* a, Region* b)
{
    return (a->aabb()[0].x + a->x()) < (b->aabb()[3].x + b->x()) &&
           (a->aabb()[3].x + a->x()) > (b->aabb()[0].x + b->x()) &&
           (a->aabb()[0].y + a->y()) < (b->aabb()[1].y + b->y()) &&
           (a->aabb()[1].y + a->y()) > (b->aabb()[0].y + b->y());
}

bool CollisionManager::collides_with_mask(const std::shared_ptr<Region> &a, const std::shared_ptr<Region> &b)
{
    auto& a_mask = a->mask();
    auto& b_mask = b->mask();

    float area_a = (a->aabb()[3].x - a->aabb()[0].x) * (a->aabb()[1].y - a->aabb()[0].y);
    float area_b = (b->aabb()[3].x - b->aabb()[0].x) * (b->aabb()[1].y - b->aabb()[0].y);

    if(area_a > area_b)
    {
        return std::find_if(b->contour().begin(), b->contour().end(), [&](auto& p)
                {
                    return ((*a_mask)[glm::vec3(p.x + b->x(), p.y + b->y(), 1)]);
                }) != b->contour().end();
    }

    return std::find_if(a->contour().begin(), a->contour().end(), [&](auto& p)
            {
                return ((*b_mask)[glm::vec3(p.x + a->x(), p.y + a->y(), 1)]);
            }) != a->contour().end();
}

bool CollisionManager::has_capabilities_in_common(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b)
{
    const std::unordered_map<std::string, std::unordered_map<std::string, bp::object>>& a_recv = a->effect()->cap_collision_recv();
    const std::unordered_map<std::string, std::unordered_map<std::string, bp::object>>& a_emit = a->effect()->cap_collision_emit();
    const std::unordered_map<std::string, std::unordered_map<std::string, bp::object>>& b_recv = b->effect()->cap_collision_recv();
    const std::unordered_map<std::string, std::unordered_map<std::string, bp::object>>& b_emit = b->effect()->cap_collision_emit();

    for(const auto& [key, value]: a_emit)
        if(b_recv.count(key))
            return true;

    for(const auto& [key, value]: b_emit)
        if(a_recv.count(key))
            return true;

    return false;
}

void CollisionManager::handle_event_leave(Region* a, Region* b)
{
    a->on_leave(b->effect());
    b->on_leave(a->effect());
}

void CollisionManager::handle_event_continuous(Region* a, Region* b)
{
    a->on_continuous(b->effect());
    b->on_continuous(a->effect());
}

void CollisionManager::handle_event_enter(Region* a, Region* b)
{
    a->on_enter(b->effect());
    b->on_enter(a->effect());
}
