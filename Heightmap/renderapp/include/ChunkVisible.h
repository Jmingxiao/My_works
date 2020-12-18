#pragma warning(push)
#pragma warning(disable: 4127)
#pragma warning(disable: 4201)
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#pragma warning(pop)
#include <avocado.hpp>
#include <avocado_render.hpp>
#include "avocado_opengl.h"
#include<vector>
using std::vector;
namespace avocado {
	struct bitMapVertexFormat
	{
		glm::vec3 position;
		glm::vec2 textCoord;

	};
class ChunkVisible
{
private:
	renderer renderer_;
	shader_program shader_;
	vertex_buffer buffer_;
	index_buffer ib;
	vertex_layout layout_;
	sampler_state sampler_;

public:
	bitmap image;
	texture texture_;
	void Init(const uint32 width,const uint32 height);
	void OnDraw();
	void PixelUpdate();


};
}