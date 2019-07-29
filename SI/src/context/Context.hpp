

#ifndef SI_CONTEXT_HPP
#define SI_CONTEXT_HPP

#include <boost/python.hpp>
#include <vector>
#include "managers/PluginManager.hpp"
#include "managers/LayerManager.hpp"
#include "managers/RegionManager.hpp"

namespace bp = boost::python;

class Context
{
public:
    static LayerManager* layer_manager();
    static PluginManager* plugin_manager();

    static int width();
    static int height();

private:
    Context(int width, int height, const std::vector<bp::object>& plugins);
    ~Context();

    void begin();
    void end();

    void set_width(int width);
    void set_height(int height);

    static int s_width, s_height;

    friend class Core;

    static LayerManager* sp_lm;
    static PluginManager* sp_pm;
    static RegionManager* sp_rm;

protected:
};


#endif //SI_CONTEXT_HPP
