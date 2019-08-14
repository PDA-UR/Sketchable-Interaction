

#ifndef SI_CONTEXT_HPP
#define SI_CONTEXT_HPP

#include <boost/python.hpp>
#include <vector>
#include <core/render_engine_core/IRenderEngine.hpp>
#include <thread>
#include "core/context/managers/PluginManager.hpp"
#include "core/context/managers/LayerManager.hpp"
#include "core/context/managers/RegionManager.hpp"


namespace SI
{
    namespace bp = boost::python;

    class Context
    {
    public:
        static LayerManager *layer_manager();
        static PluginManager *plugin_manager();
        static RegionManager *region_manager();

        static int width();
        static int height();

    private:
        Context(int width, int height, const std::vector<bp::object> &plugins);

        ~Context();

        void begin(IRenderEngine *ire);

        void end();

        void set_width(int width);

        void set_height(int height);

        static int s_width, s_height;

        friend class Core;

        static LayerManager *sp_lm;
        static PluginManager *sp_pm;
        static RegionManager *sp_rm;

        IRenderEngine *p_renderer;
        std::thread *p_render_thread;

    protected:
    };
}

#endif //SI_CONTEXT_HPP
