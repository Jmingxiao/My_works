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



using std::cout;
using std::endl;
using std::cin;
using std::vector;

void imGui(){

/*
  
    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();
    {
        static float f = 0.0f;
        static int counter = 0;
        ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
     ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
            ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
*/
}



int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
   
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 720, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    //vector<float> randNum = randomG();
    glfwMakeContextCurrent(window);
    if (!glewInit() == GLEW_OK)
        std::cout << "Error!" << std::endl;

    //blend setup
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    
    std::vector<Vertex> vertices;
    vertices.push_back({ glm::vec3(-0.5f, -0.5f, 0.0f),glm::vec3(0.0f, 0.0f, 1.0f),glm::vec2(0.0f, 0.0f) });
    vertices.push_back({ glm::vec3(0.5f, -0.5f, 0.0f),glm::vec3(0.0f, 0.0f, 1.0f),glm::vec2(1.0f, 0.0f) });
    vertices.push_back({ glm::vec3(0.5f,  0.5f, 0.0f),glm::vec3(0.0f, 0.0f, 1.0f),glm::vec2(1.0f, 1.0f) });
    vertices.push_back({ glm::vec3(-0.5f,  0.5f, 0.0f),glm::vec3(0.0f, 0.0f, 1.0f),glm::vec2(0.0f, 1.0f) });

    unsigned int indices[] = {
        0,1,2,2,3,0
    };


    VertexArray va;
    VertexBuffer vb;
    vb.Creat(&vertices[0], vertices.size() * sizeof(Vertex));

    VertexBufferLayout layout;
    
    layout.Push<float>(3);
    layout.Push<float>(3); 
    layout.Push<float>(2);

    va.AddBuffer(vb, layout);

    IndexBuffer ib;
    ib.Creat(indices, 3);
    Shader shader;
    shader.Creat("rsc/shaders/Basic.shader");
    shader.Bind();





    
    //--------------------------------------------------
    SolarSys sol;

    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //solar system rendering
        sol.processInputs(window);
        sol.DrawSun();
        sol.DrawEarth();
        sol.DrawMoon();
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

