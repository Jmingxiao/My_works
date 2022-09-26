$input o_viewDir,o_uv, o_normal,o_tangent, o_bitangent 

#include "../common/common.sh"

SAMPLER2D(texColor,  0);
SAMPLER2D(texNormal, 1);

uniform vec4 light;

void main()
{
	vec2 uv = vec2(o_uv.x,-o_uv.y);
	
	vec3 lightDir =normalize(light.xyz);
    vec3 lightColor = vec3(light.a,light.a,light.a);

	vec4 color = texture2D(texColor, uv);
	vec3 normalTS = texture2D(texNormal,uv).xyz*2.0-1.0;
	
	mat3 tbn = mtxFromCols(o_tangent,o_bitangent,o_normal);
	vec3 lightDirTS = mul(tbn,lightDir);

	
	vec3 viewDir = normalize(o_viewDir);
	
	//ambient
	vec3 ambientColor = 0.2f * color.rgb*lightColor;

	//diffuse
	float ndotl = max(dot(normalTS, lightDirTS), 0.0);
	vec3 diffColor = color.rgb*ndotl*lightColor;

	//specular blinn-phong
	vec3 halfwayDir = normalize(lightDirTS + viewDir); 
	float spec =pow(max(dot(normalTS, halfwayDir), 0.0), 16.0);
	vec3 specColor = vec3(0.2,0.2,0.2)*spec*lightColor;


	gl_FragColor = vec4( ambientColor+diffColor+specColor ,1);
	
}
