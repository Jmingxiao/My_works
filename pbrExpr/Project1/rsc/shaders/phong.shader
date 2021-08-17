#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;
layout(location = 2) in vec3 aNormal;


out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    TexCoords = aTexCoords;
    WorldPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;

    gl_Position = projection * view * vec4(WorldPos, 1.0);
};

#shader fragment
#version 330 core

out vec4 color;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

uniform vec3 albedo;

// lights
uniform vec3 lightPositions;
uniform vec3 lightColors;

uniform vec3 camPos;

void main()
{
    //ambient
    float ambientStrength = 0.3;
    vec3  ambient = ambientStrength * lightColors;

    //diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPositions - WorldPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColors;

    //specular
    float specularStrength = 0.4;
    vec3 viewDir = normalize(camPos - WorldPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColors;

    color = vec4(ambient + diffuse + specular, 1.0) * vec4(albedo,1.0f);
};