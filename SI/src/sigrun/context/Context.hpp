

#ifndef SITEST_CONTEXT_HPP
#define SITEST_CONTEXT_HPP


#include <sigrun/SIObject.hpp>
#include "Capability.hpp"
#include <unordered_map>
#include <memory>
#include <boost/python.hpp>
#include <sigrun/context/managers/RegionManager.hpp>
#include <sigrun/context/managers/LinkingManager.hpp>
#include <sigrun/concurrency/RenderWorker.hpp>
#include <QThread>
#include <QObject>

class Context: public QObject, public SIObject
{Q_OBJECT

public:
    static Context* SIContext();

    ~Context();

    void begin(IRenderEngine* ire, int argc, char** argv);

    Capability* capability_manager();
    RegionManager* region_manager();
    void update();
    void enable(int what);
    void disable(int what);

    int width();
    int height();

private:
    static Context* self;

    Context(int width, int height, const std::unordered_map<std::string, std::unique_ptr<bp::object>>& plugins);

    void initialize_rendering_worker();

    std::unique_ptr<Capability> upcm;
    std::unique_ptr<RegionManager> uprm;
    std::unique_ptr<LinkingManager> uplm;

    int s_width, s_height;

    QThread d_render_thread;
    RenderWorker* d_render_worker;

    friend class Core;
};


#endif //SITEST_CONTEXT_HPP
