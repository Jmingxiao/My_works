#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texcoord;


out VS_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} vs_out;

uniform mat4 u_MVP;
uniform mat4 u_model;
uniform mat4 lightSpaceMatrix;


void main() {
    vs_out.TexCoords = a_texcoord;
	vs_out.Normal = transpose(inverse(mat3(u_model))) * a_normal;
    vs_out.FragPos = vec3(u_model * vec4(a_position, 1.0f));
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0f);
	gl_Position = u_MVP * vec4(a_position, 1.0f);
}