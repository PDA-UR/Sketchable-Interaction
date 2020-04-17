

#include "SIGRunLinkCandidateTest.hpp"
#include <sigrun/context/managers/helpers/linking/LinkCandidate.hpp>

TEST_F(SIGRunLinkCandidateTest, construction)
{
    ASSERT_NO_FATAL_FAILURE(LinkCandidate c("sender", "sender_attrib", "receiver", "receiver_attrib"));
}

TEST_F(SIGRunLinkCandidateTest, _operator_equality)
{
    LinkCandidate c1("sender1", "sender_attrib1", "receiver1", "receiver_attrib1");
    LinkCandidate c2("sender1", "sender_attrib1", "receiver1", "receiver_attrib1");
    LinkCandidate c3("sender2", "sender_attrib2", "receiver2", "receiver_attrib2");

    ASSERT_TRUE(c1 == c2);
    ASSERT_TRUE(c2 == c1);
    ASSERT_FALSE(c2 == c3);
    ASSERT_FALSE(c3== c2);
    ASSERT_FALSE(c1== c3);
    ASSERT_FALSE(c3== c1);
}

TEST_F(SIGRunLinkCandidateTest, _operator_inequality)
{
    LinkCandidate c1("sender1", "sender_attrib1", "receiver1", "receiver_attrib1");
    LinkCandidate c2("sender1", "sender_attrib1", "receiver1", "receiver_attrib1");
    LinkCandidate c3("sender2", "sender_attrib2", "receiver2", "receiver_attrib2");

    ASSERT_FALSE(c1 != c2);
    ASSERT_FALSE(c2 != c1);
    ASSERT_TRUE(c2 != c3);
    ASSERT_TRUE(c3 != c2);
    ASSERT_TRUE(c1 != c3);
    ASSERT_TRUE(c3 != c1);
}