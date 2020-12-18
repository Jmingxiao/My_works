#pragma once
#include"glm/gtc/matrix_transform.hpp"
#include"glm/glm.hpp"

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
const float ZOOM = 45.0f;

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

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3 up = glm::vec3(0.0f,1.0f,0.0f), float yaw = YAW, float pitch = PITCH)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)),cameraSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {

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
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
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
        glm::vec3 direction= glm::vec3(1.0f);
        direction.x = cos(glm::radians(M_Yaw)) * cos(glm::radians(M_Pitch));
        direction.y = sin(glm::radians(M_Pitch));
        direction.z = sin(glm::radians(M_Yaw)) * cos(glm::radians(M_Pitch));
        Front = glm::normalize(direction);
        Right = glm::normalize(glm::cross(Front, WorldUp));  
        Up = glm::normalize(glm::cross(Right, Front));
    }
};