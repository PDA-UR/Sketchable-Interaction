

#ifndef SITEST_SIGRUNCOLLISIONMANAGERTEST_HPP
#define SITEST_SIGRUNCOLLISIONMANAGERTEST_HPP

#include <gtest/gtest.h>
#include <sigrun/context/managers/CollisionManager.hpp>
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

    CollisionManager cm;

    bool cm_has_capabilities_in_common(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b)
    {
        return cm.has_capabilities_in_common(a, b);
    }

    bool cm_collides_with_aabb(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b)
    {
        return cm.collides_with_aabb(a.get(), b.get());
    }

    bool cm_collides_with_mask(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b)
    {
        return cm.collides_with_mask(a, b);
    }

    void cm_handle_event_continuous(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b)
    {
        cm.handle_event_continuous(a.get(), b.get());
    }

    void cm_handle_event_leave(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b)
    {
        cm.handle_event_leave(a.get(), b.get());
    }

    void cm_handle_event_enter(const std::shared_ptr<Region>& a, const std::shared_ptr<Region>& b)
    {
        cm.handle_event_enter(a.get(), b.get());
    }
};


#endif //SITEST_SIGRUNCOLLISIONMANAGERTEST_HPP
