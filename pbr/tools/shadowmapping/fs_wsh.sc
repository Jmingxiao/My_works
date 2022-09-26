$input  o_posLS,o_normal

#include "../common/common.sh"

SAMPLER2D(shadowMap, 0);

uniform vec4 light;



void main()
{
	
	vec3 lightDir =normalize(light.xyz);
    vec3 lightColor = vec3(light.a,light.a,light.a);

	vec4 color =vec4(0.8,0.8,0.8,1.0);
	vec3 normalWS = o_normal;
	
	//ambient
	vec3 ambientColor = 0.05f * color.rgb*lightColor;

	//diffuse
	float ndotl = max(dot(normalWS, lightDir), 0.0);
	vec3 diffColor = color.rgb*ndotl*lightColor;


	vec4 posls = o_posLS;
	vec3 lightprojcoord = posls.xyz/ posls.w;
	float lightdepth = texture2D(shadowMap,lightprojcoord.xy).r;
	float currentdepth = lightprojcoord.z;
	float bias = max(0.05 * (1.0 -ndotl), 0.005);

	float shadow = 0.0;
	vec2 texSize = vec2(1.0,1.0)/vec2(512.0,512.0);

	for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture2D(shadowMap, lightprojcoord.xy + vec2(x, y) * texSize).r;
            shadow += (currentdepth - bias)> pcfDepth ? 1.0:0.0 ;
        }
    }
    shadow /= 9.0;
	if (lightprojcoord.z > 1.0)
        shadow = 0.0;

	vec3 result = diffColor*(1.0-shadow)+ambientColor;

	gl_FragColor = vec4( result ,color.a);
	
}
