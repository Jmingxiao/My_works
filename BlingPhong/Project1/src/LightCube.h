#pragma once
#include "Renderer.h"
#include <GLFW/glfw3.h>
#include<iostream>
#include "Object.h"
#include<vector>
using std::vector;

class LightCube
{
public:

    LightCube(glm::vec3 Pos);
    ~LightCube();
    void LightUpdate(glm::mat4 proj, glm::mat4 view, float dt);
private:
    VertexArray va;
    VertexBuffer vb;
    VertexBufferLayout layout;
    IndexBuffer ib;
    Shader lightCube;
    Renderer renderer;
    const float MoveSpeed;
public:
    glm::vec3 LightPos;
    glm::vec3 lightColor;
    void processInput(GLFWwindow* window, float dt);

};

