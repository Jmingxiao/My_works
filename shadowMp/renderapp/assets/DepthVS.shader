#version 330 core

layout(location = 0) in vec3 a_position;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(a_position, 1.0);
}