#include"SolarSystem.h"
#define M_PI 3.141592653589793f

SolarSys::SolarSys()
    :SunRot(0.0f), PlanetRot(0.0f), view_(glm::mat4(1.0f)), proj_(glm::mat4(1.0f)), camRot(0.0f)
{
    unsigned int n = 50;
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
    unsigned int jupiter = texture.CreatTexture("rsc/textures/jupiter.png");
    unsigned int mars = texture.CreatTexture("rsc/textures/mars.png");
    unsigned int mercury = texture.CreatTexture("rsc/textures/mercury.png");
    unsigned int neptune = texture.CreatTexture("rsc/textures/neptune.png");
    unsigned int saturn = texture.CreatTexture("rsc/textures/saturn.png");
    unsigned int uranus = texture.CreatTexture("rsc/textures/uranus.png");
    unsigned int venus = texture.CreatTexture("rsc/textures/venus.png");

    glBindTextureUnit(Sun, sun);
    glBindTextureUnit(Earth, earth);
    glBindTextureUnit(Moon, moon);
    glBindTextureUnit(Mercury, mercury);
    glBindTextureUnit(Venus, venus);
    glBindTextureUnit(Mars, mars);
    glBindTextureUnit(Jupiter, jupiter);
    glBindTextureUnit(Saturn, saturn);
    glBindTextureUnit(Uranus, uranus);
    glBindTextureUnit(Neptune, neptune);




    int samplers[10] = { 0,1,2,3,4,5,6,7,8,9};
    shader.SetUniform1iv("u_Texture", 10, samplers);
    shader.SetUniform3f("lightcolor", 1.0f, 1.0f, 1.0f);
    shader.SetUniform3f("lightPos", 0.0f,0.0f,0.0f);
    shader.SetUniform3f("viewPos", 0.0f, 0.0f, 15.0f);
}

void SolarSys::DrawSun()
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(SunRot), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(2.0f));
    glm::mat4 mvp = proj_ * view_ * model;
    SunRot += 0.1f;
    shader.SetUniformMat4f("u_MVP", mvp);
    shader.SetUniformMat4f("model", model);
    shader.SetUniform1i("index", Sun);
    m_renderer.DrawElement(va, ib, shader);

}

void SolarSys::DrawEarth()
{
    unsigned int n = 2400;
    float a = 12.0f;
    float b = 8.0f;
    float angle = 2 * M_PI / n;
    planet_ellipse[Earth]++;
    if (planet_ellipse[Earth] == n) {
        planet_ellipse[Earth] = 0;
    }
    PlanetRot += 0.4f;
    glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(cosf((float)planet_ellipse[Earth] *angle)*a, 0.0f, sinf((float)planet_ellipse[Earth] * angle)*b));
    earthPos = model;
    model = glm::rotate(model, glm::radians(PlanetRot), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 modelRot = glm::rotate(glm::mat4(1.0), glm::radians(PlanetRot), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.5f));
    glm::mat4 mvp = proj_ * view_ * model;
    shader.SetUniformMat4f("u_MVP", mvp);
    shader.SetUniformMat4f("model", model);
    shader.SetUniformMat4f("modelRot", modelRot);
    shader.SetUniform1i("index", Earth);
    m_renderer.DrawElement(va, ib, shader);

}

void SolarSys::DrawMoon()
{
    unsigned int n = 400;
    float a = 2.4f;
    float b = 1.6f;
    float angle = 2 * M_PI / n;
    planet_ellipse[Moon]++;
    if (planet_ellipse[Moon] == n) {
        planet_ellipse[Moon] = 0;
    }
    glm::mat4 model = earthPos;
    model = glm::translate(model, glm::vec3(cosf((float)planet_ellipse[Moon]* angle) * a, 0.0f, sinf((float)planet_ellipse[Moon] * angle) * b));
    model = glm::scale(model, glm::vec3(0.05f));
    glm::mat4 mvp = proj_ * view_ * model;
    shader.SetUniformMat4f("u_MVP", mvp);
    shader.SetUniformMat4f("model", model);
    shader.SetUniformMat4f("modelRot", glm::mat4(1.0f));
    shader.SetUniform1i("index", Moon);
    m_renderer.DrawElement(va, ib, shader);
}

void SolarSys::DrawMercury()
{
    unsigned int n = 1600;
    float a = 6.0f;
    float b = 4.0f;
    float angle = 2 * M_PI / n;
    planet_ellipse[Mercury]++;
    if (planet_ellipse[Mercury] == n) {
        planet_ellipse[Mercury] = 0;
    }
    glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(cosf((float)planet_ellipse[Mercury] * angle) * a, 0.0f, sinf((float)planet_ellipse[Mercury] * angle) * b));
    model = glm::rotate(model, glm::radians(PlanetRot), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 modelRot = glm::rotate(glm::mat4(1.0), glm::radians(PlanetRot), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.4f));
    glm::mat4 mvp = proj_ * view_ * model;
    shader.SetUniformMat4f("u_MVP", mvp);
    shader.SetUniformMat4f("model", model);
    shader.SetUniformMat4f("modelRot", modelRot);
    shader.SetUniform1i("index", Mercury);
    m_renderer.DrawElement(va, ib, shader);
}

void SolarSys::DrawVenus()
{
    unsigned int n = 2000;
    float a = 9.0f;
    float b = 6.0f;
    float angle = 2 * M_PI / n;
    planet_ellipse[Venus]++;
    if (planet_ellipse[Venus] == n) {
        planet_ellipse[Venus] = 0;
    }
    glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(cosf((float)planet_ellipse[Venus] * angle) * a, 0.0f, sinf((float)planet_ellipse[Venus] * angle) * b));
    model = glm::rotate(model, glm::radians(PlanetRot), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 modelRot = glm::rotate(glm::mat4(1.0), glm::radians(PlanetRot), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.4f));
    glm::mat4 mvp = proj_ * view_ * model;
    shader.SetUniformMat4f("u_MVP", mvp);
    shader.SetUniformMat4f("model", model);
    shader.SetUniformMat4f("modelRot", modelRot);
    shader.SetUniform1i("index", Venus);
    m_renderer.DrawElement(va, ib, shader);
}

void SolarSys::DrawMars()
{
    unsigned int n = 1800;
    float a = 15.0f;
    float b = 10.0f;
    float angle = 2 * M_PI / n;
    planet_ellipse[Mars]++;
    if (planet_ellipse[Mars] == n) {
        planet_ellipse[Mars] = 0;
    }
    glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(cosf((float)planet_ellipse[Mars] * angle) * a, 0.0f, sinf((float)planet_ellipse[Mars] * angle) * b));
    model = glm::rotate(model, glm::radians(PlanetRot), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 modelRot = glm::rotate(glm::mat4(1.0), glm::radians(PlanetRot), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.4f));
    glm::mat4 mvp = proj_ * view_ * model;
    shader.SetUniformMat4f("u_MVP", mvp);
    shader.SetUniformMat4f("model", model);
    shader.SetUniformMat4f("modelRot", modelRot);
    shader.SetUniform1i("index", Mars);
    m_renderer.DrawElement(va, ib, shader);
}

void SolarSys::DrawJupiter()
{
    unsigned int n = 2500;
    float a = 18.0f;
    float b = 12.0f;
    float angle = 2 * M_PI / n;
    planet_ellipse[Jupiter]++;
    if (planet_ellipse[Jupiter] == n) {
        planet_ellipse[Jupiter] = 0;
    }
    glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(cosf((float)planet_ellipse[Jupiter] * angle) * a, 0.0f, sinf((float)planet_ellipse[Jupiter] * angle) * b));
    model = glm::rotate(model, glm::radians(PlanetRot), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 modelRot = glm::rotate(glm::mat4(1.0), glm::radians(PlanetRot), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.4f));
    glm::mat4 mvp = proj_ * view_ * model;
    shader.SetUniformMat4f("u_MVP", mvp);
    shader.SetUniformMat4f("model", model);
    shader.SetUniformMat4f("modelRot", modelRot);
    shader.SetUniform1i("index", Jupiter);
    m_renderer.DrawElement(va, ib, shader);
}

void SolarSys::DrawSaturn()
{
    unsigned int n = 3000;
    float a = 21.0f;
    float b = 14.0f;
    float angle = 2 * M_PI / n;
    planet_ellipse[Saturn]++;
    if (planet_ellipse[Saturn] == n) {
        planet_ellipse[Saturn] = 0;
    }
    glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(cosf((float)planet_ellipse[Saturn] * angle) * a, 0.0f, sinf((float)planet_ellipse[Saturn] * angle) * b));
    model = glm::rotate(model, glm::radians(PlanetRot), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 modelRot = glm::rotate(glm::mat4(1.0), glm::radians(PlanetRot), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.4f));
    glm::mat4 mvp = proj_ * view_ * model;
    shader.SetUniformMat4f("u_MVP", mvp);
    shader.SetUniformMat4f("model", model);
    shader.SetUniformMat4f("modelRot", modelRot);
    shader.SetUniform1i("index", Saturn);
    m_renderer.DrawElement(va, ib, shader);
}

void SolarSys::DrawUranus()
{
    unsigned int n = 2600;
    float a = 24.0f;
    float b = 16.0f;
    float angle = 2 * M_PI / n;
    planet_ellipse[Uranus]++;
    if (planet_ellipse[Uranus] == n) {
        planet_ellipse[Uranus] = 0;
    }
    glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(cosf((float)planet_ellipse[Uranus] * angle) * a, 0.0f, sinf((float)planet_ellipse[Uranus] * angle) * b));
    model = glm::rotate(model, glm::radians(PlanetRot), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 modelRot = glm::rotate(glm::mat4(1.0), glm::radians(PlanetRot), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.4f));
    glm::mat4 mvp = proj_ * view_ * model;
    shader.SetUniformMat4f("u_MVP", mvp);
    shader.SetUniformMat4f("model", model);
    shader.SetUniformMat4f("modelRot", modelRot);
    shader.SetUniform1i("index", Uranus);
    m_renderer.DrawElement(va, ib, shader);
}

void SolarSys::DrawNeptune()
{
    unsigned int n = 2200;
    float a = 30.0f;
    float b = 20.0f;
    float angle = 2 * M_PI / n;
    planet_ellipse[Neptune]++;
    if (planet_ellipse[Neptune] == n) {
        planet_ellipse[Neptune] = 0;
    }
    glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(cosf((float)planet_ellipse[Neptune] * angle) * a, 0.0f, sinf((float)planet_ellipse[Neptune] * angle) * b));
    model = glm::rotate(model, glm::radians(PlanetRot), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 modelRot = glm::rotate(glm::mat4(1.0), glm::radians(PlanetRot), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.4f));
    glm::mat4 mvp = proj_ * view_ * model;
    shader.SetUniformMat4f("u_MVP", mvp);
    shader.SetUniformMat4f("model", model);
    shader.SetUniformMat4f("modelRot", modelRot);
    shader.SetUniform1i("index", Neptune);
    m_renderer.DrawElement(va, ib, shader);
}

void SolarSys::CameraUpdate(glm::mat4 proj, glm::mat4 view, glm::vec3 viewPos)
{
    proj_ = proj;
    view_ = view;
    shader.Bind();
    shader.SetUniform3f("viewPos", viewPos.x, viewPos.y, viewPos.z);
}




