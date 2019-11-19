

#ifndef SITEST_SIGRUNCORETEST_HPP
#define SITEST_SIGRUNCORETEST_HPP

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <sigrun/Core.hpp>

class SIGRunCoreTest: public ::testing::Test
{
public:
    virtual void SetUp()
    {

    }

    virtual void TearDown()
    {

    }

protected:
    void core_retrieve_available_plugins(std::unordered_map<std::string, std::shared_ptr<bp::object>> &plugins, const std::string& plugin_path)
    {
        core.retrieve_available_plugins(plugins, plugin_path);
    }

private:
    Core core;
};


class MockCore : public Core
{
public:
    MOCK_METHOD(void, start ,());
};


#endif //SITEST_SIGRUNCORETEST_HPP
