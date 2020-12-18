#include "Mesh.h"

Mesh::Mesh(vector<VertexM> vertices, vector<unsigned int> indices, vector<TextureM> texture)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = texture;
	setupMesh();

}

void Mesh::setupMesh()
{
	m_vb.Creat(&vertices[0],vertices.size()*sizeof(Vertex));
	m_ib.Creat(&indices[0], indices.size()*sizeof(unsigned int));
	m_layout.Push<float>(3);
	m_layout.Push<float>(3);
	m_layout.Push<float>(2);
	m_layout.Push<float>(3);
	m_layout.Push<float>(3);
	m_va.AddBuffer(m_vb, m_layout);

}

void Mesh::Draw(Shader& shader)
{
}
