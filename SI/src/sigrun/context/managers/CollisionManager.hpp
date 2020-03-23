

#ifndef SITEST_COLLISIONMANAGER_HPP
#define SITEST_COLLISIONMANAGER_HPP


#include <memory>
#include <vector>
#include <sigrun/region/Region.hpp>
#include <unordered_map>
#include <sigrun/SIObject.hpp>

class CollisionManager: public SIObject
{
public:
    ~CollisionManager();
    CollisionManager();

    void collide(std::vector<std::shared_ptr<Region>>& regions);

private:
    bool collides_with_aabb(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b);
    bool is_aabb_enveloped(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b);
    bool collides_with_mask(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b);
    bool has_capabilities_in_common(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b);
    bool are_aabbs_equal(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b);

    void handle_event_continuous(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b, const std::tuple<std::string, std::string>& tuple);
    void handle_event_leave(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b, const std::tuple<std::string, std::string>& tuple);
    void handle_event_enter(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b, const std::tuple<std::string, std::string>& tuple);


    typedef std::tuple<std::string, std::string> key_t;

    struct key_hash: public std::unary_function<key_t, std::size_t>
    {
        std::size_t operator()(const key_t& k) const
        {
            return std::get<0>(k)[0] ^ std::get<1>(k)[0];
        }
    };

    struct key_equal: public std::binary_function<key_t, key_t, bool>
    {
        bool operator()(const key_t& v0, const key_t& v1) const
        {
            return std::get<0>(v0) == std::get<0>(v1) && std::get<1>(v0) == std::get<1>(v1);
        }
    };

    std::unordered_map<const key_t, bool, key_hash, key_equal> d_collision_map;

    friend class Context;
    friend class RegionManager;
    friend class SIGRunCollisionManagerTest;

protected:
};


#endif //SITEST_COLLISIONMANAGER_HPP
