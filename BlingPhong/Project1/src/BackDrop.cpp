#include "BackDrop.h"

BackDrop::BackDrop()
{
	vector<Vertex_I> backDrop;
	vector<unsigned int> indices;
	backDrop.push_back({ glm::vec3(-5.0f, 0.0f,-5.0f),glm::vec3(0.0f, 0.0f, 1.0f),glm::vec2(0.0f, 0.0f),0.0f });
	backDrop.push_back({ glm::vec3( 5.0f, 0.0f,-5.0f),glm::vec3(0.0f, 0.0f, 1.0f),glm::vec2(6.0f, 0.0f),0.0f });
	backDrop.push_back({ glm::vec3( 5.0f, 5.0f,-5.0f),glm::vec3(0.0f, 0.0f, 1.0f),glm::vec2(6.0f, 6.0f),0.0f });
	backDrop.push_back({ glm::vec3(-5.0f, 5.0f,-5.0f),glm::vec3(0.0f, 0.0f, 1.0f),glm::vec2(0.0f, 6.0f),0.0f });

	backDrop.push_back({ glm::vec3( 5.0f, 0.0f, 0.0f),glm::vec3(-1.0f, 0.0f, 0.0f),glm::vec2(0.0f, 0.0f),0.0f });
	backDrop.push_back({ glm::vec3( 5.0f, 0.0f,-5.0f),glm::vec3(-1.0f, 0.0f, 0.0f),glm::vec2(6.0f, 0.0f),0.0f });
	backDrop.push_back({ glm::vec3( 5.0f, 5.0f,-5.0f),glm::vec3(-1.0f, 0.0f, 0.0f),glm::vec2(6.0f, 6.0f),0.0f });
	backDrop.push_back({ glm::vec3( 5.0f, 5.0f, 0.0f),glm::vec3(-1.0f, 0.0f, 0.0f),glm::vec2(0.0f, 6.0f),0.0f });

	backDrop.push_back({ glm::vec3(-5.0f, 0.0f, 0.0f),glm::vec3( 1.0f, 0.0f, 0.0f),glm::vec2(0.0f, 0.0f),0.0f });
	backDrop.push_back({ glm::vec3(-5.0f, 0.0f,-5.0f),glm::vec3( 1.0f, 0.0f, 0.0f),glm::vec2(6.0f, 0.0f),0.0f });
	backDrop.push_back({ glm::vec3(-5.0f, 5.0f,-5.0f),glm::vec3( 1.0f, 0.0f, 0.0f),glm::vec2(6.0f, 6.0f),0.0f });
	backDrop.push_back({ glm::vec3(-5.0f, 5.0f, 0.0f),glm::vec3( 1.0f, 0.0f, 0.0f),glm::vec2(0.0f, 6.0f),0.0f });

	backDrop.push_back({ glm::vec3(-5.0f, 0.0f,-5.0f),glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(0.0f, 0.0f),1.1f });
	backDrop.push_back({ glm::vec3( 5.0f, 0.0f,-5.0f),glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(6.0f, 0.0f),1.1f });
	backDrop.push_back({ glm::vec3( 5.0f, 0.0f, 0.0f),glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(6.0f, 6.0f),1.1f });
	backDrop.push_back({ glm::vec3(-5.0f, 0.0f, 0.0f),glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(0.0f, 6.0f),1.1f });



	for (size_t i = 0; i < backDrop.size(); i += 4)
	{
		indices.push_back(i);
		indices.push_back(i + 1);
		indices.push_back(i + 2);
		indices.push_back(i + 2);
		indices.push_back(i + 3);
		indices.push_back(i);
	}
	vb.Creat(&backDrop[0], backDrop.size() * sizeof(Vertex_I));
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);
	layout.Push<float>(1);
	va.AddBuffer(vb, layout);

	ib.Creat(&indices[0], indices.size());
	SetShader();
}

BackDrop::~BackDrop()
{
	va.Unbind();
	vb.Unbind();
	ib.Unbind();
}

void BackDrop::SetShader()
{
	shader.Creat("rsc/shaders/basicVS.shader", "rsc/shaders/basicFS.shader", nullptr);
	shader.Bind();

	shader.Bind();
	int samplers[2] = { 0,1 };
	shader.SetUniform1iv("u_Texture", 2, samplers);
	shader.SetUniform3f("material.ambient", 0.3f, 0.3f, 0.3f);
	shader.SetUniform3f("material.diffuse", 1.0f, 1.0f, 1.0f);
	shader.SetUniform3f("material.specular",0.5f, 0.5f, 0.5f);
	shader.SetUniform1f("material.shininess", 32.0f);
	shader.SetUniform3f("light.ambient", 1.0f, 1.0f, 1.0f);
	shader.SetUniform3f("light.diffuse", 1.0f, 1.0f, 1.0f);
	shader.SetUniform3f("light.specular", 1.0f, 1.0f, 1.0f);
}

void BackDrop::LightUpdate(glm::vec3 lightPos, glm::vec3 lightColor)
{
	shader.Bind();
	shader.SetUniform3f("light.diffuse", lightColor.r, lightColor.g, lightColor.b);
	shader.SetUniform3f("light.specular", lightColor.r, lightColor.g, lightColor.b);
	shader.SetUniform3f("light.position", lightPos.x, lightPos.y, lightPos.z);
}

void BackDrop::DrawUpdate(glm::mat4 proj, glm::mat4 view,glm::vec3 viewPos)
{
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f));
	glm::mat4 mvp = proj * view * model;
	shader.Bind();
	shader.SetUniformMat4f("u_MVP", mvp);
	shader.SetUniformMat4f("model", model);
	shader.SetUniform3f("viewPos", viewPos.x, viewPos.y, viewPos.z);
	render.DrawElement(va, ib, shader);
	shader.Unbind();
}
