

#ifndef SITEST_SIGRUNCOLLISIONMANAGERTEST_HPP
#define SITEST_SIGRUNCOLLISIONMANAGERTEST_HPP

#include <gtest/gtest.h>
#include <sigrun/context/managers/CollisionManager.hpp>
#include <sigrun/context/Context.hpp>
#include <unordered_map>
#include <boost/python.hpp>
#include <memory>

class SIGRunCollisionManagerTest: public ::testing::Test
{
public:
    virtual void SetUp()
    {

    }

    virtual void TearDown()
    {

    }

    Context ctx = Context(1920, 1080, std::unordered_map<std::string, std::unique_ptr<bp::object>>());

    CollisionManager cm;

    bool cm_has_capabilities_in_common(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b)
    {
        return cm.has_capabilities_in_common(a, b);
    }

    bool cm_collides_with_aabb(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b)
    {
        return cm.collides_with_aabb(a, b);
    }

    bool cm_is_aabb_enveloped(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b)
    {
        return cm.is_aabb_enveloped(a, b);
    }

    bool cm_collides_with_mask(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b)
    {
        return cm.collides_with_mask(a, b);
    }

    bool cm_are_aabbs_equal(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b)
    {
        return cm.are_aabbs_equal(a, b);
    }

    void cm_handle_event_continuous(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b)
    {
        cm.handle_event_continuous(a, b);
    }

    void cm_handle_event_leave(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b, const std::tuple<std::string, std::string>& tuple)
    {
        cm.handle_event_leave(a, b, tuple);
    }

    void cm_handle_event_enter(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b, const std::tuple<std::string, std::string>& tuple)
    {
        cm.handle_event_enter(a, b, tuple);
    }
};


#endif //SITEST_SIGRUNCOLLISIONMANAGERTEST_HPP