#pragma once
#include"glm/gtc/matrix_transform.hpp"
#include"glm/glm.hpp"

struct Vertex_I {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	float textureID;
};
struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};