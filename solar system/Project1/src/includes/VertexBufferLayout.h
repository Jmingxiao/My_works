#pragma once
#include<vector>
#include"Renderer.h"
#include<iostream>

struct VertexBufferElement
{
	unsigned int count;
	unsigned int type;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_INT: return 4;
		case GL_UNSIGNED_BYTE: return 1;
		}
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement>m_Element;
	unsigned int m_Stride;
public:
	VertexBufferLayout():m_Stride(0){}

	template<typename T>
	void Push(unsigned int count) 
	{
		//static_assert(false,"why������");
		std::cout << "why������";
	}
	template<>
	void Push<float>(unsigned int count) {
		m_Element.push_back({ count,GL_FLOAT,GL_FALSE });
		m_Stride += count* VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}
	template<>
	void Push<int>(unsigned int count) {
		m_Element.push_back({ count,GL_INT,GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_INT);
	}
	template<>
	void Push<unsigned int>(unsigned int count) {
		m_Element.push_back({ count,GL_UNSIGNED_INT,GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}
	template<>
	void Push<unsigned char>(unsigned int count) {
		m_Element.push_back({ count,GL_UNSIGNED_BYTE,GL_TRUE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);

	}
	inline const std::vector<VertexBufferElement> GetElemrnts() const { return  m_Element; }
	inline unsigned int GetStride() const { return m_Stride; }
};
