#include "Object.h"
#ifndef M_PI
#define M_PI 3.141592653589793f
#endif
Sphere::Sphere():Pos(glm::vec3(0.0f)), withTexture(false)
{
	init();
}

Sphere::Sphere(const glm::vec3& pos):Pos(pos), withTexture(false)
{
	init();
}

Sphere::~Sphere()
{
}

inline void Sphere::init()
{
	unsigned int n = 64;
	std::vector<float> vertic;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> uv;
	std::vector<glm::vec3> normals;
	vertic.clear();

	for (size_t j = 0; j <= n; j++)
	{
		for (size_t i = 0; i <= n; i++)
		{
			float xSegment = (float)i / (float)n;
			float ySegment = (float)j / (float)n;
			float xPos = std::cos(xSegment * 2.0f * M_PI) * std::sin(ySegment * M_PI);
			float yPos = std::cos(ySegment * M_PI);
			float zPos = std::sin(xSegment * 2.0f * M_PI) * std::sin(ySegment * M_PI);

			positions.push_back(glm::vec3(xPos, yPos, zPos));
			uv.push_back(glm::vec2(xSegment, ySegment));
			normals.push_back(glm::vec3(xPos, yPos, zPos));
		}
	}

	for (unsigned int i = 0; i < positions.size(); ++i)
	{
		vertic.push_back(positions[i].x);
		vertic.push_back(positions[i].y);
		vertic.push_back(positions[i].z);
		if (uv.size() > 0)
		{
			vertic.push_back(uv[i].x);
			vertic.push_back(uv[i].y);
		}
		if (normals.size() > 0)
		{
			vertic.push_back(normals[i].x);
			vertic.push_back(normals[i].y);
			vertic.push_back(normals[i].z);
		}
	}

	vb.Creat(&vertic[0], vertic.size()* sizeof(float));
	layout.Push<float>(3);
	layout.Push<float>(2);
	layout.Push<float>(3);
	va.AddBuffer(vb, layout);

	std::vector<unsigned int> indices;
	bool oddRow = false;
	for (unsigned int y = 0; y < n; ++y)
	{
		if (!oddRow) // even rows: y == 0, y == 2; and so on
		{
			for (unsigned int x = 0; x <= n; ++x)
			{
				indices.push_back(y * (n + 1) + x);
				indices.push_back((y + 1) * (n + 1) + x);
			}
		}
		else
		{
			for (int x = n; x >= 0; --x)
			{
				indices.push_back((y + 1) * (n + 1) + x);
				indices.push_back(y * (n + 1) + x);
			}
		}
		oddRow = !oddRow;
	}
	ib.Creat(&indices[0], indices.size());
}

void Sphere::SetPBRShader()
{
	shader.Creat("rsc/shaders/pbr.shader");
	shader.Bind();

	shader.SetUniform3f("lightColors", 300.0f, 300.0f, 300.0f);
	shader.SetUniform3f("lightPositions", 2.0f, 1.0f, 0.5f);
	shader.SetUniform3f("camPos", 0.0f, 0.0f, 15.0f);
	shader.SetUniform3f("albedo", 0.0f, 0.5f, 0.8f);
	shader.SetUniform1f("metallic", 0.4f);
	shader.SetUniform1f("roughness", 0.3f);
	shader.SetUniform1f("ao", 0.8f);
}

void Sphere::SetPBRTexShader()
{
	shader.Creat("rsc/shaders/pbrTex.shader");
	shader.Bind();

	shader.SetUniform3f("lightColors", 150.0f, 150.0f, 150.0f);
	shader.SetUniform3f("lightPositions", 2.0f, 1.0f, 0.5f);
	shader.SetUniform3f("camPos", 0.0f, 0.0f, 15.0f);
	shader.SetUniform1i("albedoMap", 1);
	shader.SetUniform1i("aoMap", 2);
	shader.SetUniform1i("metallicMap", 3);
	shader.SetUniform1i("normalMap", 4);
	shader.SetUniform1i("roughnessMap", 5);

	albedo.CreatTexture("rsc/textures/medieval_wall/albedo.png");
	ao.CreatTexture("rsc/textures/medieval_wall/ao.png");
	metallic.CreatTexture("rsc/textures/medieval_wall/metallic.png");
	normal.CreatTexture("rsc/textures/medieval_wall/normalMap.png");
	roughness.CreatTexture("rsc/textures/medieval_wall/roughness.png");
	
	albedo.Bind(1);
	ao.Bind(2);
	metallic.Bind(3);
	normal.Bind(4);
	roughness.Bind(5);

}

void Sphere::SetPhongShader()
{
	shader.Creat("rsc/shaders/phong.shader");
	shader.Bind();
	shader.SetUniform3f("lightColors", 1.0f, 1.0f, 1.0f);
	shader.SetUniform3f("lightPositions", 2.0f, 1.0f, 0.5f);
	shader.SetUniform3f("camPos", 0.0f, 0.0f, 15.0f);
	shader.SetUniform3f("albedo", 1.0f, 0.5f, 1.8f);

}

void Sphere::SetTransparent()
{
	shader.Creat("rsc/shaders/transparent.shader");
	shader.Bind();
	shader.SetUniform1i("skybox", 0);
}

void Sphere::Update(const glm::mat4& proj, const glm::mat4& view, const glm::vec3& viewPos, const glm::vec3& lightPos)
{
	glm::mat4 model = glm::translate(glm::mat4(1.0f), Pos);
	model = glm::scale(model, glm::vec3(0.3f));
	shader.Bind();
	shader.SetUniformMat4f("projection", proj);
	shader.SetUniformMat4f("view", view);
	shader.SetUniformMat4f("model", model);
	shader.SetUniform3f("camPos",viewPos);
	shader.SetUniform3f("lightPositions", lightPos);
	m_renderer.DrawElement(va, ib, shader);
}

Light::Light():Pos(glm::vec3(2.0f, 1.0f, 0.5f))
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
	vb.Creat(vertices, sizeof(vertices));
	layout.Push<float>(3);
	va.AddBuffer(vb, layout);
	shader.Creat("rsc/shaders/LightCube.shader");
	shader.Bind();
}

Light::~Light()
{

}

void Light::Input(const glm::vec3& Dir,float dt)
{
	Pos += Dir*dt*5.0f;
}

void Light::Update(const glm::mat4& proj, const glm::mat4& view)
{
	glm::mat4 model = glm::translate(glm::mat4(1.0f), Pos);
	model = glm::scale(model, glm::vec3(0.1f));
	glm::mat4 mvp = proj * view * model;
	shader.Bind();
	shader.SetUniformMat4f("MVP", mvp);
	va.Bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);

}
