
#version 330

out vec4 frag_color;

in  vec2 f_texcoord;
in  vec3 f_normal;
in vec3 FragPos;

uniform sampler2D u_texture;
uniform vec3 lightcolor;
uniform vec3 lightPos;


void main() {

    //ambient
    float ambientStrength = 0.4;
    vec3  ambient = ambientStrength * lightcolor;

    //diffuse
    vec3 norm = normalize(f_normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightcolor;


	vec4 textColor = texture(u_texture, f_texcoord);
    frag_color = vec4(ambient + diffuse, 1.0) * textColor.r;
}