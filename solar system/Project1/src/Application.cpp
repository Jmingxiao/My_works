#include<GL/glew.h>
#include <GLFW/glfw3.h>

#include<iostream>
#include<string>
#include<assert.h>
#include<vector>
#include<math.h>

#include "Renderer.h"
#include"VertexBuffer.h"
#include"VertexBufferLayout.h"
#include"Texture.h"
#include"glm/gtc/matrix_transform.hpp"
#include"glm/glm.hpp"

#include"imgui/imgui.h"
#include"imgui/imgui_impl_glfw_gl3.h"

#include <array>
#include"SolarSystem.h"
#include"Camera.h"
#define SCR_WIDTH 1920
#define SCR_HEIGHT 1080


using std::cout;
using std::endl;
using std::cin;
using std::vector;


static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
static void processInput(GLFWwindow* window, float dt);


//----------set camera---------
Camera camera(glm::vec3(0.0f, 0.0f, 20.0f));
float previousMouse_x = SCR_WIDTH / 2.0f;
float previousMouse_y = SCR_HEIGHT / 2.0f;
bool firstMouse = true;


int main(void)
{

    //------Set time--------
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
   
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!glewInit() == GLEW_OK)
        std::cout << "Error!" << std::endl;

    //----------call back--------------

    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, mouse_callback);


    //blend setup
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    

    
    //--------------------------------------------------
    SolarSys sol;

    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //-----------------camera_set----------------
        glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projview = proj * view;
        processInput(window, deltaTime);
        //solar system rendering
        sol.CameraUpdate(proj, view, camera.Position);
        sol.DrawSun();
        sol.DrawEarth();
        sol.DrawMercury();
        sol.DrawVenus();
        sol.DrawMars();
        sol.DrawJupiter();
        sol.DrawSaturn();
        sol.DrawUranus();
        sol.DrawNeptune();
        sol.DrawMoon();
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll((float)yoffset);
}

static void processInput(GLFWwindow* window, float dt)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, dt);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, dt);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, dt);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, dt);
    
}

static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        previousMouse_x = (float)xpos;
        previousMouse_y = (float)ypos;
        firstMouse = false;
    }
    float xoffset = (float)xpos - previousMouse_x;
    float yoffset = previousMouse_y - (float)ypos; // reversed since y-coordinates go from bottom to top

    previousMouse_x = (float)xpos;
    previousMouse_y = (float)ypos;
    if (glfwGetMouseButton(window, 1) == GLFW_PRESS) {

        camera.ProcessMouseMovement(xoffset, yoffset);
    }
}   

