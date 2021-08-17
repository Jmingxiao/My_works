#pragma once
#include"glm/gtc/matrix_transform.hpp"
#include"glm/glm.hpp"
#include "Renderer.h"
#include"VertexBuffer.h"
#include"VertexBufferLayout.h"
#include "Texture.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Sphere {
private:


	VertexBuffer vb;
	VertexArray va;
	VertexBufferLayout layout;
	IndexBuffer ib;
	Renderer m_renderer;
	Shader shader;
	Texture albedo;
	Texture ao;
	Texture normal;
	Texture metallic;
	Texture roughness;
	Texture skybox;
public :
	
	bool withTexture;
	glm::vec3 Pos;
	Sphere();
	Sphere(const glm::vec3& pos);
	~Sphere();
	inline void init();
	void SetPBRShader();
	void SetPBRTexShader();
	void SetPhongShader();
	void SetTransparent();

	void Update(const glm::mat4& proj, const glm::mat4& view, const glm::vec3& viewPos, const glm::vec3& lightPos);
};

class Light
{
private:
	VertexBuffer vb;
	VertexArray va;
	VertexBufferLayout layout;
	Renderer m_renderer;
	Shader shader;
public:
	
	glm::vec3 Pos;
	Light();
	
	~Light();
	void Input(const glm::vec3& Dir, float dt);
	void Update(const glm::mat4& proj, const glm::mat4& view);

};