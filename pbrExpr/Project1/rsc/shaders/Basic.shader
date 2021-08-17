#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 texCoord;


out vec3 oNormal;
out vec3 FragPos;
out vec2 v_TexCoord;


uniform mat4 u_MVP;
uniform mat4 model;
uniform mat4 modelRot;

void main() 
{
    gl_Position = u_MVP * vec4(position,1.0);
    v_TexCoord = texCoord;
    FragPos = vec3(model * vec4(position, 1.0));
    oNormal = vec3(modelRot * vec4(aNormal, 1.0));
    /*oNormal = aNormal;*/
};

#shader fragment
#version 330 core

out vec4 color;
in vec3 oNormal;
in vec3 FragPos;
in vec2 v_TexCoord;

uniform sampler2D u_Texture[10];
uniform vec3 lightcolor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform int index;

void main()
{
    //ambient
    float ambientStrength = 0.2;
    vec3  ambient = ambientStrength * lightcolor;
    
    //diffuse
    vec3 norm = normalize(oNormal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightcolor;

    //specular
    float specularStrength = 0.4;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = specularStrength * spec * lightcolor;

    vec4 texColor = texture(u_Texture[index], v_TexCoord);
    if (index==0) {
        color = texColor;
    }
    else {
        color = vec4(ambient+diffuse+specular,1.0)*texColor;
    }
};
