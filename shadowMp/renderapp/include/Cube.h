#ifndef CUBE_H_INCLUEDED
#define CUBE_H_INCLUEDED

#include <avocado.hpp>
#include <avocado_render.hpp>
#include "avocado_opengl.h"
#include<vector>
#include"Camera.h"
#include"Vertex.h"
using std::vector;
using glm::vec3;
using glm::mat4;
namespace avocado {

	class Cube
	{
	private:
		renderer renderer_;
		vertex_buffer buffer_;
		index_buffer ib;
		vertex_layout layout_;
		int index_count = 0;
	public:
		texture texture_;
		sampler_state ss_;
		glm::mat4 model_;
		camera camera_;

		bool Init( mat4 model);
		void OnDraw(shader_program& shader_);
		void DrawDepth(shader_program &depthshader_);
		void Delete();

	};
}
#endif // !1