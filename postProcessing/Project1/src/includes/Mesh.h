#pragma once

#include <string>
#include"Renderer.h"
#include<vector>
#include<iostream>

using std::cout;
using std::endl;
using std::cin;
using std::vector;

struct VertexM {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
};

struct TextureM {
    unsigned int id;
    string type;
    string path;
};

class Mesh
{
private:
    unsigned int VBO, EBO;

	void setupMesh();
public:
    unsigned int VAO;
	vector<VertexM> vertices;
	vector<unsigned int> indices;
	vector<TextureM> textures;

	Mesh(vector<VertexM> vertices, vector<unsigned int> indices, vector<TextureM> texture);
	void Draw(Shader& shader);
	

		
};


