$input o_uv

#include "../common/common.sh"

SAMPLERCUBE(skybox, 0);

void main()
{
    gl_FragColor = textureCubeLod(skybox , o_uv,0); 
}