

#ifndef SITEST_SIGRUNTEST_HPP
#define SITEST_SIGRUNTEST_HPP

#include <memory>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <SIGRun.hpp>

class MockSIGRun : public SIGRun
{
public:
    MOCK_METHOD(int, exec, (int argc, char** argv));
};


#endif //SITEST_SIGRUNTEST_HPP
