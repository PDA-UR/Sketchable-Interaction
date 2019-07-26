

#ifndef SI_LAYERMANAGER_HPP
#define SI_LAYERMANAGER_HPP

#include <map>
#include <iterator>

#include "context/Layer.hpp"
#include <string>

class LayerManager
{
public:
    //static Layer* active_layer();
    //static Layer* next();
    //static Layer* previous();
    //static Layer* layer(const int id);

    static void add_layer();
    static void remove_layer(const int id);
    static void set_active_layer(int id);
    static void destroy();

    static int num_layers();
    static int consecutive_id();
    static int active_layer_id();

private:
    static std::map<int, Layer*> s_layers;
    static int s_consecutive_id;
    static int s_active_layer_id;
protected:
};


#endif //SI_LAYERMANAGER_HPP
