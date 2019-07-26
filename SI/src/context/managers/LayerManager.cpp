

#include <debug/Print.hpp>
#include "LayerManager.hpp"

LayerManager::LayerManager() : d_consecutive_id(-1), d_active_layer_id(-1)
{

}

LayerManager::~LayerManager()
{
    destroy();
}

/*
Layer* LayerManager::active_layer()
{
    return LayerManager::s_layers[LayerManager::s_active_layer_id];
}

Layer* LayerManager::next()
{
    std::map<int, Layer*>::iterator current = LayerManager::s_layers.find(LayerManager::s_active_layer_id);

    if(++current != LayerManager::s_layers.end())
        return current->second;

    return (--current)->second;
}

Layer* LayerManager::previous()
{
    std::map<int, Layer*>::iterator current = LayerManager::s_layers.find(LayerManager::s_active_layer_id);

    if(--current != LayerManager::s_layers.begin())
        return current->second;

    return (++current)->second;
}

Layer* LayerManager::layer(const int id)
{
    return LayerManager::s_layers[id];
}
*/

void LayerManager::add_layer()
{
    d_consecutive_id++;

    d_layers.insert(std::make_pair<int, Layer*>(consecutive_id(), new Layer(consecutive_id())));

    if(active_layer_id() == -1)
        set_active_layer(LayerManager::consecutive_id());
}

void LayerManager::remove_layer(const int id)
{
    if(active_layer_id() == id)
        if(num_layers() > 1)
            if(d_layers.find(id) == d_layers.begin())
                set_active_layer((std::next(d_layers.find(id))->first));
            else if(std::next(d_layers.find(id)) == (d_layers.end()))
                set_active_layer((std::prev(d_layers.find(id))->first));

    delete d_layers[id];

    d_layers[id] = nullptr;
    d_layers.erase(id);
}

void LayerManager::set_active_layer(int id)
{
    d_active_layer_id = id;
}

int LayerManager::num_layers() const
{
    return d_layers.size();
}

int LayerManager::consecutive_id() const
{
    return d_consecutive_id;
}

int LayerManager::active_layer_id() const
{
    return d_active_layer_id;
}

void LayerManager::destroy()
{
    for(auto it = d_layers.rbegin(); it != d_layers.rend(); ++it)
    {
        delete it->second;
        it->second = nullptr;
    }

    d_layers.clear();
    d_active_layer_id = -1;
    d_consecutive_id = -1;
}
