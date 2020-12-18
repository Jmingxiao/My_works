#pragma once
#include "Renderer.h"
#include <GLFW/glfw3.h>
#include<iostream>
#include "Object.h"
#include<vector>

using std::vector;

class Cube
{
public:
	Cube(glm::vec3 Pos, unsigned int T_ID);
	~Cube();
	void SetShader(unsigned int T_ID);
	void DrawUpdate(glm::mat4 proj, glm::mat4 view, glm::vec3 viewPos, float dt);
	void LightUpdate(glm::vec3 lightPos, glm::vec3 lightColor);
private:
	Shader shader;
	Texture texture;
	VertexBuffer vb;
	IndexBuffer ib;
	VertexBufferLayout layout;
	VertexArray va;
	Renderer render;
	float timer;
public:
	glm::vec3 pos;
};