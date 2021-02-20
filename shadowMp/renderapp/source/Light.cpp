#include "Light.h"

bool avocado::Light::Init()
{
	Pos = vec3(-1.5f, 3.0f, 1.0f);
	Dir = glm::normalize(-Pos);
	lightProjection = glm::ortho(-13.0f, 13.0f, -13.0f, 13.0f, 1.0f, 10.0f);
	lightView = glm::lookAt(Pos,vec3(0.0f), vec3(0.0f,1.0f,0.0f));
	lightSpaceMatrix = lightProjection *lightView;

    return true;
}

void avocado::depthquad::init()
{
	float quadVertices[] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f,	0.4f, 0.0f, 0.0f, 0.0f,
		-0.4f,  0.4f, 0.0f, 1.0f, 0.0f,
		-0.4f,  0.4f, 0.0f, 1.0f, 0.0f,
		-0.4f,  1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
	};
	vb_.create(BUFFER_ACCESS_MODE_STATIC, sizeof(quadVertices), quadVertices);
	layout_.add_attribute(0, vertex_layout::ATTRIBUTE_FORMAT_FLOAT, 3, false);
	layout_.add_attribute(1, vertex_layout::ATTRIBUTE_FORMAT_FLOAT, 2, false);


}

void avocado::depthquad::renderquad()
{
	render_.set_vertex_buffer(vb_);
	render_.set_vertex_layout(layout_);
	render_.draw(PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 0, 6);
}

void avocado::depthquad::destroy()
{
	vb_.destroy();

}
