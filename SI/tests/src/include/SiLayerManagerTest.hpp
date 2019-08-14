

#ifndef SI_SILAYERMANAGERTEST_HPP
#define SI_SILAYERMANAGERTEST_HPP

#include <boost/python.hpp>

namespace bp = boost::python;

#include <gtest/gtest.h>
#include <core/context/managers/LayerManager.hpp>

namespace SI
{
    class SiLayerManagerTest : public ::testing::Test
    {
    public:
        virtual void SetUp()
        {

        }

        virtual void TearDown()
        {

        }

        SI::LayerManager d_layer_manager;
    private:
    protected:
        void destroy()
        {
            d_layer_manager.destroy();
        }
    };
}

#endif //SI_SILAYERMANAGERTEST_HPP
