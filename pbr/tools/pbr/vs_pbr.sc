$input a_position,a_normal,a_tangent,a_texcoord0
$output  o_viewDir, o_uv,o_normal,o_tangent, o_bitangent

#include "../common/common.sh"

void main()
{
	gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
	vec3 normal = a_normal.xyz*2.0 - 1.0;
	vec3 tangent = a_tangent.xyz;
	vec3 posWS = mul(u_model[0], vec4(a_position, 1.0) ).xyz;
	vec3 viewPos = mul(vec4(0.0, 0.0, 0.0, 1.0), u_view).xyz;

	normal = normalize( mul(u_model[0],vec4(normal,0))).xyz;
	tangent = normalize( mul(u_model[0],vec4(tangent,0))).xyz;

	o_normal = normal;
	o_tangent = normalize(tangent - dot(tangent, normal) * normal);
	o_bitangent = cross(normal,tangent);
	o_viewDir = viewPos - posWS;
	o_uv = a_texcoord0;


}