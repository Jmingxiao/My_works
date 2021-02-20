#pragma once
#include"Renderer.h"


using std::string;
using std::vector;




class Texture 
{
private:
	string m_Filepath;

public:
	unsigned int m_RendererID;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;

	Texture();
	~Texture();
	Texture(const string& path);


	unsigned int CreatTexture(const string& path);
	unsigned int CreatRepeatTexture(const string& path);
	unsigned int CreatCubeMap(vector<string>faces);
	void Bind(unsigned int slot = 0)const;
	void BindCubeMap(unsigned int slot = 0) const;
	void Unbind()const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }


};