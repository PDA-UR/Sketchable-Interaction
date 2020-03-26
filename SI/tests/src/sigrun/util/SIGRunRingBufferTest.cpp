

#include "SIGRunRingBufferTest.hpp"
#include <sigrun/util/RingBuffer.hpp>

TEST_F(SIGRunRingBufferTest, construction)
{
    EXPECT_NO_FATAL_FAILURE(RingBuffer<int> b(10));
}

TEST_F(SIGRunRingBufferTest, size)
{
    RingBuffer<uint8_t> b(10);

    ASSERT_EQ(b.size(), 0);
}

TEST_F(SIGRunRingBufferTest, max_size)
{
    RingBuffer<uint8_t> b(10);

    ASSERT_EQ(b.max_size(), 10);
}

TEST_F(SIGRunRingBufferTest, push_back)
{
    RingBuffer<uint8_t> b(10);

    b.push_back(1);

    ASSERT_EQ(b.size(), 1);
    ASSERT_EQ(b.max_size(), 10);

    b.push_back(2);

    ASSERT_EQ(b.size(), 2);
    ASSERT_EQ(b.max_size(), 10);

    b.push_back(3);

    ASSERT_EQ(b.size(), 3);
    ASSERT_EQ(b.max_size(), 10);

    b.push_back(4);

    ASSERT_EQ(b.size(), 4);
    ASSERT_EQ(b.max_size(), 10);

    b.push_back(5);

    ASSERT_EQ(b.size(), 5);
    ASSERT_EQ(b.max_size(), 10);

    b.push_back(6);

    ASSERT_EQ(b.size(), 6);
    ASSERT_EQ(b.max_size(), 10);

    b.push_back(7);

    ASSERT_EQ(b.size(), 7);
    ASSERT_EQ(b.max_size(), 10);

    b.push_back(8);

    ASSERT_EQ(b.size(), 8);
    ASSERT_EQ(b.max_size(), 10);

    b.push_back(9);

    ASSERT_EQ(b.size(), 9);
    ASSERT_EQ(b.max_size(), 10);

    b.push_back(10);

    ASSERT_EQ(b.size(), 10);
    ASSERT_EQ(b.max_size(), 10);

    b.push_back(11);

    ASSERT_EQ(b.size(), 10);
    ASSERT_EQ(b.max_size(), 10);
}

TEST_F(SIGRunRingBufferTest, push_back_operator_overload_with_shift_op)
{
    RingBuffer<uint8_t> b(10);

    b << 1;

    ASSERT_EQ(b.size(), 1);
    ASSERT_EQ(b.max_size(), 10);

    b << 2;

    ASSERT_EQ(b.size(), 2);
    ASSERT_EQ(b.max_size(), 10);

    b << 3;

    ASSERT_EQ(b.size(), 3);
    ASSERT_EQ(b.max_size(), 10);

    b << 4;

    ASSERT_EQ(b.size(), 4);
    ASSERT_EQ(b.max_size(), 10);

    b << 5;

    ASSERT_EQ(b.size(), 5);
    ASSERT_EQ(b.max_size(), 10);

    b << 6;

    ASSERT_EQ(b.size(), 6);
    ASSERT_EQ(b.max_size(), 10);

    b << 7;

    ASSERT_EQ(b.size(), 7);
    ASSERT_EQ(b.max_size(), 10);

    b << 8;

    ASSERT_EQ(b.size(), 8);
    ASSERT_EQ(b.max_size(), 10);

    b << 9;

    ASSERT_EQ(b.size(), 9);
    ASSERT_EQ(b.max_size(), 10);

    b << 10;

    ASSERT_EQ(b.size(), 10);
    ASSERT_EQ(b.max_size(), 10);

    b << 11;

    ASSERT_EQ(b.size(), 10);
    ASSERT_EQ(b.max_size(), 10);
}

TEST_F(SIGRunRingBufferTest, get)
{
    RingBuffer<uint8_t> b(10);

    b.push_back(1);

    ASSERT_EQ(b.size(), 1);
    ASSERT_EQ(b.max_size(), 10);

    b.push_back(2);

    ASSERT_EQ(b.size(), 2);
    ASSERT_EQ(b.max_size(), 10);

    b.push_back(3);

    ASSERT_EQ(b.size(), 3);
    ASSERT_EQ(b.max_size(), 10);

    b.push_back(4);

    ASSERT_EQ(b.size(), 4);
    ASSERT_EQ(b.max_size(), 10);

    b.push_back(5);

    ASSERT_EQ(b.size(), 5);
    ASSERT_EQ(b.max_size(), 10);

    b.push_back(6);

    ASSERT_EQ(b.size(), 6);
    ASSERT_EQ(b.max_size(), 10);

    b.push_back(7);

    ASSERT_EQ(b.size(), 7);
    ASSERT_EQ(b.max_size(), 10);

    b.push_back(8);

    ASSERT_EQ(b.size(), 8);
    ASSERT_EQ(b.max_size(), 10);

    b.push_back(9);

    ASSERT_EQ(b.size(), 9);
    ASSERT_EQ(b.max_size(), 10);

    b.push_back(10);

    ASSERT_EQ(b.size(), 10);
    ASSERT_EQ(b.max_size(), 10);

    b.push_back(11);

    ASSERT_EQ(b.size(), 10);
    ASSERT_EQ(b.max_size(), 10);

    ASSERT_EQ(2, b.get());
    ASSERT_EQ(3, b.get());
    ASSERT_EQ(4, b.get());

    b.push_back(12);

    ASSERT_EQ(b.size(), 10);
    ASSERT_EQ(b.max_size(), 10);

    ASSERT_EQ(5, b.get());
    ASSERT_EQ(6, b.get());
    ASSERT_EQ(7, b.get());
}

TEST_F(SIGRunRingBufferTest, find)
{
    RingBuffer<uint8_t> b(10);

    b.push_back(12);
    b.push_back(14);
    b.push_back(16);
    b.push_back(18);

    ASSERT_TRUE(b.find(18));
    ASSERT_FALSE(b.find(17));
    b.push_back(17);
    ASSERT_TRUE(b.find(17));
}

TEST_F(SIGRunRingBufferTest, find_operator_overload_with_and_op)
{
    RingBuffer<uint8_t> b(10);

    b.push_back(12);
    b.push_back(14);
    b.push_back(16);
    b.push_back(18);

    ASSERT_TRUE(b & 18);
    ASSERT_FALSE(b & 17);

    b.push_back(17);

    ASSERT_TRUE(b & 17);
}

TEST_F(SIGRunRingBufferTest, clear)
{
    RingBuffer<uint8_t> b(10);

    b.push_back(12);
    b.push_back(14);
    b.push_back(16);
    b.push_back(18);

    ASSERT_TRUE(b & 12);
    ASSERT_TRUE(b & 14);
    ASSERT_TRUE(b & 16);
    ASSERT_TRUE(b & 18);

    ASSERT_EQ(b.size(), 4);

    b.clear();

    ASSERT_EQ(b.size(), 0);

    ASSERT_FALSE(b.find(12));
    ASSERT_FALSE(b.find(14));
    ASSERT_FALSE(b.find(16));
    ASSERT_FALSE(b.find(18));
}

TEST_F(SIGRunRingBufferTest, empty)
{
    RingBuffer<uint8_t> b(10);

    ASSERT_TRUE(b.empty());

    b.push_back(1);

    ASSERT_FALSE(b.empty());

    b.clear();

    ASSERT_TRUE(b.empty());
}