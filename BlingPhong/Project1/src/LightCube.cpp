#include"LightCube.h"


 LightCube::LightCube(glm::vec3 Pos): LightPos(Pos), MoveSpeed(2.5f), lightColor(glm::vec3(1.0f))
{
    float vertices[] = {
     -0.5f, -0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,
      0.5f,  0.5f, -0.5f,
      0.5f,  0.5f, -0.5f,
     -0.5f,  0.5f, -0.5f,
     -0.5f, -0.5f, -0.5f,

     -0.5f, -0.5f,  0.5f,
      0.5f, -0.5f,  0.5f,
      0.5f,  0.5f,  0.5f,
      0.5f,  0.5f,  0.5f,
     -0.5f,  0.5f,  0.5f,
     -0.5f, -0.5f,  0.5f,

     -0.5f,  0.5f,  0.5f,
     -0.5f,  0.5f, -0.5f,
     -0.5f, -0.5f, -0.5f,
     -0.5f, -0.5f, -0.5f,
     -0.5f, -0.5f,  0.5f,
     -0.5f,  0.5f,  0.5f,

      0.5f,  0.5f,  0.5f,
      0.5f,  0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,
      0.5f, -0.5f,  0.5f,
      0.5f,  0.5f,  0.5f,

     -0.5f, -0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,
      0.5f, -0.5f,  0.5f,
      0.5f, -0.5f,  0.5f,
     -0.5f, -0.5f,  0.5f,
     -0.5f, -0.5f, -0.5f,

     -0.5f,  0.5f, -0.5f,
      0.5f,  0.5f, -0.5f,
      0.5f,  0.5f,  0.5f,
      0.5f,  0.5f,  0.5f,
     -0.5f,  0.5f,  0.5f,
     -0.5f,  0.5f, -0.5f,
    };

    vector<unsigned int> indices;
    for (size_t i = 0; i < 36; i++)
    {
        indices.push_back(i);
    }

    vb.Creat(vertices, sizeof(vertices));
    layout.Push<float>(3);
    va.AddBuffer(vb, layout);
    ib.Creat(&indices[0], indices.size());
    lightCube.Creat("rsc/shaders/lightCubeVS.shader", "rsc/shaders/lightCubeFS.shader");
}

 LightCube::~LightCube()
 {
     vb.Unbind();
     va.Unbind();
     ib.Unbind();
 }

 void LightCube::LightUpdate(glm::mat4 proj, glm::mat4 view,float dt)
 {

     glm::mat4 model = glm::translate(glm::mat4(1.0f), LightPos);
     model = glm::scale(model, glm::vec3(0.3f));
     glm::mat4 mvp = proj * view * model;
     lightCube.Bind();
     lightCube.SetUniformMat4f("u_mvp", mvp);
     lightCube.SetUniform3f("lightColor", lightColor.r, lightColor.g, lightColor.b);
     renderer.DrawElement(va, ib, lightCube);
     lightCube.Unbind();
 }

 void LightCube::processInput(GLFWwindow* window, float dt)
 {
     if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
         if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
             LightPos += glm::vec3(0.0f, 1.0f, 0.0f) * MoveSpeed * dt;
         }
         else
         {
             LightPos += glm::vec3(0.0f, 0.0f, -1.0f) * MoveSpeed * dt;
         }
     }
     if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
         if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
             LightPos += glm::vec3(0.0f, -1.0f, 0.0f) * MoveSpeed * dt;
         }
         else
         {
             LightPos += glm::vec3(0.0f, 0.0f, 1.0f) * MoveSpeed * dt;
         }
     }
     if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) 
     {
         LightPos += glm::vec3(-1.0f, 0.0f, 0.0f) * MoveSpeed * dt;
     }
     if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) 
     {
         LightPos += glm::vec3(1.0f, 0.0f, 0.0f) * MoveSpeed * dt;
     }
     if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
     
         lightColor = glm::vec3(1.0f, 0.5f, 0.5f);
     }
     if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
         lightColor = glm::vec3(0.5f, 1.0f, 0.5f);
     }
     if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
         lightColor = glm::vec3(0.5f, 0.5f, 1.0f);
     }
     if (glfwGetKey(window, GLFW_KEY_4 )== GLFW_PRESS) {
         lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
     }
 }
