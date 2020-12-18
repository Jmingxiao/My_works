// main.cc

#include "main.hpp"
#define SCR_WIDTH  1920
#define SCR_HEIGHT 1080
namespace avocado {
    // note: camera


    // note: application create implementation
    application* application::create(settings& settings)
    {
        settings.title_ = "renderapp";
        settings.width_ = SCR_WIDTH;
        settings.height_ = SCR_HEIGHT;
        settings.center_ = true;
        //settings.borderless_ = true;

        return new renderapp;
    }

    // note: renderapp class
    renderapp::renderapp()
        : controller_(camera_)
    {
    }

    bool renderapp::on_init()
    {
        if (!renderer_.initialize()) {
            return false;
        }

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, 1000.0f);
        glm::vec3 view = glm::vec3(4.0f, 15.0f, 5.0f);
        camera_.set_projection(projection);
        camera_.set_position(view);

        if (!heightmap.Init()) {
        
            return on_error("no good");
        }
        return true;
    }

    void renderapp::on_exit()
    {
    }

    bool renderapp::on_tick(const time& deltatime)
    {
        if (keyboard_.key_pressed(keyboard::key::escape)) {
            return false;
        }
        if (keyboard_.key_pressed(keyboard::key::space)) {
            if (heightmap.drawmode == triangles) {
                heightmap.drawmode = wireframe;
            }
            else
            {
                heightmap.drawmode = triangles;
            }
        }
        
        controller_.update(keyboard_, mouse_, deltatime);
        heightmap.camera_ = std::move(camera_);
        heightmap.frustum_.construct(glm::transpose(camera_.projection_ * camera_.view_));


        return true;
    }

    void renderapp::on_draw()
    {
        heightmap.OnDraw();
    }
} // !avocado
