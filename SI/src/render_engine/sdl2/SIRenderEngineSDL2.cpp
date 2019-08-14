

#include <core/debug/Print.hpp>
#include <render_engine/debug/SiRenPrint.hpp>
#include <render_engine/sdl2/io/ResourceManager.hpp>
#include "SIRenderEngineSDL2.hpp"
#include "render_engine/sdl2/region_sprite/RegionSprite.hpp"
#include "render_engine/sdl2/shader/GLSLProgram.hpp"


namespace SIRen
{
    SIRenderEngineSDL2::SIRenderEngineSDL2() :
            d_width(800), d_height(600), d_time(0), d_frame_time(0), d_state(STATE::ON), d_max_fps(60.0)
    {
    }

    SIRenderEngineSDL2::~SIRenderEngineSDL2()
    {
        SDL_Quit();
    }

    void SIRenderEngineSDL2::start(int width, int height)
    {
        d_width = width;
        d_height = height;
        d_state = STATE::ON;

        d_camera.initialize(d_width, d_height);

        SDL_Init(SDL_INIT_EVERYTHING);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        d_window.create("SI", width, height, FULLSCREEN);

        initialize_shaders();
        d_sprite_batch.initialize();
        run();
    }

    void SIRenderEngineSDL2::run()
    {
        d_is_running = true;

        while (!is_stop_requested() && d_state == STATE::ON)
        {
            float start_ticks = SDL_GetTicks();

            const float cam_speed = 20.0f;
            const float cam_scale = 0.1f;

            SDL_Event ev;
            while (SDL_PollEvent(&ev))
            {
                switch (ev.type)
                {
                    case SDL_QUIT:
                        d_state = STATE::OFF;
                        set_is_running(false);
                    break;

                    case SDL_KEYDOWN:
                        switch(ev.key.keysym.sym)
                        {
                            case SDLK_ESCAPE:
                                d_state = STATE::OFF;
                                set_is_running(false);
                            break;

                            case SDLK_w:
                                d_camera.set_position(d_camera.position() + glm::vec2(0.0f, cam_speed));
                            break;

                            case SDLK_s:
                                d_camera.set_position(d_camera.position() + glm::vec2(0.0f, -cam_speed));
                            break;

                            case SDLK_a:
                                d_camera.set_position(d_camera.position() + glm::vec2(-cam_speed, 0.0f));
                            break;

                            case SDLK_d:
                                d_camera.set_position(d_camera.position() + glm::vec2(cam_speed, 0.0f));
                            break;

                            case SDLK_q:
                                d_camera.set_scale(d_camera.scale() + cam_scale);
                            break;

                            case SDLK_e:
                                d_camera.set_scale(d_camera.scale() - cam_scale);
                            break;
                        }
                    break;
                }
            }

            d_time += 0.01;

            d_camera.update();

            draw();
            calculate_fps();
            static int frame_counter = 0;
            frame_counter++;
            if (frame_counter == 10)
            {
                SIRenPrint::print(d_fps);
                frame_counter = 0;
            }

            float frame_ticks = SDL_GetTicks() - start_ticks;

            // limit fps to max fps
            if (1000.0f / d_max_fps > frame_ticks)
                SDL_Delay(1000.0f / d_max_fps - frame_ticks);
        }
    }

    void SIRenderEngineSDL2::pause()
    {

    }

    void SIRenderEngineSDL2::draw()
    {
        glClearDepth(1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        d_color_shader_program.use();
        glActiveTexture(GL_TEXTURE0);

        GLint texture_location = d_color_shader_program.uniform_location("mysampler");
        glUniform1i(texture_location, 0);

        GLuint time_location = d_color_shader_program.uniform_location("time");
        glUniform1f(time_location, d_time);

        // set the camera matrix
        GLuint p_location = d_color_shader_program.uniform_location("P");
        glm::mat4 camera_matrix = d_camera.camera_matrix();
        glUniformMatrix4fv(p_location, 1, GL_FALSE, &(camera_matrix[0][0]));

        d_sprite_batch.begin();

        glm::vec4 pos(0, 0, 50, 50);
        glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
        float depth = 0.0f;
        GLuint texture = ResourceManager::texture("src/render_engine/sdl2/res/textures/jimmyJump_pack/PNG/CharacterRight_Standing.png").id;
        Color color;
        color.r = 255;
        color.g = 255;
        color.b = 255;
        color.a = 255;

        d_sprite_batch.draw(pos, uv, texture, depth, color);

        d_sprite_batch.end();
        d_sprite_batch.render_batch();

        glBindTexture(GL_TEXTURE_2D, 0);

        d_color_shader_program.unuse();

        d_window.swap_buffer();
    }

    void SIRenderEngineSDL2::initialize_shaders()
    {
        d_color_shader_program.compile_shaders("src/render_engine/sdl2/res/shaders/color_shading.vert",
                                             "src/render_engine/sdl2/res/shaders/color_shading.frag");
        d_color_shader_program.add_attribute("vertex_position");
        d_color_shader_program.add_attribute("vertex_color");
        d_color_shader_program.add_attribute("vertex_uv");
        d_color_shader_program.link_shaders();
    }

    void SIRenderEngineSDL2::calculate_fps()
    {
        static const int NUM_SAMPLES = 100;
        static float frame_times[NUM_SAMPLES];
        static int current_frame = 0;

        static float prev_ticks = SDL_GetTicks();
        float current_ticks;

        current_ticks = SDL_GetTicks();

        d_frame_time = current_ticks - prev_ticks;
        prev_ticks = current_ticks;

        frame_times[current_frame % NUM_SAMPLES] = d_frame_time;

        int count;
        current_frame++;

        if (current_frame < NUM_SAMPLES)
            count = current_frame;
        else
            count = NUM_SAMPLES;

        float average_frame_time = 0;

        for (int i = 0; i < count; i++)
            average_frame_time += frame_times[i];

        average_frame_time /= count;

        if (average_frame_time > 0)
            d_fps = 1000.0f / average_frame_time;
        else
            d_fps = 0.0f;
    }
}