

#ifndef SITEST_RENDERENGINEQT5_HPP
#define SITEST_RENDERENGINEQT5_HPP

#include <SI/SI.hpp>
#include <QApplication>
#include <QMainWindow>
#include <siren/window/MainWindow.hpp>
#include <QGraphicsView>
#include <QGraphicsScene>

class RenderEngineQT5: public IRenderEngine, public SIObject
{Q_OBJECT SIREN
public:
    RenderEngineQT5();
    ~RenderEngineQT5();

    void start(uint32_t width, uint32_t height, uint32_t target_fps=60) override;
    void run() override;
    void pause() override;
    void stop() override;
    void disable_anti_aliasing() override;
    void enable_anti_aliasing(uint32_t samplng_factor) override;

    void set_cursor_stroke_width_by_cursor_id(const std::string &cursor_id, int stroke_width) override;
    void set_cursor_stroke_color_by_cursor_id(const std::string &cursor_id, const glm::vec4 &color) override;

private:
    MainWindow* d_window;

protected:
};

#endif //SITEST_RENDERENGINEQT5_HPP
