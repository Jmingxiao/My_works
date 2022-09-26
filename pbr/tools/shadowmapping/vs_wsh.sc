$input a_position,a_normal
$output  o_posLS ,o_normal

#include "../common/common.sh"

uniform mat4 lightSpaceMatrix;

void main()
{
	gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));

	vec3 normal = a_normal.xyz*2.0 - 1.0;

	vec3 posWS = mul(u_model[0], vec4(a_position, 1.0)).xyz;
	o_normal = normal;
	o_posLS = mul(lightSpaceMatrix,vec4(posWS,1.0));
}