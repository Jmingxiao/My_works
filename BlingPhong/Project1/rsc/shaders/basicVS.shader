#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float texID;

out vec3 oNormal;
out vec3 FragPos;
out vec2 v_TexCoord;
out float o_texID;


uniform mat4 u_MVP;
uniform mat4 model;

void main() 
{
    gl_Position = u_MVP * vec4(position,1.0);
    v_TexCoord = texCoord;
    FragPos = vec3(model * vec4(position, 1.0));
    oNormal = aNormal;
    o_texID = texID;
};
