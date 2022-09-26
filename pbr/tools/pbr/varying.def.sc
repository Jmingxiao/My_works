vec2 o_uv           : TEXCOORD0 = vec2(0.0,0.0);
vec3 o_viewDir      : TEXCOORD1 = vec3(0.0, 0.0, 0.0);
vec3 o_normal       : NORMAL    = vec3(0.0, 0.0, 1.0);
vec3 o_tangent      : TANGENT   = vec3(1.0, 0.0, 0.0);
vec3 o_bitangent    : BINORMAL  = vec3(0.0, 1.0, 0.0);


vec3 a_position     : POSITION;
vec4 a_normal       : NORMAL;
vec4 a_tangent      : TANGENT;
vec2 a_texcoord0    : TEXCOORD0;
