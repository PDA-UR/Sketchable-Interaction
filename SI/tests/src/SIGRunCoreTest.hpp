

#ifndef SITEST_SIGRUNCORETEST_HPP
#define SITEST_SIGRUNCORETEST_HPP

#include <gmock/gmock.h>

#include "../../src/sigrun/Core.hpp"


class MockCore : public Core
{
public:
    MOCK_METHOD(void, start ,());
};


#endif //SITEST_SIGRUNCORETEST_HPP
