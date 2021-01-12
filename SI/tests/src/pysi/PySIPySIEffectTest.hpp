

#ifndef SITEST_PYSIPYSIEFFECTTEST_HPP
#define SITEST_PYSIPYSIEFFECTTEST_HPP

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sigrun/util/UUID.hpp>
#include <sigrun/util/Util.hpp>

#include <pysi/PySIEffect.hpp>
#include <sigrun/SIObject.hpp>
#include <sigrun/context/Context.hpp>

class PySIPySIEffectTest: public ::testing::Test, public SIObject
{ SITEST
public:
    virtual void SetUp() {

    }

    virtual void TearDown() {

    }

};

#endif //SITEST_PYSIPYSIEFFECTTEST_HPP
