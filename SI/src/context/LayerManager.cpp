

#include <set>
#include "LayerManager.hpp"

std::map<int, Layer*> LayerManager::s_layers = std::map<int, Layer*>();
int LayerManager::s_consecutive_id = 0;
int LayerManager::s_active_layer_id = 0;


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

std::map<int, Layer*> &LayerManager::layers()
{
    return LayerManager::s_layers;
}

void LayerManager::add_layer()
{
    LayerManager::s_layers.insert(std::make_pair<int, Layer*>(LayerManager::s_consecutive_id++, new Layer));
}

void LayerManager::remove_layer(const int id)
{
    delete LayerManager::s_layers[id];
    LayerManager::s_layers[id] = nullptr;

    LayerManager::s_layers.erase(id);
}

void LayerManager::set_active_layer(int id)
{

}

int LayerManager::num_layers()
{
    return 0;
}

int LayerManager::consecutive_id()
{
    return 0;
}

int LayerManager::active_layer_id()
{
    return 0;
}

void LayerManager::clear()
{
    for(auto it = LayerManager::s_layers.rbegin(); it != LayerManager::s_layers.rend(); ++it)
    {
        delete it->second;
        it->second = nullptr;
    }

    LayerManager::s_layers.clear();
}
