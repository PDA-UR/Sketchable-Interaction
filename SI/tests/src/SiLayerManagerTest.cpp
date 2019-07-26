

#include <context/managers/LayerManager.hpp>
#include "include/SiLayerManagerTest.hpp"

TEST_F(SiLayerManagerTest, add_layer)
{
    EXPECT_NO_FATAL_FAILURE(LayerManager::add_layer());

    EXPECT_EQ(1, LayerManager::num_layers());
}

TEST_F(SiLayerManagerTest, num_layers)
{
    EXPECT_NO_FATAL_FAILURE(LayerManager::num_layers());
    EXPECT_EQ(LayerManager::num_layers(), 1);
}

TEST_F(SiLayerManagerTest, active_layer_id_and_consecutive_id)
{
    EXPECT_NO_FATAL_FAILURE(LayerManager::active_layer_id());
    EXPECT_NO_FATAL_FAILURE(LayerManager::consecutive_id());

    EXPECT_EQ(0, LayerManager::active_layer_id());
    EXPECT_EQ(0, LayerManager::consecutive_id());
    LayerManager::add_layer();
    EXPECT_EQ(0, LayerManager::active_layer_id());
    EXPECT_EQ(1, LayerManager::consecutive_id());
}

TEST_F(SiLayerManagerTest, set_active_layer)
{
    EXPECT_NO_FATAL_FAILURE(LayerManager::set_active_layer(LayerManager::consecutive_id()));
    LayerManager::set_active_layer(LayerManager::consecutive_id());

    EXPECT_NE(0, LayerManager::active_layer_id());
    EXPECT_EQ(1, LayerManager::active_layer_id());
}

TEST_F(SiLayerManagerTest, remove_layer)
{
    LayerManager::add_layer();

    EXPECT_EQ(3, LayerManager::num_layers());
    EXPECT_EQ(1, LayerManager::active_layer_id());
    EXPECT_EQ(2, LayerManager::consecutive_id());

    EXPECT_NO_FATAL_FAILURE(LayerManager::remove_layer(0));
    EXPECT_EQ(2, LayerManager::num_layers());
    EXPECT_EQ(1, LayerManager::active_layer_id());

    EXPECT_NO_FATAL_FAILURE(LayerManager::remove_layer(LayerManager::active_layer_id()));
    EXPECT_EQ(1, LayerManager::num_layers());
    EXPECT_EQ(2, LayerManager::active_layer_id());

    EXPECT_NO_FATAL_FAILURE(LayerManager::add_layer());
    EXPECT_NO_FATAL_FAILURE(LayerManager::add_layer());

    EXPECT_EQ(2, LayerManager::active_layer_id());
    EXPECT_EQ(3, LayerManager::num_layers());
    EXPECT_EQ(4, LayerManager::consecutive_id());

    EXPECT_NO_FATAL_FAILURE(LayerManager::set_active_layer(LayerManager::consecutive_id()));
    EXPECT_EQ(4, LayerManager::active_layer_id());
    EXPECT_NO_FATAL_FAILURE(LayerManager::remove_layer(LayerManager::active_layer_id()));
    EXPECT_EQ(3, LayerManager::active_layer_id());
    EXPECT_EQ(2, LayerManager::num_layers());
    EXPECT_NO_FATAL_FAILURE(LayerManager::remove_layer(LayerManager::active_layer_id()));
    EXPECT_EQ(1, LayerManager::num_layers());
    EXPECT_EQ(2, LayerManager::active_layer_id());
    EXPECT_NO_FATAL_FAILURE(LayerManager::remove_layer(LayerManager::active_layer_id()));
    EXPECT_EQ(2, LayerManager::active_layer_id());
}

TEST_F(SiLayerManagerTest, destroy)
{
    EXPECT_NO_FATAL_FAILURE(LayerManager::destroy());
    EXPECT_EQ(LayerManager::consecutive_id(), -1);
    EXPECT_EQ(LayerManager::active_layer_id(), -1);

    EXPECT_NO_FATAL_FAILURE(LayerManager::add_layer());
    EXPECT_NO_FATAL_FAILURE(LayerManager::add_layer());
    EXPECT_NO_FATAL_FAILURE(LayerManager::add_layer());
    EXPECT_NO_FATAL_FAILURE(LayerManager::add_layer());
    EXPECT_NO_FATAL_FAILURE(LayerManager::add_layer());

    EXPECT_EQ(0, LayerManager::active_layer_id());
    EXPECT_EQ(4, LayerManager::consecutive_id());
    EXPECT_EQ(5, LayerManager::num_layers());

    EXPECT_NO_FATAL_FAILURE(LayerManager::destroy());

    EXPECT_EQ(0, LayerManager::num_layers());
    EXPECT_EQ(LayerManager::consecutive_id(), -1);
    EXPECT_EQ(LayerManager::active_layer_id(), -1);
}







