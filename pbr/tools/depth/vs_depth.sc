$input a_position
$output o_position
#include "../common/common.sh"

void main()
{
	gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
	o_position = gl_Position;
}