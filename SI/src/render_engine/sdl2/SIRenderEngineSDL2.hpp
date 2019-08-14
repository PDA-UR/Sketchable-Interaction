

#ifndef SI_SIRENDERENGINESDL2_HPP
#define SI_SIRENDERENGINESDL2_HPP

#include "core/render_engine_core/IRenderEngine.hpp"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <render_engine/sdl2/shader/GLSLProgram.hpp>
#include "render_engine/sdl2/region_sprite/RegionSprite.hpp"
#include "render_engine/sdl2/geometry/GLTexture.hpp"
#include <vector>
#include <render_engine/sdl2/display/Window.hpp>
#include <render_engine/sdl2/display/Camera2D.hpp>
#include <render_engine/sdl2/geometry/SpriteBatch.hpp>

enum class STATE
{
    ON,
    OFF
};

class RegionSprite;

namespace SIRen
{
class SIRenderEngineSDL2 : public SI::IRenderEngine
    {
    public:
        SIRenderEngineSDL2();

        ~SIRenderEngineSDL2();

        void run() override;

        void start(int width, int height) override;

        void pause() override;

    private:
        void draw();

        void initialize_shaders();

        void calculate_fps();

        STATE d_state;
        float d_frame_time, d_time, d_fps, d_max_fps;
        int d_width, d_height;
        Window d_window;

        GLSLProgram d_color_shader_program;
        Camera2D d_camera;
        SpriteBatch d_sprite_batch;
    };
}


#endif //SI_SIRENDERENGINESDL2_HPP
