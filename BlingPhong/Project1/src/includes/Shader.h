#pragma once

#include<string>
#include<unordered_map>

#include"glm/glm.hpp"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:

	std::unordered_map<std::string, int> m_UniformLocationCache;
	unsigned int m_RendererID;

public:
	Shader();
    ~Shader();
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
	void Creat(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
	void Bind()const;
	void Unbind() const;
	//ser uniforms
	void SetUniform1iv(const std::string& name, int count, int* value);
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniform4f(const std::string& name,float v0, float v1,float v2,float v3 );
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

private:
	int GetUniformLocation(const std::string& name);
	void checkCompileErrors(unsigned int shader, std::string type);
};