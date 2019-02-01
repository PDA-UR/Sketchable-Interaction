//
// Created by juergen on 01/02/19.
//

#ifndef CORE_SI_TEST_H
#define CORE_SI_TEST_H

class SITest : public ::testing::Test
{
public:
    virtual void SetUp() {}
    virtual void TearDown() {}
};

extern int callback_test(long uuid)
{
    return 0;
}

#endif //CORE_SI_TEST_H
