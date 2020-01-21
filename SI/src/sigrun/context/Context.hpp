

#ifndef SITEST_CONTEXT_HPP
#define SITEST_CONTEXT_HPP


#include <sigrun/SIObject.hpp>
#include "Capability.hpp"
#include <unordered_map>
#include <memory>
#include <boost/python.hpp>
#include <sigrun/context/managers/RegionManager.hpp>
#include <sigrun/context/managers/LinkingManager.hpp>
#include <sigrun/context/managers/InputManager.hpp>
#include <sigrun/rendering/IRenderEngine.hpp>

class Context: public SIObject
{

public:
    static Context* SIContext();

    ~Context();

    void begin(IRenderEngine* ire, int argc, char** argv);
    void end();

    Capability* capability_manager();
    RegionManager* region_manager();
    InputManager* input_manager();
    void update();
    void enable(int what);
    void disable(int what);

    int width();
    int height();

private:
    static Context* self;

    Context(int width, int height, const std::unordered_map<std::string, std::unique_ptr<bp::object>>& plugins);

    std::unique_ptr<Capability> upcm;
    std::unique_ptr<RegionManager> uprm;
    std::unique_ptr<LinkingManager> uplm;
    std::unique_ptr<InputManager> upim;

    int s_width, s_height;
    IRenderEngine* d_ire;

    friend class Core;
};


#endif //SITEST_CONTEXT_HPP
