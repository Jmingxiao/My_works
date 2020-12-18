#version 330

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texcoord;
layout(location = 2) in vec3 a_normal;

uniform mat4 u_MVP;
uniform mat4 u_model;

out vec2 f_texcoord;
out vec3 f_normal;
out vec3 FragPos;

void main() {
	gl_Position = u_MVP * vec4(a_position, 1);
	f_texcoord = a_texcoord;
	f_normal = a_normal;
	FragPos = vec3(u_model * vec4(a_position, 1.0f));
}