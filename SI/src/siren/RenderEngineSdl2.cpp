

#include <debug/Print.hpp>
#include "RenderEngineSdl2.hpp"
#include <SDL2/SDL.h>
#include <siren/geometry/SpriteBatch.hpp>
#include <siren/geometry/Tessellator.hpp>
#include <siren/display/Timing.hpp>

RenderEngineSDL2::RenderEngineSDL2():
        d_width(800),
        d_height(600),
        d_time(0),
        d_state(STATE::ON),
        d_max_fps(25)
{

}

RenderEngineSDL2::~RenderEngineSDL2()
{
    for(auto& [key, value]: d_region_representations)
    {
        for(auto& it: value->patches)
        {
            delete it;
            it = nullptr;
        }

        value->patches.clear();
        delete value;
        value = nullptr;
    }

    d_region_representations.clear();

    SDL_Quit();
}

void RenderEngineSDL2::start(int width, int height)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    d_width = width;
    d_height = height;

    d_window.create("SI", width, height, SDL_WINDOW_OPENGL);

    d_camera = std::make_unique<Camera2D>();
    d_shader_program = std::make_unique<GLSLProgram>();
    d_rsb = std::make_unique<SpriteBatch>();

    init_shaders();
    d_camera->initialize(d_width, d_height);
    d_rsb->initialize();

    run();
}

void RenderEngineSDL2::run()
{
    double last_time = Time::get_time();
    double unprocessed_time = 0.0;
    double frame_counter = 0.0;
    int frames = 0;

    while (!is_stop_requested() && d_state == STATE::ON)
    {
        bool render = false;

        double start_time = Time::get_time();
        double passed_time = start_time - last_time;
        last_time = start_time;

        unprocessed_time += passed_time;
        frame_counter += passed_time;

        if(frame_counter > 1.0)
        {
//            std::cout << "Current FPS: " << frames << "\n";
            frame_counter = 0.0;
            frames = 0;
        }

        while(unprocessed_time > 1 / d_max_fps)
        {
            render = true;

            Time::set_time_delta(1 / d_max_fps);

            d_time += 0.01;
            d_camera->update();

            SDL_Event ev;
            while (SDL_PollEvent(&ev))
            {
                switch (ev.type)
                {
                    case SDL_QUIT:
                        d_state = STATE::OFF;
                        set_is_running(false);
                        break;
                }
            }

            unprocessed_time -= 1 / d_max_fps;
        }

        if(render)
        {
            Context::SIContext()->update();

            draw();
            frames++;
        }
    }
}

void RenderEngineSDL2::pause()
{
    Print::print("pause");
}

void RenderEngineSDL2::draw()
{
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const auto& regions = Context::SIContext()->region_manager()->regions();

    bool is_present = false;
    for(const auto& [key, val]: d_region_representations)
    {
        is_present = false;

        for(const auto& region: regions)
        {
            if(key == region->uuid())
            {
                is_present = true;
                break;
            }
        }

        if(!is_present)
        {
            for(auto& it : d_region_representations[key]->patches)
            {
                delete it;
                it = nullptr;
            }

            d_region_representations[key]->patches.clear();

            delete d_region_representations[key];
            d_region_representations[key] = nullptr;

            d_region_representations.erase(key);
        }
    }

    for(const auto& region: regions)
    {
        if(d_region_representations.find(region->uuid()) == d_region_representations.end())
        {
            std::vector<glm::vec3> patch_vertices;
            Tessellator::tesselate(patch_vertices, region->contour());

            d_region_representations.insert({region->uuid(), new RegionRepresentation(
                    std::vector<TesselationPatch*>(),
                    region->transform(),
                    region->texture_path(),
                    region->contour().size(),
                    glm::vec4(region->aabb()[0].x + (region->aabb()[3].x - region->aabb()[0].x) / 2 - 80 / 2, region->aabb()[0].y + (region->aabb()[1].y - region->aabb()[0].y) / 2 - 80 / 2, 80, 80),
                    glm::vec4(0, 0, 1, 1))});

            for (size_t i = 2; i < patch_vertices.size(); i += 3)
                d_region_representations[region->uuid()]->patches.emplace_back(new TesselationPatch(patch_vertices[i - 2], patch_vertices[i - 1], patch_vertices[i]));
        }
        else
        {
            if(region->is_transformed())
            {
                d_region_representations[region->uuid()]->transform = region->transform();
                d_region_representations[region->uuid()]->destination_rect = glm::vec4(region->aabb()[0].x + (region->aabb()[3].x - region->aabb()[0].x) / 2 - 80 / 2, region->aabb()[0].y + (region->aabb()[1].y - region->aabb()[0].y) / 2 - 80 / 2, 80, 80);
            }
        }
    }

    if(!d_region_representations.empty())
    {
        std::vector<glm::vec2> partial_contour;
        d_rsb->render(d_region_representations, partial_contour, &(d_camera->camera_matrix()[0][0]));
    }

    d_window.swap_buffer();
}

void RenderEngineSDL2::init_shaders()
{
    d_shader_program->compile_shaders("src/siren/res/shaders/color_shading.vert","src/siren/res/shaders/color_shading.frag");
    d_shader_program->add_attribute("vertex_position");
    d_shader_program->add_attribute("vertex_color");
    d_shader_program->link_shaders();
}
