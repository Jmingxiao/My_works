#pragma once
#include "Object.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"`
#include <GLFW/glfw3.h>
#include<iostream>

enum PlanetType
{
	 Sun,Earth,Moon,Jupiter,Mars,Mercury,Neptune,Saturn,Uranus,Venus,
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
	unsigned int planet_ellipse[10] = {0};
	float SunRot;
	float PlanetRot;
	glm::mat4 proj_;
	glm::mat4 earthPos;
	glm::mat4 view_;
	float camRot;
public:
	SolarSys();
	~SolarSys();

	void SetShader();
	void DrawSun();
	void DrawEarth();
	void DrawMoon();
	void DrawMercury();
	void DrawVenus();
	void DrawMars();
	void DrawJupiter();
	void DrawSaturn();
	void DrawUranus();
	void DrawNeptune();
	void CameraUpdate(glm::mat4 proj, glm::mat4 view, glm::vec3 viewPos);
	
};