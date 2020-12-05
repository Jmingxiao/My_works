#pragma once
#include"Renderer.h"

using std::string;

class Texture 
{
private:
	unsigned int m_RendererID;
	string m_Filepath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;

public:
	Texture();
	~Texture();

	unsigned int CreatTexture(const string& path);
	void Bind(unsigned int slot = 0)const;
	void Unbind()const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }


};