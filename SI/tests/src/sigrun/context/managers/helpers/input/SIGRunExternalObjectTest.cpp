

#include "SIGRunExternalObjectTest.hpp"

#include <sigrun/context/managers/helpers/input/ExternalObject.hpp>

TEST_F(SIGRunExternalObjectTest, construction)
{
    EXPECT_NO_FATAL_FAILURE(ExternalObject eo(ExternalObject::ExternalObjectType::MOUSE));
    EXPECT_NO_FATAL_FAILURE(ExternalObject eo(ExternalObject::ExternalObjectType::FINGER));
    EXPECT_NO_FATAL_FAILURE(ExternalObject eo(ExternalObject::ExternalObjectType::TANGIBLE));
    EXPECT_NO_FATAL_FAILURE(ExternalObject eo(ExternalObject::ExternalObjectType::APPLICATION));
}

TEST_F(SIGRunExternalObjectTest, type)
{
    ExternalObject eo1(ExternalObject::ExternalObjectType::MOUSE);
    ExternalObject eo2(ExternalObject::ExternalObjectType::FINGER);
    ExternalObject eo3(ExternalObject::ExternalObjectType::TANGIBLE);
    ExternalObject eo4(ExternalObject::ExternalObjectType::APPLICATION);

    ASSERT_EQ(eo1.type(), ExternalObject::ExternalObjectType::MOUSE);
    ASSERT_EQ(eo2.type(), ExternalObject::ExternalObjectType::FINGER);
    ASSERT_EQ(eo3.type(), ExternalObject::ExternalObjectType::TANGIBLE);
    ASSERT_EQ(eo4.type(), ExternalObject::ExternalObjectType::APPLICATION);
}

TEST_F(SIGRunExternalObjectTest, uuid)
{
    ExternalObject eo(ExternalObject::ExternalObjectType::MOUSE);

    ASSERT_NO_FATAL_FAILURE(eo.uuid());
}