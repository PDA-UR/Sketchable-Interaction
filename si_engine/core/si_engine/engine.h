//
// Created by juergen on 22/01/19.
//

#ifndef CORE_ENGINE_H
#define CORE_ENGINE_H

#include <QApplication>
#include <QObject>
#include <QtCore>
#include <QMainWindow>
#include "../si_plugin/plugin.h"
#include "step.h"

namespace si
{
    class Engine : public QMainWindow
    { Q_OBJECT

    public:
        static Engine *__instance__();

        void start();
        void stop();
        void add_plugin(const plugin &plugin);

    private:
        step *p_step;

        static Engine *s_instance;

        Engine();
        Engine(const Engine &);
        ~Engine() override;

        class CGuard
        {
        public:
            ~CGuard()
            {
                if(Engine::s_instance != nullptr)
                {
                    delete Engine::s_instance;
                    Engine::s_instance = nullptr;
                }
            }
        };

    private slots:
        void on_step();
    };


}

#endif //CORE_ENGINE_H
