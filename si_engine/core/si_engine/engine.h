//
// Created by juergen on 22/01/19.
//

#ifndef CORE_ENGINE_H
#define CORE_ENGINE_H

#include "timing.h"

namespace si
{
    class engine
    {
    public:
        static  engine *__instance__();

        void start();
        void pause();
        void stop();
        void resume();
        void register_plugin(void * plugin);
    private:
        static engine *s_instance;

        engine();
        engine(const engine &);
        ~engine();

        void run();

        static const double sc_DESIRED_FPS;
        double d_frame_time;

        bool d_is_running;

        class CGuard
        {
        public:
            ~CGuard()
            {
                if(engine::s_instance != nullptr)
                {
                    delete engine::s_instance;
                    engine::s_instance = nullptr;
                }
            }
        };
    };
}

#endif //CORE_ENGINE_H
