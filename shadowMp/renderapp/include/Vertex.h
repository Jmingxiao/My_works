
#ifndef VERTEX_H_INCLUDED
#define VERTEX_H_INCLUDED

#pragma warning(push)
#pragma warning(disable: 4127)
#pragma warning(disable: 4201)
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#pragma warning(pop)

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textCoord;

};
#endif // !VERTEX_H_INCLUDED