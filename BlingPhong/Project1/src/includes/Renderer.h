#pragma once
#include<GL/glew.h>
#include"VertexArray.h"
#include"IndexBuffer.h"
#include"Shader.h"
#include"VertexBuffer.h"
#include"VertexBufferLayout.h"
#include"Texture.h"

class Renderer
{


public:
		
	void DrawElement(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

};
