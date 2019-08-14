

#include "include/SiLayerManagerTest.hpp"
namespace SI
{

    TEST_F(SiLayerManagerTest, add_layer)
    {
        EXPECT_NO_FATAL_FAILURE(d_layer_manager.add_layer());

        EXPECT_EQ(1, d_layer_manager.num_layers());
    }

    TEST_F(SiLayerManagerTest, num_layers)
    {
        EXPECT_NO_FATAL_FAILURE(d_layer_manager.add_layer());

        EXPECT_NO_FATAL_FAILURE(d_layer_manager.num_layers());
        EXPECT_EQ(d_layer_manager.num_layers(), 1);
    }

    TEST_F(SiLayerManagerTest, active_layer_id_and_consecutive_id)
    {
        EXPECT_NO_FATAL_FAILURE(d_layer_manager.add_layer());

        EXPECT_NO_FATAL_FAILURE(d_layer_manager.active_layer_id());
        EXPECT_NO_FATAL_FAILURE(d_layer_manager.consecutive_id());

        EXPECT_EQ(0, d_layer_manager.active_layer_id());
        EXPECT_EQ(0, d_layer_manager.consecutive_id());
        d_layer_manager.add_layer();
        EXPECT_EQ(0, d_layer_manager.active_layer_id());
        EXPECT_EQ(1, d_layer_manager.consecutive_id());
    }

    TEST_F(SiLayerManagerTest, set_active_layer)
    {
        EXPECT_NO_FATAL_FAILURE(d_layer_manager.add_layer());
        EXPECT_NO_FATAL_FAILURE(d_layer_manager.add_layer());

        EXPECT_NO_FATAL_FAILURE(d_layer_manager.set_active_layer(d_layer_manager.consecutive_id()));
        d_layer_manager.set_active_layer(d_layer_manager.consecutive_id());

        EXPECT_NE(0, d_layer_manager.active_layer_id());
        EXPECT_EQ(1, d_layer_manager.active_layer_id());
    }

    TEST_F(SiLayerManagerTest, remove_layer)
    {
        EXPECT_NO_FATAL_FAILURE(d_layer_manager.add_layer());
        EXPECT_NO_FATAL_FAILURE(d_layer_manager.add_layer());
        EXPECT_NO_FATAL_FAILURE(d_layer_manager.add_layer());

        EXPECT_NO_FATAL_FAILURE(d_layer_manager.set_active_layer(1));

        EXPECT_EQ(3, d_layer_manager.num_layers());
        EXPECT_EQ(1, d_layer_manager.active_layer_id());
        EXPECT_EQ(2, d_layer_manager.consecutive_id());

        EXPECT_NO_FATAL_FAILURE(d_layer_manager.remove_layer(0));
        EXPECT_EQ(2, d_layer_manager.num_layers());
        EXPECT_EQ(1, d_layer_manager.active_layer_id());

        EXPECT_NO_FATAL_FAILURE(d_layer_manager.remove_layer(d_layer_manager.active_layer_id()));
        EXPECT_EQ(1, d_layer_manager.num_layers());
        EXPECT_EQ(2, d_layer_manager.active_layer_id());

        EXPECT_NO_FATAL_FAILURE(d_layer_manager.add_layer());
        EXPECT_NO_FATAL_FAILURE(d_layer_manager.add_layer());

        EXPECT_EQ(2, d_layer_manager.active_layer_id());
        EXPECT_EQ(3, d_layer_manager.num_layers());
        EXPECT_EQ(4, d_layer_manager.consecutive_id());

        EXPECT_NO_FATAL_FAILURE(d_layer_manager.set_active_layer(d_layer_manager.consecutive_id()));
        EXPECT_EQ(4, d_layer_manager.active_layer_id());
        EXPECT_NO_FATAL_FAILURE(d_layer_manager.remove_layer(d_layer_manager.active_layer_id()));
        EXPECT_EQ(3, d_layer_manager.active_layer_id());
        EXPECT_EQ(2, d_layer_manager.num_layers());
        EXPECT_NO_FATAL_FAILURE(d_layer_manager.remove_layer(d_layer_manager.active_layer_id()));
        EXPECT_EQ(1, d_layer_manager.num_layers());
        EXPECT_EQ(2, d_layer_manager.active_layer_id());
        EXPECT_NO_FATAL_FAILURE(d_layer_manager.remove_layer(d_layer_manager.active_layer_id()));
        EXPECT_EQ(2, d_layer_manager.active_layer_id());
    }

    TEST_F(SiLayerManagerTest, destroy)
    {
        EXPECT_NO_FATAL_FAILURE(d_layer_manager.add_layer());
        EXPECT_NO_FATAL_FAILURE(d_layer_manager.add_layer());
        EXPECT_NO_FATAL_FAILURE(d_layer_manager.add_layer());
        EXPECT_NO_FATAL_FAILURE(d_layer_manager.add_layer());
        EXPECT_NO_FATAL_FAILURE(d_layer_manager.add_layer());

        EXPECT_NO_FATAL_FAILURE(destroy());

        EXPECT_EQ(d_layer_manager.consecutive_id(), -1);
        EXPECT_EQ(d_layer_manager.active_layer_id(), -1);

        EXPECT_NO_FATAL_FAILURE(d_layer_manager.add_layer());
        EXPECT_NO_FATAL_FAILURE(d_layer_manager.add_layer());
        EXPECT_NO_FATAL_FAILURE(d_layer_manager.add_layer());
        EXPECT_NO_FATAL_FAILURE(d_layer_manager.add_layer());
        EXPECT_NO_FATAL_FAILURE(d_layer_manager.add_layer());

        EXPECT_EQ(0, d_layer_manager.active_layer_id());
        EXPECT_EQ(4, d_layer_manager.consecutive_id());
        EXPECT_EQ(5, d_layer_manager.num_layers());

        EXPECT_NO_FATAL_FAILURE(destroy());

        EXPECT_EQ(0, d_layer_manager.num_layers());
        EXPECT_EQ(d_layer_manager.consecutive_id(), -1);
        EXPECT_EQ(d_layer_manager.active_layer_id(), -1);
    }
}
