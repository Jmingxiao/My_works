#pragma once
#include "Renderer.h"
#include <GLFW/glfw3.h>
#include<iostream>
#include "Object.h"
#include<vector>

using std::vector;

class BackDrop
{
public:
	BackDrop();
	~BackDrop();
	void SetShader();
	void DrawUpdate(glm::mat4 proj, glm::mat4 view, glm::vec3 viewPos);
	void LightUpdate(glm::vec3 lightPos, glm::vec3 lightColor);
private:
	Shader shader;
	Texture texture;
	VertexBuffer vb;
	IndexBuffer ib;
	VertexBufferLayout layout;
	VertexArray va;
	Renderer render;
};
