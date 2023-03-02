

#ifndef SITEST_COLLISIONMANAGER_HPP
#define SITEST_COLLISIONMANAGER_HPP


#include <memory>
#include <vector>
#include <sigrun/region/Region.hpp>
#include <unordered_map>
#include <sigrun/SIObject.hpp>

#if !defined(Q_MOC_RUN)
#include <tbb/task_group.h>
#include <tbb/concurrent_vector.h>
#endif

class CollisionManager: public SIObject
{ SIGRUN
public:
    ~CollisionManager();
    CollisionManager();

    void collide(std::vector<std::shared_ptr<Region>>& regions);
    void handle_event_leave_on_deletion(Region* deleted_region);

private:
    void perform_collision_check(std::vector<std::tuple<Region*, Region*, bool>>& out, const std::vector<std::shared_ptr<Region>>& in);
    void perform_collision_events(std::vector<std::tuple<Region*, Region*, bool>>& in);
    void remove_dead_collision_events();

    bool collides_with_aabb(Region* a, Region* b);
    bool collides_with_mask(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b);
    bool has_capabilities_in_common(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b);

    void handle_event_continuous(Region* a, Region* b);
    void handle_event_leave(Region* a, Region* b);
    void handle_event_enter(Region* a, Region* b);

    std::vector<std::tuple<std::string, std::string, bool>> d_cols;

    friend class Context;
    friend class RegionManager;
    friend class SIGRunCollisionManagerTest;

    bool evaluate_enveloped(const std::shared_ptr<Region> &a, const std::shared_ptr<Region> &b);
};


#endif //SITEST_COLLISIONMANAGER_HPP
