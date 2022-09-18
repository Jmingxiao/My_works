#include<GL/glew.h>
#include <GLFW/glfw3.h>

#include<iostream>
#include<string>
#include<assert.h>
#include<vector>
#include<math.h>

#include "Renderer.h"
#include "Object.h"
#include "Camera.h"
#include"model.h"

#include"glm/gtc/matrix_transform.hpp"
#include"glm/glm.hpp"
#define M_PI 3.141592653589793f

#define SCR_WIDTH 1920
#define SCR_HEIGHT 1080


using std::cout;
using std::endl;
using std::cin;
using std::vector;

static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
static void processInput(GLFWwindow* window,float dt);



//----------set camera---------
Camera camera(glm::vec3(1.0f, 1.0f, 1.0f));
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


  
//------------------------skybox---------
    VertexBuffer vb_s;
    VertexArray va_s;
    VertexBufferLayout layout_s;
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    vb_s.Creat(skyboxVertices, sizeof(skyboxVertices));
    layout_s.Push<float>(3);
    va_s.AddBuffer(vb_s, layout_s);

//---------------model_------and shader------

    Shader shader;
    Shader reflec;
    reflec.Creat("rsc/shaders/basicVS.shader", "rsc/shaders/basicFS.shader");
    shader.Creat("rsc/shaders/skyboxVS.shader", "rsc/shaders/skyboxFS.shader");
    
//-------texture------------
    Texture texture;
    vector<string>faces
    { "rsc/skybox/right.jpg",
      "rsc/skybox/left.jpg",
      "rsc/skybox/top.jpg",
      "rsc/skybox/bottom.jpg",
      "rsc/skybox/front.jpg",
      "rsc/skybox/back.jpg"
    };
   texture.CreatCubeMap(faces);

    shader.SetUniform1i("skybox", 0);
    reflec.SetUniform1i("skybox", 0);
//----Scene Objects-------------;
    Renderer render;
    //frustum cull;
    Model model("rsc/objects/backpack/backpack.obj");

//--------------quad------------------------

    VertexBuffer vb_q;
    VertexArray va_q;
    VertexBufferLayout layout_q;

    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
      // positions   // texCoords
      -1.0f,  1.0f,0.0f,  0.0f, 1.0f,
      -1.0f,  0.0f, 0.0f,  0.0f, 0.0f,
       0.0f,  0.0f, 0.0f,  1.0f, 0.0f,
                       
      0.0f,  0.0f, 0.0f,  1.0f, 0.0f,
      0.0f,  1.0f, 0.0f,  1.0f, 1.0f,
      -1.0f,  1.0f, 0.0f,  0.0f, 1.0f
    };

    vb_q.Creat(quadVertices, sizeof(quadVertices));
    layout_q.Push<float>(3);
    layout_q.Push<float>(2);
    va_q.AddBuffer(vb_q, layout_q);

    Shader quadShader;
    quadShader.Creat("rsc/shaders/quadVS.shader", "rsc/shaders/hdrfs.shader");
    quadShader.SetUniform1i("screenTexture", 0);
 //------------frame buffer----and quad--------
 // framebuffer configuration
    // -------------------------
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // create a color attachment texture
    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

//-----main Loop-----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projview = proj * view;
      /*   render
        ------
        bind to framebuffer and draw scene as we normally would to color texture */
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        // make sure we clear the framebuffer's content
        glClearColor(0.5f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        /* Render here  first pass */

        texture.Bind();
        reflec.Bind();
        reflec.SetUniform1i("skybox", 0);
        reflec.SetUniformMat4f("projview", projview);
        reflec.SetUniformMat4f("model", glm::mat4(1.0f));
        reflec.SetUniform3f("cameraPos", camera.Position.x,camera.Position.y,camera.Position.z);
        processInput(window, deltaTime);
        model.Draw(reflec);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.5f, 0.2f, 0.3f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDepthFunc(GL_LESS);
        texture.Bind();
        reflec.Bind();
        reflec.SetUniform1i("skybox", 0);
        reflec.SetUniformMat4f("projview", projview);
        reflec.SetUniformMat4f("model", glm::mat4(1.0f));
        reflec.SetUniform3f("cameraPos", camera.Position.x, camera.Position.y, camera.Position.z);
        processInput(window, deltaTime);
        model.Draw(reflec);

        //------------quadquad render---------;

        quadShader.Bind();
        va_q.Bind();
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
        glDrawArrays(GL_TRIANGLES, 0, 6);


       /* light.processInput(window, deltaTime);
        light.LightUpdate(proj, view, deltaTime);*/

        glDepthFunc(GL_LEQUAL);
        va_s.Bind();
        shader.Bind();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        projview = proj * view;
        shader.SetUniformMat4f("projview", projview);
        texture.Bind();
        shader.SetUniform1i("skybox", 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthFunc(GL_LESS);
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


