

#ifndef SI_CONTEXT_HPP
#define SI_CONTEXT_HPP

#include <boost/python.hpp>
#include <vector>
#include "managers/PluginManager.hpp"
#include "managers/LayerManager.hpp"

namespace bp = boost::python;

class Context
{
public:
    int width() const;
    int height() const;

    static LayerManager* layer_manager();
    static PluginManager* plugin_manager();

private:
    Context(int width, int height, const std::vector<bp::object>& plugins);
    ~Context();

    void begin();
    void end();

    void set_width(int width);
    void set_height(int height);

    int d_width, d_height;

    friend class Core;

    static LayerManager* s_lm;
    static PluginManager* s_pm;

protected:
};


#endif //SI_CONTEXT_HPP
