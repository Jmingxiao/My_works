// triangle.vs.txt

#version 330

layout(location=0) in vec3 a_position;
layout(location=1) in vec2 a_texcoord;

uniform mat4 u_Model;

out vec2 f_texcoord;
out vec3 f_normal;

void main() {
	gl_Position = u_Model*vec4(a_position, 1);
	f_texcoord = a_texcoord;
}
