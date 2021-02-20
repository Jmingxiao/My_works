#include "Cube.h"

Cube::Cube(glm::vec3 Pos,const unsigned int T_ID):pos(Pos), timer(0.0f)
{
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vertices.push_back({ glm::vec3(-0.5f,-0.5f, -0.5f),glm::vec3(0.0f, 0.0f, -1.0f),glm::vec2(0.0f, 0.0f) });
	vertices.push_back({ glm::vec3(0.5f,-0.5f, -0.5f),glm::vec3(0.0f, 0.0f, -1.0f),glm::vec2(1.0f, 0.0f) });
	vertices.push_back({ glm::vec3(0.5f, 0.5f, -0.5f),glm::vec3(0.0f, 0.0f, -1.0f),glm::vec2(1.0f, 1.0f) });
	vertices.push_back({ glm::vec3(-0.5f, 0.5f, -0.5f),glm::vec3(0.0f, 0.0f, -1.0f),glm::vec2(0.0f, 1.0f) });

	vertices.push_back({ glm::vec3(-0.5f,-0.5f, 0.5f),glm::vec3(0.0f, 0.0f, 1.0f),glm::vec2(0.0f, 0.0f) });
	vertices.push_back({ glm::vec3(0.5f,-0.5f, 0.5f),glm::vec3(0.0f, 0.0f, 1.0f),glm::vec2(1.0f, 0.0f) });
	vertices.push_back({ glm::vec3(0.5f, 0.5f, 0.5f),glm::vec3(0.0f, 0.0f, 1.0f),glm::vec2(1.0f, 1.0f) });
	vertices.push_back({ glm::vec3(-0.5f, 0.5f, 0.5f),glm::vec3(0.0f, 0.0f, 1.0f),glm::vec2(0.0f, 1.0f) });

	vertices.push_back({ glm::vec3(-0.5f, 0.5f, 0.5f),glm::vec3(-1.0f, 0.0f, 0.0f),glm::vec2(0.0f, 0.0f) });
	vertices.push_back({ glm::vec3(-0.5f, 0.5f,-0.5f),glm::vec3(-1.0f, 0.0f, 0.0f),glm::vec2(1.0f, 0.0f) });
	vertices.push_back({ glm::vec3(-0.5f,-0.5f,-0.5f),glm::vec3(-1.0f, 0.0f, 0.0f),glm::vec2(1.0f, 1.0f) });
	vertices.push_back({ glm::vec3(-0.5f,-0.5f, 0.5f),glm::vec3(-1.0f, 0.0f, 0.0f),glm::vec2(0.0f, 1.0f) });

	vertices.push_back({ glm::vec3(0.5f, 0.5f, 0.5f),glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(0.0f, 0.0f) });
	vertices.push_back({ glm::vec3(0.5f, 0.5f,-0.5f),glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(1.0f, 0.0f) });
	vertices.push_back({ glm::vec3(0.5f,-0.5f,-0.5f),glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(1.0f, 1.0f) });
	vertices.push_back({ glm::vec3(0.5f,-0.5f, 0.5f),glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(0.0f, 1.0f) });

	vertices.push_back({ glm::vec3(-0.5f,-0.5f,-0.5f),glm::vec3(0.0f,-1.0f, 0.0f),glm::vec2(0.0f, 0.0f) });
	vertices.push_back({ glm::vec3(0.5f,-0.5f,-0.5f),glm::vec3(0.0f,-1.0f, 0.0f),glm::vec2(1.0f, 0.0f) });
	vertices.push_back({ glm::vec3(0.5f,-0.5f, 0.5f),glm::vec3(0.0f,-1.0f, 0.0f),glm::vec2(1.0f, 1.0f) });
	vertices.push_back({ glm::vec3(-0.5f,-0.5f, 0.5f),glm::vec3(0.0f,-1.0f, 0.0f),glm::vec2(0.0f, 1.0f) });

	vertices.push_back({ glm::vec3(-0.5f, 0.5f,-0.5f),glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(0.0f, 0.0f) });
	vertices.push_back({ glm::vec3(0.5f, 0.5f,-0.5f),glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(1.0f, 0.0f) });
	vertices.push_back({ glm::vec3(0.5f, 0.5f, 0.5f),glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(1.0f, 1.0f) });
	vertices.push_back({ glm::vec3(-0.5f, 0.5f, 0.5f),glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(0.0f, 1.0f) });


	for (size_t i = 0; i < vertices.size(); i += 4)
	{
		indices.push_back(i);
		indices.push_back(i + 1);
		indices.push_back(i + 2);
		indices.push_back(i + 2);
		indices.push_back(i + 3);
		indices.push_back(i);
	}
	vb.Creat(&vertices[0], vertices.size() * sizeof(Vertex));
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);

	ib.Creat(&indices[0], indices.size());
	SetShader(T_ID);
}

Cube::~Cube()
{
}

void Cube::SetShader(const unsigned int T_ID)
{
	shader.Creat("rsc/shaders/CubeVS.shader", "rsc/shaders/CubeFS.shader");
	shader.Bind();
	shader.Bind();
	shader.SetUniform1i("u_Texture",T_ID);
	shader.SetUniform3f("material.ambient", 1.0f, 1.0f, 1.0f);
	shader.SetUniform3f("material.diffuse", 1.0f, 1.0f, 1.0f);
	shader.SetUniform3f("material.specular", 1.0f, 1.0f, 1.0f);
	shader.SetUniform1f("material.shininess", 32.0f);
	shader.SetUniform3f("light.ambient", 0.2f, 0.2f, 0.2f);
	shader.SetUniform3f("light.diffuse", 1.0f, 1.0f, 1.0f);
	shader.SetUniform3f("light.specular", 1.0f, 1.0f, 1.0f);
}

void Cube::DrawUpdate(glm::mat4 proj, glm::mat4 view, glm::vec3 viewPos,float dt)
{
	timer += dt;
	glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
	if (timer >= 4) {
		timer = 0.0f;
	}
	if (timer >= 2.0f) {
		pos += glm::vec3(0.0f, 0.01f, 0.0f);
	}
	else
	{
		pos -= glm::vec3(0.0f, 0.01f, 0.0f);
	}
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f));
	glm::mat4 mvp = proj * view * model;
	shader.Bind();
	shader.SetUniformMat4f("u_MVP", mvp);
	shader.SetUniformMat4f("model", model);
	shader.SetUniform3f("viewPos", viewPos.x, viewPos.y, viewPos.z);
	render.DrawElement(va, ib, shader);
	shader.Unbind();
}

void Cube::LightUpdate(glm::vec3 lightPos, glm::vec3 lightColor)
{
	shader.Bind();
	shader.SetUniform3f("light.diffuse", lightColor.r, lightColor.g, lightColor.b);
	shader.SetUniform3f("light.specular", lightColor.r, lightColor.g, lightColor.b);
	shader.SetUniform3f("light.position", lightPos.x, lightPos.y, lightPos.z);
}
