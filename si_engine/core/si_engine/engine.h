//
// Created by juergen on 22/01/19.
//

#ifndef CORE_ENGINE_H
#define CORE_ENGINE_H

#include <QApplication>
#include <QObject>
#include <QtCore>
#include <QMainWindow>
#include "step.h"
#include "../si_interaction/region.h"
#include <vector>
#include "../si_stdlib/canvas_region.h"
#include "../si_stdlib/mouse_region.h"
#include <QGridLayout>

namespace si
{
    class Engine : public QMainWindow
    { Q_OBJECT

    public:
        static Engine *__instance__();

        void start();
        void stop();

        void add_region_template(region *r);

    private:
        step *p_step;
        canvas *main_canvas_region;

        static Engine *s_instance;

        bool is_mouse_pressed = false;

        std::vector<std::unique_ptr<region>> d_active_regions;

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

    protected:
        void keyPressEvent(QKeyEvent *event) override;

        void mousePressEvent(QMouseEvent *event) override;

        void mouseReleaseEvent(QMouseEvent *event) override;

        void mouseMoveEvent(QMouseEvent *event) override;

    protected:

    };
}

#endif //CORE_ENGINE_H
