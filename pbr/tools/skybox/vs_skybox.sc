$input a_position
$output o_uv

#include "../common/common.sh"

void main()
{
    o_uv = a_position;
    vec4 pos = mul(u_modelViewProj ,vec4(a_position, 1.0));
    gl_Position = pos.xyww;
}