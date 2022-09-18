#include<GL/glew.h>
#include <GLFW/glfw3.h>

#include<iostream>
#include<string>
#include<assert.h>
#include<vector>
#include<math.h>

#include "Renderer.h"
#include"Object.h"
#include"Camera.h"

#include"glm/gtc/matrix_transform.hpp"
#include"glm/glm.hpp"

#include"LightCube.h"
#include"BackDrop.h"
#include"Cube.h"


#define SCR_WIDTH 1280
#define SCR_HEIGHT 720


using std::cout;
using std::endl;
using std::cin;
using std::vector;

static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
static void processInput(GLFWwindow* window,float dt);



//----------set camera---------
Camera camera(glm::vec3(5.0f, 5.0f, -5.0f));
float previousMouse_x = SCR_WIDTH / 2.0f;
float previousMouse_y = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

int main(void)
{
//------Set time--------
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
//--------glfw setup------------
    GLFWwindow* window;
    if (!glfwInit())
        return -1;
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", NULL, NULL);
    if (!window)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!glewInit() == GLEW_OK)
        std::cout << "Error!" << std::endl;
//----------call back--------------

    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

//--------blend setup--------------
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
//------Set Texture----------------
    Texture texture;
    unsigned int wall = texture.CreatRepeatTexture("rsc/textures/wall.png");
    unsigned int floor = texture.CreatRepeatTexture("rsc/textures/wood.png");
    unsigned int T = texture.CreatTexture("rsc/textures/twins.png");
    unsigned int T1 = texture.CreatTexture("rsc/textures/akali.png");
    unsigned int T2 = texture.CreatTexture("rsc/textures/cargirl.png");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, wall);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, floor);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, T);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, T1);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, T2);

//----Scene Objects-------------;
    LightCube light(glm::vec3(2.0f,5.0f,-3.0f));
    BackDrop backdrop;
    Cube cube(glm::vec3(2.0f,4.0f,3.0f),2);
    Cube cube1(glm::vec3(-2.0f,2.0f, 5.0f),3);
    Cube cube2(glm::vec3(0.0f, 3.0f, 8.0f), 4);

    Renderer render;

//-----main Loop-----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 mvp = proj * view;
        processInput(window, deltaTime);
        
        light.LightUpdate(proj, view,deltaTime);
        light.processInput(window, deltaTime);
        backdrop.LightUpdate(light.LightPos, light.lightColor);
        backdrop.DrawUpdate(proj, view, camera.Position);
        cube.LightUpdate(light.LightPos, light.lightColor);
        cube.DrawUpdate(proj, view, camera.Position,deltaTime);
        cube1.LightUpdate(light.LightPos, light.lightColor);
        cube1.DrawUpdate(proj, view, camera.Position,deltaTime);
        cube2.LightUpdate(light.LightPos, light.lightColor);
        cube2.DrawUpdate(proj, view, camera.Position, deltaTime);

     
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

static void processInput(GLFWwindow* window,float dt)
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