#include "Cube.h"


bool avocado::Cube::Init(mat4 model)
{
	model_ = model;

	bitmap image;
	if (!image.create("assets/crate.png")) {
		return application::on_error("could not load bitmap image!");
	}

	texture_format format = texture::from_bitmap_format(image.pixel_format());
	int32 width = image.width();
	int32 height = image.height();
	const uint8* data = image.data();
	texture_.create(format, width, height, data);

	// sampler
	{
		ss_.create(SAMPLER_FILTER_MODE_LINEAR, SAMPLER_ADDRESS_MODE_CLAMP, SAMPLER_ADDRESS_MODE_CLAMP);
	}


	vector<Vertex> vertices;
	vector<unsigned int> indices;
	const float Q = 0.5f;
	vertices.push_back({ glm::vec3(-Q,-Q,-Q),glm::vec3(0.0f, 0.0f, -1.0f),glm::vec2(0.0f, 0.0f) });
	vertices.push_back({ glm::vec3(	Q,-Q,-Q),glm::vec3(0.0f, 0.0f, -1.0f),glm::vec2(1.0f, 0.0f) });
	vertices.push_back({ glm::vec3(	Q, Q,-Q),glm::vec3(0.0f, 0.0f, -1.0f),glm::vec2(1.0f, 1.0f) });
	vertices.push_back({ glm::vec3(-Q, Q,-Q),glm::vec3(0.0f, 0.0f, -1.0f),glm::vec2(0.0f, 1.0f) });
									  
	vertices.push_back({ glm::vec3(-Q,-Q, Q),glm::vec3(0.0f, 0.0f, 1.0f),glm::vec2(0.0f, 0.0f) });
	vertices.push_back({ glm::vec3(	Q,-Q, Q),glm::vec3(0.0f, 0.0f, 1.0f),glm::vec2(1.0f, 0.0f) });
	vertices.push_back({ glm::vec3(	Q, Q, Q),glm::vec3(0.0f, 0.0f, 1.0f),glm::vec2(1.0f, 1.0f) });
	vertices.push_back({ glm::vec3(-Q, Q, Q),glm::vec3(0.0f, 0.0f, 1.0f),glm::vec2(0.0f, 1.0f) });
									   
	vertices.push_back({ glm::vec3(-Q, Q, Q),glm::vec3(-1.0f, 0.0f, 0.0f),glm::vec2(0.0f, 0.0f) });
	vertices.push_back({ glm::vec3(-Q, Q,-Q),glm::vec3(-1.0f, 0.0f, 0.0f),glm::vec2(1.0f, 0.0f) });
	vertices.push_back({ glm::vec3(-Q,-Q,-Q),glm::vec3(-1.0f, 0.0f, 0.0f),glm::vec2(1.0f, 1.0f) });
	vertices.push_back({ glm::vec3(-Q,-Q, Q),glm::vec3(-1.0f, 0.0f, 0.0f),glm::vec2(0.0f, 1.0f) });
									 
	vertices.push_back({ glm::vec3(	Q, Q, Q),glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(0.0f, 0.0f) });
	vertices.push_back({ glm::vec3(	Q, Q,-Q),glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(1.0f, 0.0f) });
	vertices.push_back({ glm::vec3(	Q,-Q,-Q),glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(1.0f, 1.0f) });
	vertices.push_back({ glm::vec3(	Q,-Q, Q),glm::vec3(1.0f, 0.0f, 0.0f),glm::vec2(0.0f, 1.0f) });
									 
	vertices.push_back({ glm::vec3(-Q,-Q,-Q),glm::vec3(0.0f,-1.0f, 0.0f),glm::vec2(0.0f, 0.0f) });
	vertices.push_back({ glm::vec3(	Q,-Q,-Q),glm::vec3(0.0f,-1.0f, 0.0f),glm::vec2(1.0f, 0.0f) });
	vertices.push_back({ glm::vec3(	Q,-Q, Q),glm::vec3(0.0f,-1.0f, 0.0f),glm::vec2(1.0f, 1.0f) });
	vertices.push_back({ glm::vec3(-Q,-Q, Q),glm::vec3(0.0f,-1.0f, 0.0f),glm::vec2(0.0f, 1.0f) });
									   
	vertices.push_back({ glm::vec3(-Q, Q,-Q),glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(0.0f, 0.0f) });
	vertices.push_back({ glm::vec3(	Q, Q,-Q),glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(1.0f, 0.0f) });
	vertices.push_back({ glm::vec3(	Q, Q, Q),glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(1.0f, 1.0f) });
	vertices.push_back({ glm::vec3(-Q, Q, Q),glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(0.0f, 1.0f) });

	for (unsigned int i = 0; i < vertices.size(); i += 4)
	{
		indices.push_back(i);
		indices.push_back(i + 1);
		indices.push_back(i + 2);
		indices.push_back(i + 2);
		indices.push_back(i + 3);
		indices.push_back(i);
	}
	index_count = static_cast<int>(indices.size());
	//bind two buffers
	if (!buffer_.create(BUFFER_ACCESS_MODE_STATIC, (int32)vertices.size() * sizeof(Vertex), &vertices[0])) {
		return application::on_error("could not create vertex buffer!");
	}
	//indicies I used dynamic draw so that I could munipulate the chunk visibility and spatial partitioning 
	if (!ib.create((int32)indices.size() * sizeof(unsigned int), &indices[0])) {
		return application::on_error("could not create vertex buffer!");
	}
	//laout attributs
	{
		layout_.add_attribute(0, vertex_layout::ATTRIBUTE_FORMAT_FLOAT, 3, false);
		layout_.add_attribute(1, vertex_layout::ATTRIBUTE_FORMAT_FLOAT, 3, false);
		layout_.add_attribute(2, vertex_layout::ATTRIBUTE_FORMAT_FLOAT, 2, false);
	}

	return true;
}

void avocado::Cube::OnDraw(shader_program &shader_)
{
	
	glm::mat4 mvp = camera_.projection_ * camera_.view_ * model_;
	renderer_.set_shader_program(shader_);
	renderer_.set_vertex_buffer(buffer_);
	renderer_.set_vertex_layout(layout_);
	renderer_.set_index_buffer(ib);
	renderer_.set_shader_uniform(shader_, UNIFORM_TYPE_MATRIX, "u_MVP", 1, glm::value_ptr(mvp));
	renderer_.set_shader_uniform(shader_, UNIFORM_TYPE_MATRIX, "u_model", 1, glm::value_ptr(model_));
	renderer_.draw_indexed(PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, INDEX_TYPE_UNSIGNED_INT, 0, index_count);

}

void avocado::Cube::DrawDepth(shader_program& depthshader_)
{
	model_ = glm::rotate(model_, glm::radians(0.1f), vec3(1.0f, 1.0f, 0.0f));
	renderer_.set_shader_uniform(depthshader_, UNIFORM_TYPE_MATRIX, "model", 1, glm::value_ptr(model_));
	renderer_.set_vertex_buffer(buffer_);
	renderer_.set_vertex_layout(layout_);
	renderer_.set_index_buffer(ib);
	renderer_.draw_indexed(PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, INDEX_TYPE_UNSIGNED_INT, 0, index_count);
}

void avocado::Cube::Delete()
{
	buffer_.destroy();
	ib.destroy();
	texture_.destroy();
}




