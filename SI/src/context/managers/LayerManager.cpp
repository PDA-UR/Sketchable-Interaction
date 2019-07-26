

#include <debug/Print.hpp>
#include "LayerManager.hpp"

std::map<int, Layer*> LayerManager::s_layers = std::map<int, Layer*>();
int LayerManager::s_consecutive_id = -1;
int LayerManager::s_active_layer_id = -1;

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
    LayerManager::s_consecutive_id++;

    LayerManager::s_layers.insert(std::make_pair<int, Layer*>(LayerManager::consecutive_id(), new Layer(LayerManager::consecutive_id())));

    if(LayerManager::active_layer_id() == -1)
        LayerManager::set_active_layer(LayerManager::consecutive_id());
}

void LayerManager::remove_layer(const int id)
{
    if(LayerManager::active_layer_id() == id)
        if(LayerManager::num_layers() > 1)
            if(LayerManager::s_layers.find(id) == LayerManager::s_layers.begin())
                LayerManager::set_active_layer((std::next(LayerManager::s_layers.find(id))->first));
            else if(std::next(LayerManager::s_layers.find(id)) == (LayerManager::s_layers.end()))
                LayerManager::set_active_layer((std::prev(LayerManager::s_layers.find(id))->first));

    delete LayerManager::s_layers[id];

    LayerManager::s_layers[id] = nullptr;
    LayerManager::s_layers.erase(id);
}

void LayerManager::set_active_layer(int id)
{
    LayerManager::s_active_layer_id = id;
}

int LayerManager::num_layers()
{
    return LayerManager::s_layers.size();
}

int LayerManager::consecutive_id()
{
    return LayerManager::s_consecutive_id;
}

int LayerManager::active_layer_id()
{
    return LayerManager::s_active_layer_id;
}

void LayerManager::destroy()
{
    for(auto it = LayerManager::s_layers.rbegin(); it != LayerManager::s_layers.rend(); ++it)
    {
        delete it->second;
        it->second = nullptr;
    }

    LayerManager::s_layers.clear();
    LayerManager::s_active_layer_id = -1;
    LayerManager::s_consecutive_id = -1;
}
