//
// Created by juergen on 22/01/19.
//

#ifndef CORE_ENGINE_H
#define CORE_ENGINE_H

#include <QObject>
#include <QtCore>
#include <QMainWindow>
#include "step.h"
#include "region.h"
#include <vector>
#include "canvas_region.h"
#include "mouse_region.h"
#include <chrono>

namespace si
{
    class Engine : public QMainWindow
    { Q_OBJECT

    public:
        static Engine *__instance__();

        void start(bool test=false);
        void stop();
        void add_region_template(region *r);
        void clear_active_regions();

        bool is_running();
        bool has_active_regions();

        const std::unique_ptr<step> &i_step() const;
        const std::unique_ptr<canvas> &i_main_canvas_region() const;

    private:
        std::unique_ptr<step> p_step;
        std::unique_ptr<canvas> main_canvas_region;

        bool d_is_running = false;

        std::chrono::high_resolution_clock::time_point d_engine_start;

        bool is_mouse_pressed = false;

        std::vector<std::unique_ptr<region>> d_active_regions;
        std::vector<std::unique_ptr<region>> d_region_blueprints;

    protected:
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

    protected:
        void keyPressEvent(QKeyEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
    };
}

#endif //CORE_ENGINE_H
