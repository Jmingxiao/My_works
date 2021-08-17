#pragma once
#include "Object.h"
#include "Texture.h"
#include <string>
#include <vector>
class Skybox
{
public:
	Skybox();
	~Skybox();
	void Render(const glm::mat4& proj, const glm::mat4& view);
private:
	VertexBuffer vb;
	VertexArray va;
	VertexBufferLayout layout;
	Renderer m_renderer;
	Shader shader;
	Texture texture;
};

Skybox::Skybox()
{
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
	vb.Creat(skyboxVertices, sizeof(skyboxVertices));
	layout.Push<float>(3);
	va.AddBuffer(vb, layout);

	vector<string>faces
	{ "rsc/textures/skybox/right.jpg",
	  "rsc/textures/skybox/left.jpg",
	  "rsc/textures/skybox/top.jpg",
	  "rsc/textures/skybox/bottom.jpg",
	  "rsc/textures/skybox/front.jpg",
	  "rsc/textures/skybox/back.jpg"
	};
	shader.Creat("rsc/shaders/skybox.shader");
	texture.CreatCubeMap(faces);
	shader.SetUniform1i("skybox",0);
}

Skybox::~Skybox()
{


}

void Skybox::Render(const glm::mat4& proj, const glm::mat4& view) {
	
	glDepthFunc(GL_LEQUAL);
	shader.Bind();
	auto projview = proj * view;
	shader.SetUniformMat4f("projview", projview);
	texture.Bind(0);
	shader.SetUniform1i("skybox", 0);
	va.Bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthFunc(GL_LESS);
}