#pragma once
#include "Object.h"
#include "Renderer.h"
#include"VertexBuffer.h"
#include"VertexBufferLayout.h"
#include"Texture.h"`
#include <GLFW/glfw3.h>
#include<iostream>

enum CameraMode
{
	FORWARD,BACKWARD,TOPWARD,DOWNWARD
};

class SolarSys {
private:
	VertexBuffer vb;
	VertexArray va;
	VertexBufferLayout layout;
	IndexBuffer ib;
	Renderer m_renderer;
	Shader shader;
	Texture texture;
	unsigned int m_timer_e;
	unsigned int m_timer_m; 
	float SunRot;
	float EarthRot;
	glm::mat4 proj;
	glm::mat4 earthPos;
	glm::mat4 view;
	CameraMode mode;
	float camRot;
public:
	SolarSys();
	~SolarSys();

	void SetShader();
	void DrawSun();
	void DrawEarth();
	void DrawMoon();
	void processInputs(GLFWwindow* window);
	
};