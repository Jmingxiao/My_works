#pragma once
class VertexBuffer
{
private:
	unsigned int m_RendererID;
public:
	VertexBuffer();
	~VertexBuffer();
	void Creat(const void* data, unsigned int size);
	void DynamicDraw(const void* data, unsigned int size);
	void Subdata(const void* data, unsigned int size, unsigned int offset);
	void StreamDraw(const void* data, unsigned int size);
	void Bind()const;
	void Unbind()const;


};