#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 texCoord;

out vec3 oNormal;
out vec3 FragPos;
out vec2 v_TexCoord;

uniform sampler2D heightmap;
uniform mat4 u_MVP;
uniform mat4 model;

void main()
{
    vec3 _vertex = position;
  /*  _vertex.y = texture(heightmap, texCoord).r * 0.1f;*/
    gl_Position = u_MVP * vec4(_vertex, 1.0);
    v_TexCoord = texCoord;
    FragPos = vec3(model * vec4(_vertex, 1.0));
    oNormal = aNormal;
};