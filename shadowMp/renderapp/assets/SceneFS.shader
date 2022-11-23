
#version 330 core

out vec4 frag_color;

in VS_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;


uniform sampler2D u_texture;
uniform sampler2D shadowMap;

uniform vec3 lightDirection;

float ShadowCalculation(vec4 fragPosLightSpace)
{ 
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from lights perspective 
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // get depth of current fragment from lights perspective
    float currentDepth = projCoords.z;
    // calculate bias
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(-lightDirection);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    float shadow =0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);

        for (int x = -1; x <= 1; ++x)
        {
            for (int y = -1; y <= 1; ++y)
            {
                float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
                shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
            }
        }
        shadow /= 9.0;

    // keep the shadow at 0.0 when outside the light's frustum.
    if (projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}




void main() {

	vec3 textColor = texture(u_texture, fs_in.TexCoords).rgb;
    vec3 norm = normalize(fs_in.Normal);
    vec3 lightColor = vec3(1.0f);
    //ambient
    float ambientStrength = 0.3f;
    vec3  ambient = ambientStrength * lightColor;

    //diffuse
    vec3 lightDir = normalize(-lightDirection);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    //calculate shadow
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);
    vec3 lighting = (ambient + (1.0- shadow) * diffuse ) * textColor.rgb;

    frag_color = vec4(lighting, 1.0);
}