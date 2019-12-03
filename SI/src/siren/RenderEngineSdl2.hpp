

#ifndef SITEST_RENDERENGINESDL2_HPP
#define SITEST_RENDERENGINESDL2_HPP


#include <sigrun/rendering/IRenderEngine.hpp>
#include <siren/display/Camera2D.hpp>
#include <siren/shader/GLSLProgram.hpp>
#include <siren/geometry/SpriteBatch.hpp>
#include <siren/display/Timing.hpp>

class SpriteBatch;

enum class STATE
{
    ON,
    OFF
};

typedef std::unique_ptr<Camera2D> Camera2D_ptr;
typedef std::unique_ptr<GLSLProgram> GLSLProgram_ptr;
typedef std::unique_ptr<SpriteBatch> SpriteBatch_ptr;

class RenderEngineSDL2: public IRenderEngine
{
public:
    RenderEngineSDL2();
    ~RenderEngineSDL2();

    void start(int width, int height) override;
    void run() override;
    void pause() override;

private:
    void draw();
    void init_shaders();

    float d_frame_time, d_time, d_fps, d_max_fps;
    int d_width, d_height;

    STATE d_state;

    const float DESIRED_FPS = 60.0f;
    const float MS_PER_SECOND = 1000.0f;
    const float DESIRED_FRAME_TIME = MS_PER_SECOND / DESIRED_FPS;
    const float MAX_DELTA_TIME = 1.0f;

    bool d_is_running = false;
    bool d_is_displaying_wireframe = false;

    GLSLProgram_ptr d_shader_program;
    Camera2D_ptr d_camera;
    SpriteBatch_ptr d_rsb;

    FPSLimiter d_fps_limiter;

protected:
};


#endif //SITEST_RENDERENGINESDL2_HPP
