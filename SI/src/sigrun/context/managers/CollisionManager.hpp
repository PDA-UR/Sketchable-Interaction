

#ifndef SITEST_COLLISIONMANAGER_HPP
#define SITEST_COLLISIONMANAGER_HPP


#include <memory>
#include <vector>
#include <sigrun/region/Region.hpp>
#include <unordered_map>
#include <sigrun/SIObject.hpp>
#include <tbb/task_group.h>
#include <tbb/concurrent_vector.h>

class CollisionManager: public SIObject
{ SIGRUN
public:
    ~CollisionManager();
    CollisionManager();

    void collide(std::vector<std::shared_ptr<Region>>& regions);
    void handle_event_leave_on_deletion(std::shared_ptr<Region>& deleted_region);

private:
    void perform_collision_check(tbb::concurrent_vector<std::tuple<int, int, bool>>& out, const std::vector<std::shared_ptr<Region>>& in);
    void perform_collision_events(const tbb::concurrent_vector<std::tuple<int, int, bool>>& in, std::vector<std::shared_ptr<Region>>& regions);
    void remove_dead_collision_events();

    bool collides_with_aabb(const std::vector<glm::vec3>& a_aabb, int32_t ax, int32_t ay, const std::vector<glm::vec3>& b_aabb, int32_t bx, int32_t by);
    bool collides_with_mask(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b);
    bool has_capabilities_in_common(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b);

    void handle_event_continuous(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b);
    void handle_event_leave(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b);
    void handle_event_enter(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b);

    // test concurrent vector next
    std::vector<std::tuple<std::string, std::string, bool>> d_cols;

    friend class Context;
    friend class RegionManager;
    friend class SIGRunCollisionManagerTest;

protected:
};


#endif //SITEST_COLLISIONMANAGER_HPP
