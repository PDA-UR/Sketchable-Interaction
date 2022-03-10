

#ifndef SITEST_IRENDERENGINE_HPP
#define SITEST_IRENDERENGINE_HPP

#include <QObject>
#include <glm/glm.hpp>

class IRenderEngine: public QObject
{Q_OBJECT
public:
    virtual void start(uint32_t width, uint32_t height, uint32_t target_fps=60) = 0;
    virtual void run() = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;
    virtual void set_cursor_stroke_width_by_cursor_id(const std::string& cursor_id, int stroke_width) = 0;
    virtual void set_cursor_stroke_color_by_cursor_id(const std::string& cursor_id, const glm::vec4& color) = 0;

    virtual void disable_anti_aliasing() = 0;
    virtual void enable_anti_aliasing(uint32_t samplng_factor) = 0;
};


#endif //SITEST_IRENDERENGINE_HPP
