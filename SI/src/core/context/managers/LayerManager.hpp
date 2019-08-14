

#ifndef SI_LAYERMANAGER_HPP
#define SI_LAYERMANAGER_HPP

#include <map>
#include <iterator>

#include "core/runtime_core/region/Layer.hpp"
#include <string>
namespace SI
{
    class LayerManager
    {
    public:

        void add_layer();

        void remove_layer(const int id);

        void set_active_layer(int id);

        int num_layers() const;

        int consecutive_id() const;

        int active_layer_id() const;

    private:
        LayerManager();
        ~LayerManager();

        void destroy();


        std::map<int, Layer *> d_layers;
        int d_consecutive_id, d_active_layer_id;

        friend class SiLayerManagerTest;
        friend class Context;
    };
}

#endif //SI_LAYERMANAGER_HPP
