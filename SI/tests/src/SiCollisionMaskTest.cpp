

#include "include/SiCollisionMaskTest.hpp"

TEST_F(SiCollisionMaskTest, construction)
{
    CollisionMask* m;

    EXPECT_NO_FATAL_FAILURE(m = new CollisionMask(1920, 1080, std::vector<Vector2<float>>()));
    EXPECT_NO_FATAL_FAILURE(m = new CollisionMask(1920, 1080, std::vector<Vector2<float>>{Vector2<float>(10, 10)}));
    EXPECT_NO_FATAL_FAILURE(m = new CollisionMask(1920, 1080, std::vector<Vector2<float>>{Vector2<float>(10, 10), Vector2<float>(20, 20)}));
    EXPECT_NO_FATAL_FAILURE(m = new CollisionMask(1920, 1080, contour_IXI));
    EXPECT_NO_FATAL_FAILURE(delete m) ;
    EXPECT_NO_FATAL_FAILURE(m = nullptr);
}

TEST_F(SiCollisionMaskTest, destruction)
{
    CollisionMask* m;

    EXPECT_NO_FATAL_FAILURE(m = new CollisionMask(1920, 1080, contour_IXI));
    EXPECT_NO_FATAL_FAILURE(delete m) ;
    EXPECT_NO_FATAL_FAILURE(m = nullptr);
}

TEST_F(SiCollisionMaskTest, set_bit)
{
    CollisionMask* m;

    EXPECT_NO_FATAL_FAILURE(m = new CollisionMask(1920, 1080, std::vector<Vector2<float>>()));
    EXPECT_FALSE((*m)[50]);
    EXPECT_NO_FATAL_FAILURE(m->set_bit(50));
    EXPECT_TRUE((*m)[50]);
    EXPECT_FALSE((*m)[1919 * 0 + 1000]);
    EXPECT_NO_FATAL_FAILURE(m->set_bit(Vector2<float>(1000, 0)));
    EXPECT_TRUE((*m)[1919 * 0 + 1000]);

    EXPECT_NO_FATAL_FAILURE(delete m) ;
    EXPECT_NO_FATAL_FAILURE(m = nullptr);
}

TEST_F(SiCollisionMaskTest, clear_bit)
{
    CollisionMask* m;

    EXPECT_NO_FATAL_FAILURE(m = new CollisionMask(1920, 1080, std::vector<Vector2<float>>()));

    EXPECT_NO_FATAL_FAILURE(m = new CollisionMask(1920, 1080, std::vector<Vector2<float>>()));
    EXPECT_FALSE((*m)[50]);
    EXPECT_NO_FATAL_FAILURE(m->set_bit(50));
    EXPECT_TRUE((*m)[50]);
    EXPECT_NO_FATAL_FAILURE(m->clear_bit(50));
    EXPECT_FALSE((*m)[50]);
    EXPECT_FALSE((*m)[1919 * 0 + 1000]);
    EXPECT_NO_FATAL_FAILURE(m->set_bit(Vector2<float>(1000, 0)));
    EXPECT_TRUE((*m)[1919 * 0 + 1000]);
    EXPECT_NO_FATAL_FAILURE(m->clear_bit(Vector2<float>(1000, 0)));
    EXPECT_FALSE((*m)[1919 * 0 + 1000]);

    EXPECT_NO_FATAL_FAILURE(delete m) ;
    EXPECT_NO_FATAL_FAILURE(m = nullptr);
}

TEST_F(SiCollisionMaskTest, test_bit_vector)
{
    CollisionMask* m;

    EXPECT_NO_FATAL_FAILURE(m = new CollisionMask(1920, 1080, std::vector<Vector2<float>>()));

    EXPECT_FALSE((*m)[1920 * 50 + 50]);
    EXPECT_NO_FATAL_FAILURE(m->set_bit(Vector2<float>(50, 50)));
    EXPECT_TRUE(m->test_bit(Vector2<float>(50, 50)));

    EXPECT_NO_FATAL_FAILURE(delete m) ;
    EXPECT_NO_FATAL_FAILURE(m = nullptr);
}

TEST_F(SiCollisionMaskTest, test_bit_index)
{
    CollisionMask* m;

    EXPECT_NO_FATAL_FAILURE(m = new CollisionMask(1920, 1080, std::vector<Vector2<float>>()));

    EXPECT_FALSE((*m)[50]);
    EXPECT_NO_FATAL_FAILURE(m->set_bit(50));
    EXPECT_TRUE(m->test_bit(50));

    EXPECT_NO_FATAL_FAILURE(delete m) ;
    EXPECT_NO_FATAL_FAILURE(m = nullptr);
}

TEST_F(SiCollisionMaskTest, overload_field_operator)
{
    CollisionMask* m;

    EXPECT_NO_FATAL_FAILURE(m = new CollisionMask(1920, 1080, std::vector<Vector2<float>>()));

    EXPECT_FALSE((*m)[50]);
    EXPECT_FALSE((*m)[65]);
    EXPECT_FALSE((*m)[80]);
    EXPECT_FALSE((*m)[95]);
    EXPECT_FALSE((*m)[110]);

    EXPECT_NO_FATAL_FAILURE(delete m) ;
    EXPECT_NO_FATAL_FAILURE(m = nullptr);
}
