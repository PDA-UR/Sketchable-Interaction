

#include "CollisionManager.hpp"
#include <sigrun/context/Context.hpp>
#include <execution>

#if !defined(Q_MOC_RUN)
#include <tbb/parallel_for.h>
#include <tbb/concurrent_unordered_map.h>
#endif

namespace bp = boost::python;

CollisionManager::CollisionManager() = default;
CollisionManager::~CollisionManager() = default;

void CollisionManager::collide(std::vector<std::shared_ptr<Region>> &regions)
{
    std::vector<std::tuple<Region*, Region*, bool>> collisions;

    perform_collision_check(collisions, regions);
    perform_collision_events(collisions);
    remove_dead_collision_events();
}

void CollisionManager::perform_collision_check(std::vector<std::tuple<Region*, Region*, bool>> &out, const std::vector<std::shared_ptr<Region>>& in)
{
    std::unordered_map<Region*, std::vector<std::vector<std::string>>> current_collisions;
    std::unordered_map<Region*, std::vector<std::string>> enveloped;

    for(int i = 0; i < in.size() - 1; ++i)
    {
        for(int k = i + 1; k < in.size(); ++k)
        {
            if(in[i]->name() == SI_NAME_EFFECT_MOUSE_CURSOR || in[k]->name() == SI_NAME_EFFECT_MOUSE_CURSOR || in[i]->name() == "__ Pen __" || in[k]->name() == "__ Pen __" || in[i]->name() == "__ Tip __" || in[k]->name() == "__ Tip __")
            {
                if (collides_with_mask(in[i], in[k]))
                {
                    if (has_capabilities_in_common(in[i], in[k]))
                    {
                        out.emplace_back(in[i].get(), in[k].get(), true);
                        continue;
                    }
                    else
                    {
                        const auto& a = in[i].get();
                        const auto& b = in[k].get();
                        current_collisions[a].push_back(std::vector<std::string> {b->uuid(), b->name()});
                        current_collisions[b].push_back(std::vector<std::string> {a->uuid(), a->name()});
                    }
                }
            }
            else
            {
                if (has_capabilities_in_common(in[i], in[k]))
                {
                    if (collides_with_aabb(in[i].get(), in[k].get()))
                    {
                        if (collides_with_mask(in[i], in[k]))
                        {
                            out.emplace_back(in[i].get(), in[k].get(), true);

                            if(in[i]->effect()->evaluate_enveloped())
                                if (evaluate_enveloped(in[i], in[k]))
                                    enveloped[in[i].get()].push_back(in[k]->uuid());

                            if(in[k]->effect()->evaluate_enveloped())
                                if(evaluate_enveloped(in[k], in[i]))
                                    enveloped[in[k].get()].push_back(in[i]->uuid());

                            continue;
                        }
                    }
                }
            }

            out.emplace_back(in[i].get(), in[k].get(), false);
        }
    }

    for(auto& [r, v]: enveloped)
    {
        bp::list l;

        for(const std::string& s: v)
            l.append(bp::str(s));

        r->raw_effect().attr("__enveloped_by__") = l;
    }

    for(std::tuple<Region*, Region*, bool> tuple: out)
    {
        if(std::get<2>(tuple))
        {
            const auto& a = std::get<0>(tuple);
            const auto& b = std::get<1>(tuple);

            current_collisions[a].push_back(std::vector<std::string> {b->uuid(), b->name()});
            current_collisions[b].push_back(std::vector<std::string> {a->uuid(), a->name()});
        }
    }

    std::vector<std::vector<std::string>> t;

    for(auto& [k, v]: current_collisions)
    {
        for(auto& inner: v)
        {
            t.emplace_back();
            for(auto& s: inner)
                t[t.size() - 1].push_back(s);
        }

        k->effect()->set_collisions(t);
        t.clear();
    }
}

void CollisionManager::perform_collision_events(std::vector<std::tuple<Region*, Region*, bool>> &in)
{
    for(std::tuple<Region*, Region*, bool>* elemit = in.data() + in.size() - 1; elemit >= in.data(); --elemit)
    {
        auto a = std::get<0>(*elemit);
        auto b = std::get<1>(*elemit);

        bool is_colliding = std::get<2>(*elemit);
        bool found = false;

        for(auto& tup: d_cols)
        {
            if(a && b && (std::get<0>(tup) == a->uuid()) && (std::get<1>(tup) == b->uuid()))
            {
                found = true;

                if(is_colliding)
                    handle_event_continuous(a, b);
                else
                {
                    handle_event_leave(a, b);
                    std::get<2>(tup) = false;
                }
                break;
            }
        }

        if(!found)
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
    for(int i = d_cols.size() - 1; i >= 0; --i)
    {
        if(!std::get<2>(d_cols[i]))
        {
            d_cols.erase(d_cols.begin() + i);
        }
    }
}

void CollisionManager::handle_event_leave_on_deletion(Region* deleted_region)
{
    auto& regions = Context::SIContext()->region_manager()->regions();

    for(int i = d_cols.size() - 1; i >= 0; --i)
    {
        if(std::get<0>(d_cols[i]) == deleted_region->uuid() || std::get<1>(d_cols[i]) == deleted_region->uuid())
        {
            for(auto& region: regions)
            {
                if(region && (region->uuid() == std::get<1>(d_cols[i]) || region->uuid() == std::get<0>(d_cols[i])))
                {
                    handle_event_leave(region.get(), deleted_region);
                    d_cols.erase(d_cols.begin() + i);
                }
            }
        }
    }
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
        for(auto& p: b->contour())
        {
            if((*a_mask)[glm::vec3(p.x + b->x(), p.y + b->y(), 1)])
            {
                return true;
            }
        }

        return false;
    }

    for(auto& p: a->contour())
    {
        if((*b_mask)[glm::vec3(p.x + a->x(), p.y + a->y(), 1)])
        {
            return true;
        }
    }

    return false;
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

bool CollisionManager::evaluate_enveloped(const std::shared_ptr<Region> &a, const std::shared_ptr<Region> &b)
{
    for(const glm::vec3& p: a->contour())
        if(!(*b->mask())[glm::vec3(p.x + a->x(), p.y + a->y(), 1)])
            return false;

    return true;
}
