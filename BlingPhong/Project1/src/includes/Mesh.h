#pragma once
#include"Object.h"
#include"Texture.h"
#include"VertexBuffer.h"
#include"VertexArray.h"
#include"VertexBufferLayout.h"
#include"IndexBuffer.h"

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
	VertexBuffer m_vb;
	IndexBuffer m_ib;
	VertexBufferLayout m_layout;
	VertexArray m_va;

	void setupMesh();
public:

	vector<VertexM> vertices;
	vector<unsigned int> indices;
	vector<TextureM> textures;

	Mesh(vector<VertexM> vertices, vector<unsigned int> indices, vector<TextureM> texture);
	void Draw(Shader& shader);
	

		
};


