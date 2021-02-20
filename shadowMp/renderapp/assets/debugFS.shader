#version 330 core

in vec2 TexCoords;

uniform sampler2D DepthMap;
out vec4 color;

void main()
{
    float depthValue = texture(DepthMap, TexCoords).r;
    color = vec4(vec3(depthValue),1.0f);
};