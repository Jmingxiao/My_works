$input o_position
#include "../common/common.sh"

void main()
{	
	float depth = o_position.z/o_position.w;
	gl_FragColor = packFloatToRgba(depth);
}