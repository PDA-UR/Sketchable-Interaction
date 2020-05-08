

#ifndef SITEST_COLLISIONMANAGER_HPP
#define SITEST_COLLISIONMANAGER_HPP


#include <memory>
#include <vector>
#include <sigrun/region/Region.hpp>
#include <unordered_map>
#include <sigrun/SIObject.hpp>

class CollisionManager: public SIObject
{ SIGRUN
public:
    ~CollisionManager();
    CollisionManager();

    void collide(std::vector<std::shared_ptr<Region>>& regions);
    void handle_event_leave_on_deletion(std::shared_ptr<Region>& deleted_region);

private:
    bool collides_with_aabb(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b);
    bool collides_with_mask(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b);
    bool has_capabilities_in_common(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b);
    bool are_aabbs_equal(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b);

    void handle_event_continuous(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b);
    void handle_event_leave(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b);
    void handle_event_enter(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b);

    std::vector<std::tuple<std::string, std::string>> d_cols;

    friend class Context;
    friend class RegionManager;
    friend class SIGRunCollisionManagerTest;

protected:
};


#endif //SITEST_COLLISIONMANAGER_HPP
