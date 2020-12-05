#include "SolarSystem.h"
#define M_PI 3.141592653589793f

SolarSys::SolarSys():SunRot(0.0f),m_timer_e(0), EarthRot(0.0f), 
m_timer_m(0),view(glm::mat4(1.0f)), proj(glm::mat4(1.0f)), camRot(0.0f),mode(CameraMode::FORWARD)
{
    unsigned int n = 100;
    float r = 0.5f;
    float angle = 2 * M_PI / n;
    float anglez = M_PI / n;
    std::vector<Vertex> vertic;

    vertic.clear();

    for (size_t j = 0; j <= n; j++)
    {
        for (size_t i = 0; i <= n; i++)
        {
            glm::vec3 position = glm::vec3(sinf(anglez * j) * sinf(angle * i), cosf(anglez * j), sinf(anglez * j) * cosf(angle * i));
            glm::vec3 normal = position;
            vertic.push_back({
                position,
                normal,
                glm::vec2((float)i / (float)n,(float)j / (float)n)
                });
        }
    }
    vb.Creat(&vertic[0], vertic.size() * 8 * sizeof(float));
    layout.Push<float>(3);
    layout.Push<float>(3);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    std::vector<unsigned int> indices;
    if (vertic.size() > 0) {
        for (size_t i = 0; i < vertic.size() -n - 2; i++) {
            indices.push_back(i);
            indices.push_back(i + n + 1);
            indices.push_back(i + 1);
            indices.push_back(i + 1);
            indices.push_back(i + n + 1);
            indices.push_back(i + n + 2);
        }
    }

    ib.Creat(&indices[0], indices.size());
    
    SetShader();
    proj = glm::perspective(glm::radians(60.0f), (float)1280 / 720, 0.1f, 100.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, 15.0f)*(-1.0f));
    view = glm::rotate(view, glm::radians(camRot), glm::vec3(1.0, 0.0, 0.0));
    
}

SolarSys::~SolarSys()
{
}

void SolarSys::SetShader()
{
    shader.Creat("rsc/shaders/Basic.shader");
    shader.Bind();


    unsigned int earth = texture.CreatTexture("rsc/textures/earth.png");
    unsigned int moon = texture.CreatTexture("rsc/textures/moon.png");
    unsigned int sun = texture.CreatTexture("rsc/textures/bigSun.png");

    glBindTextureUnit(0, sun);
    glBindTextureUnit(1, earth);
    glBindTextureUnit(2, moon);



    int samplers[3] = { 0,1,2};
    shader.SetUniform1iv("u_Texture", 3, samplers);
    shader.SetUniform3f("lightcolor", 1.0f, 1.0f, 1.0f);
    shader.SetUniform3f("lightPos", 0.0f,0.0f,0.0f);
    shader.SetUniform3f("viewPos", 0.0f, 0.0f, 15.0f);
}

void SolarSys::DrawSun()
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(SunRot), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(2.0f));
    glm::mat4 mvp = proj * view * model;
    SunRot += 0.1f;
    shader.SetUniformMat4f("u_MVP", mvp);
    shader.SetUniformMat4f("model", model);
    shader.SetUniform1i("index", 0);
    m_renderer.DrawElement(va, ib, shader);

}

void SolarSys::DrawEarth()
{
    unsigned int n = 2400;
    float a = 9.6f;
    float b = 6.4f;
    float angle = 2 * M_PI / n;
    m_timer_e++;
    if (m_timer_e == n) {
        m_timer_e = 0;
    }
    EarthRot += 0.4f;
    glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(cosf((float)m_timer_e*angle)*a, 0.0f, sinf((float)m_timer_e * angle)*b));
    earthPos = model;
    model = glm::rotate(model, glm::radians(EarthRot), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 modelRot = glm::rotate(glm::mat4(1.0), glm::radians(EarthRot), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f));
    glm::mat4 mvp = proj * view * model;
    shader.SetUniformMat4f("u_MVP", mvp);
    shader.SetUniformMat4f("model", model);
    shader.SetUniformMat4f("modelRot", modelRot);
    shader.SetUniform1i("index", 1);
    m_renderer.DrawElement(va, ib, shader);

}

void SolarSys::DrawMoon()
{
    unsigned int n = 400;
    float a = 3.6f;
    float b = 2.4f;
    float angle = 2 * M_PI / n;
    m_timer_m++;
    if (m_timer_m == n) {
        m_timer_m = 0;
    }
    glm::mat4 model = earthPos;
    model = glm::translate(model, glm::vec3(cosf((float)m_timer_m * angle) * a, 0.0f, sinf((float)m_timer_m * angle) * b));
    model = glm::scale(model, glm::vec3(0.2f));
    glm::mat4 mvp = proj * view * model;
    shader.SetUniformMat4f("u_MVP", mvp);
    shader.SetUniformMat4f("model", model);
    shader.SetUniformMat4f("modelRot", glm::mat4(1.0f));
    shader.SetUniform1i("index", 2);
    m_renderer.DrawElement(va, ib, shader);
}

void SolarSys::processInputs(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        mode = CameraMode::FORWARD;
        camRot = 0.0f;
        shader.SetUniform3f("viewPos", 0.0f, 0.0f, 15.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        mode = CameraMode::TOPWARD;
        camRot = 90.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        mode = CameraMode::BACKWARD;
        camRot = 180.0f;
        shader.SetUniform3f("viewPos", 0.0f, 0.0f, -15.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        mode = CameraMode::DOWNWARD;
        camRot = 270.0f;
    }
    view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 15.0f) * (-1.0f));
    view = glm::rotate(view, glm::radians(camRot), glm::vec3(1.0, 0.0, 0.0));
}



