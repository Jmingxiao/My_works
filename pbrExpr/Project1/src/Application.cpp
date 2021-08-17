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
#include"Camera.hpp"
#include "Object.h"
#include "skybox.hpp"
#define SCR_WIDTH 1920
#define SCR_HEIGHT 1080


using std::cout;
using std::endl;
using std::cin;
using std::vector;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
static void processInput(GLFWwindow* window, float dt, Light& light);


//----------set camera---------
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
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
    /*glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);*/
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, mouse_callback);


    //blend setup
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

    Skybox skybox;
    Sphere sphere;
    sphere.SetPBRTexShader();
    Sphere sphere_b(glm::vec3(3.0f,2.0f,1.0f));
    sphere_b.SetTransparent();
    Light light;
    //--------------------------------------------------

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
        processInput(window, deltaTime,light);
        
        sphere.Update(proj, view, camera.Position, light.Pos);
        sphere_b.Update(proj, view, camera.Position, light.Pos);
        light.Update(proj, view);
        skybox.Render(proj,glm::mat4(glm::mat3(camera.GetViewMatrix())));

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll((float)yoffset);
}

static void processInput(GLFWwindow* window, float dt, Light& light)
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
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		light.Input(glm::vec3(-1.0f,0.0f,0.0f), dt);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        light.Input(glm::vec3(1.0f, 0.0f, 0.0f), dt);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		light.Input(glm::vec3(0.0, 1.0f, 0.0f), dt);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		light.Input(glm::vec3(0.0f, -1.0f, 0.0f), dt);
		
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

