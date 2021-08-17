#pragma once
#include"glm/gtc/matrix_transform.hpp"
#include"glm/glm.hpp"

struct frustum {
    enum class side {
        left,
        right,
        top,
        bottom,
        near,
        far,
        count,
    };

    void construct(const glm::mat4& projview) {

        planes_[int(side::left)].x = projview[3][0] + projview[0][0];
        planes_[int(side::left)].y = projview[3][1] + projview[0][1];
        planes_[int(side::left)].z = projview[3][2] + projview[0][2];
        planes_[int(side::left)].w = projview[3][3] + projview[0][3];

        planes_[int(side::right)].x = projview[3][0] - projview[0][0];
        planes_[int(side::right)].y = projview[3][1] - projview[0][1];
        planes_[int(side::right)].z = projview[3][2] - projview[0][2];
        planes_[int(side::right)].w = projview[3][3] - projview[0][3];

        planes_[int(side::bottom)].x = projview[3][0] + projview[1][0];
        planes_[int(side::bottom)].y = projview[3][1] + projview[1][1];
        planes_[int(side::bottom)].z = projview[3][2] + projview[1][2];
        planes_[int(side::bottom)].w = projview[3][3] + projview[1][3];

        planes_[int(side::top)].x = projview[3][0] - projview[1][0];
        planes_[int(side::top)].y = projview[3][1] - projview[1][1];
        planes_[int(side::top)].z = projview[3][2] - projview[1][2];
        planes_[int(side::top)].w = projview[3][3] - projview[1][3];

        planes_[int(side::near)].x = projview[3][0] + projview[2][0];
        planes_[int(side::near)].y = projview[3][1] + projview[2][1];
        planes_[int(side::near)].z = projview[3][2] + projview[2][2];
        planes_[int(side::near)].w = projview[3][3] + projview[2][3];

        planes_[int(side::far)].x = projview[3][0] - projview[2][0];
        planes_[int(side::far)].y = projview[3][1] - projview[2][1];
        planes_[int(side::far)].z = projview[3][2] - projview[2][2];
        planes_[int(side::far)].w = projview[3][3] - projview[2][3];

        for (int i = 0; i < int(side::count); i++) {
            glm::vec3 normal = glm::vec3(planes_[i]);
            const float length = glm::length(normal);
            planes_[i].x = normal.x / length;
            planes_[i].y = normal.y / length;
            planes_[i].z = normal.z / length;
            planes_[i].w /= length;
        }

    }
    bool is_inside(const glm::vec3& position) const {
        for (int index = 0; index < int(side::count); index++) {
            const glm::vec3 normal(planes_[index]);
            const float d = planes_[index].w;
            const float distance = glm::dot(normal, position) + d;
            if (distance < 0.0f) {
                return false;
            }
        }

        return true;


    }

    // Ax + By + Cz + D = 0
    glm::vec4 planes_[int(side::count)];
};


enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};
// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 60.0f;

class Camera
{
public:

    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // euler Angles
    float M_Yaw;
    float M_Pitch;
    // camera options
    float cameraSpeed;
    float MouseSensitivity;
    float Zoom;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
        : Front(glm::vec3(0.0f, 0.0f, -1.0f)), cameraSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {

        Position = position;
        WorldUp = up;
        M_Yaw = yaw;
        M_Pitch = pitch;
        updateCameraVectors();
    }
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    void ProcessKeyboard(Camera_Movement direction, float dt)
    {
        float velocity = cameraSpeed * dt;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
    }
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        M_Yaw += xoffset;
        M_Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (M_Pitch > 80.0f)
                M_Pitch = 80.0f;
            if (M_Pitch < -80.0f)
                M_Pitch = -80.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 10.0f)
            Zoom = 10.0f;
        if (Zoom > 60.0f)
            Zoom = 60.0f;
    }

private:
    void updateCameraVectors()
    {
        glm::vec3 direction = glm::vec3(1.0f);
        direction.x = cos(glm::radians(M_Yaw)) * cos(glm::radians(M_Pitch));
        direction.y = sin(glm::radians(M_Pitch));
        direction.z = sin(glm::radians(M_Yaw)) * cos(glm::radians(M_Pitch));
        Front = glm::normalize(direction);
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));
    }
};