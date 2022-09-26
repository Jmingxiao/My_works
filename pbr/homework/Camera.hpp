#pragma once

#include <bx/uint32_t.h>
#include "common.h"
#include "bgfx_utils.h"
#include <cmath>
#include <stdexcept>
#include "entry/input.h"

const float pi = std::acos(-1);

enum CameraDir {
    Forward ,
    Backward ,
    Left ,
    Right ,
    None
};

inline CameraDir operator|(CameraDir a, CameraDir b)
{
    return static_cast<CameraDir>(static_cast<int>(a) | static_cast<int>(b));
}



class Camera
{
private:

    float pitch_{};
    float yaw_{90.0f};
    float speed{ 5.0f };
    
    bx::Vec3 right{ 1.0f, 0.0f, 0.0f };
    bx::Vec3 up{ 0.0f, 1.0f, 0.0f };
    bx::Vec3 front{ 0.0f, 0.0f, 1.0f };

public:


    void update() noexcept
    {
        bx::Vec3 dir(1.0f);
        float d2r = pi / 180.0f;

        dir.x = cos(yaw_ *d2r) * cos(pitch_ *d2r);
        dir.y = sin(pitch_ *d2r);
        dir.z = sin(yaw_ *d2r) * cos(pitch_ *d2r);
        front = bx::normalize(dir);
        const auto worldUp = bx::Vec3(0.0f, 1.0f, 0.0f);
        right = bx::normalize(bx::cross(front, worldUp));
        up = bx::normalize(bx::cross(right, front));  
    }

    void getviewmtx(float*view) {
        bx::mtxLookAt(view, position_, bx::add(position_, front), up);
    }
    void set_projection(const float* projection )
    {
        memcpy(projection_, projection, sizeof(projection));
    }

    void set_position(const bx::Vec3& position) noexcept
    {
        position_ = position;
    }

    void rotate_x(const float amount)
    {
        pitch_ += amount;
        pitch_ = std::min(pitch_, 80.0f);
        pitch_ = std::max(pitch_, -80.0f);
    }

    void rotate_y(const float amount)
    {
        yaw_ += amount;
    }

    void zoom(const float amount) {
       position_ =bx::mad(front,amount,position_);
    }

    void move(CameraDir dir,float dt) {
        
        float velocity = dt * speed;

        switch (dir)
        {
        case Forward:
            position_ = bx::mad(front, velocity, position_);
            break;
        case Backward:
            position_ = bx::nms(front, velocity, position_);
            break;
        case Left:
            position_ = bx::mad(right, velocity, position_);
            break;
        case Right:
            position_ = bx::nms(right, velocity, position_);
            break;
        default:
            break;
        }
    }


    bx::Vec3 position_{1.0f,2.0f,-5.0f};
	float projection_[16]{};
};

struct Controller
{
    Controller(Camera& camera) :
        cam(camera) {

        
    }

    void Update(entry::MouseState& mouse, float deltatime) {

        if (firstpress) {
            previous_mouse_position_.x = mouse.m_mx;
            previous_mouse_position_.y = mouse.m_my;
            previous_mouse_position_.z = mouse.m_mz;
            firstpress = false;
        }

        float offset_x = mouse.m_mx - previous_mouse_position_.x;
        float offset_y = mouse.m_my - previous_mouse_position_.y;
        float offset_z = mouse.m_mz - previous_mouse_position_.z;

        previous_mouse_position_.x = mouse.m_mx;
        previous_mouse_position_.y = mouse.m_my;
        previous_mouse_position_.z = mouse.m_mz;

        mouse_down = !!mouse.m_buttons[entry::MouseButton::Right];

        if (mouse_down) {
            
            offset_y *= mouse_sensitivity_;
            offset_x *= mouse_sensitivity_;

            cam.rotate_y(-offset_x);
            cam.rotate_x(-offset_y);
            cam.update();
        }

        if (offset_z > 0) {
            std::cout << offset_z;
        }
        cam.zoom(offset_z);

        CameraDir dir{None};
        dir = inputGetKeyState(entry::Key::KeyW) ? Forward : dir;
        dir = inputGetKeyState(entry::Key::KeyS) ? Backward : dir;
        dir = inputGetKeyState(entry::Key::KeyA) ? Left : dir;
        dir = inputGetKeyState(entry::Key::KeyD) ? Right : dir;
        cam.move(dir,deltatime);

    }
    



    uint8_t m_keys{};
    Camera& cam;
    bool mouse_down{};
    bool firstpress{true};
    float camera_speed_{5.0f};
    float mouse_yaw_{0.0f};
    float mouse_pitch_{0.0f};
    float mouse_sensitivity_{0.05f};
    bx::Vec3 previous_mouse_position_{0};
};
