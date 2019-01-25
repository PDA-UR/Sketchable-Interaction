//
// Created by juergen on 22/01/19.
//

#include "engine.h"
#include "../si_debug/debug.h"
#include "signal_handler.h"

namespace si
{
    /* Engine */

    /* PUBLIC */
    Engine * Engine::s_instance = nullptr;

    Engine *Engine::__instance__()
    {
        static CGuard guard;

        if(!s_instance)
            s_instance = new Engine();

        return s_instance;
    }

    void Engine::start()
    {
        connect(p_step, &step::on_step, this, &Engine::on_step);
        p_step->start();
        show();
    }

    void Engine::stop()
    {
        close();
    }

    void Engine::add_plugin(const plugin &plugin)
    {
        // transform plugin to region or trackable
        // add region or trackable to its dedicated datastructure where all are stored

        if (plugin.type() == "region")
        {
            // appears to be the most common type
        }
        else if(plugin.type() == "trackable")
        {


            //debug::print(i);

            //plugin.on_enter(i);

            plugin.on_enter(5);

            plugin.on_leave(this->findChild<QWidget *>("hello")->winId());
            //plugin.on_continuous();
            //plugin.on_destroy();
        }
    }

    /* PRIVATE */

    Engine::Engine() : QMainWindow(), p_step(new step(33.0))
    {
        signal(SIGINT, signal_handler::handle);
    }

    Engine::Engine(const Engine &) : QMainWindow(), p_step(new step(33.0))
    {
        signal(SIGINT, signal_handler::handle);
    }

    Engine::~Engine()
    {
        debug::print("called");

        p_step->stop();

        while (!p_step->isFinished())
        {
        }

        delete p_step;
        p_step = nullptr;
        debug::print("called");

    }

    /* SLOT */
    void Engine::on_step()
    {
        debug::print("Hello");
    }

    /* End of Engine */
}