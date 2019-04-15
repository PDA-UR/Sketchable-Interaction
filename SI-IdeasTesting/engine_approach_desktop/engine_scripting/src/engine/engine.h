
#ifndef ENGINE_SCRIPTING_ENGINE_H
#define ENGINE_SCRIPTING_ENGINE_H

#include <boost/python.hpp>
#include "../scripting/scripting.h"

class engine
{

public:
    engine() = default;
    ~engine() = default;

    void start();
    void run();
    void stop();

    void load_plugins();
    void add_region_plugin(boost::python::object pyObject);

private:
    bool is_running = false;
    double frame_time = 1 / 60.0;

    scripting script;
};


#endif //ENGINE_SCRIPTING_ENGINE_H
