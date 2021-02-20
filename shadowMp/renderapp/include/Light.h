#ifndef LIGHT_H_INCLUDED
#define LIGHT_H_INCLUDED

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
#include"Camera.h"

using std::vector;
using glm::vec3;
using glm::mat4;
namespace avocado {

	struct Light
	{
		vec3 Pos;
		glm::mat4 lightProjection;
		glm::mat4 lightView;
		glm::mat4 lightSpaceMatrix;
		vec3 Dir;
		bool Init();
	};

	struct depthquad
	{
		vertex_buffer vb_;
		vertex_layout layout_;
		renderer render_;
		void init();
		void renderquad();
		void destroy();
	};
}

#endif // 