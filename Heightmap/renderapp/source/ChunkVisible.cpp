#include "ChunkVisible.h"

void avocado::ChunkVisible::Init(const uint32 width_, const uint32 height_)
{
    string vertex_source;
    file_system::read_file_content("assets/bitmapVS.shader", vertex_source);

    string fragment_source;
    file_system::read_file_content("assets/bitmapFS.shader", fragment_source);

    shader_.create(vertex_source.c_str(), fragment_source.c_str());
    image.create(bitmap::format::rgba8, width_, height_);
    image.fill(0xffffffff);
    texture_format format = texture::from_bitmap_format(image.pixel_format());
    int32 width = image.width();
    int32 height = image.height();
    const uint8* data = image.data();
    texture_.create(format, width_, height_, data);
    const float Q = 0.3f;
    const bitMapVertexFormat vertices[] =
    {
        { glm::vec3(-Q, Q, Q),glm::vec2(0.0f,0.0f)},
        { glm::vec3(-Q,-Q, Q),glm::vec2(0.0f,1.0f)},
        { glm::vec3( Q,-Q, Q),glm::vec2(1.0f,1.0f)},
        { glm::vec3( Q,-Q, Q),glm::vec2(1.0f,1.0f)},
        { glm::vec3( Q, Q, Q),glm::vec2(1.0f,0.0f)},
        { glm::vec3(-Q, Q, Q),glm::vec2(0.0f,0.0f)}
    };
    buffer_.create(BUFFER_ACCESS_MODE_STATIC, sizeof(vertices), vertices);

    // note: specify the vertex layout
    layout_.add_attribute(0, vertex_layout::ATTRIBUTE_FORMAT_FLOAT, 3, false);
    layout_.add_attribute(1, vertex_layout::ATTRIBUTE_FORMAT_FLOAT, 2, false);
    sampler_.create(SAMPLER_FILTER_MODE_LINEAR,
        SAMPLER_ADDRESS_MODE_CLAMP,
        SAMPLER_ADDRESS_MODE_CLAMP);

    
}
void avocado::ChunkVisible::OnDraw()
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.7f, 0.7f, 0.0f));
    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    renderer_.set_shader_program(shader_);
    renderer_.set_sampler_state(sampler_);
    renderer_.set_vertex_buffer(buffer_);
    renderer_.set_vertex_layout(layout_);
    PixelUpdate();
    renderer_.set_texture(texture_);
    renderer_.set_shader_uniform(shader_, UNIFORM_TYPE_MATRIX, "u_Model", 1, glm::value_ptr(model));
    renderer_.draw(PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,0,6);
}

void avocado::ChunkVisible::PixelUpdate()
{
    texture_format format = texture::from_bitmap_format(image.pixel_format());
    int32 width = image.width();
    int32 height = image.height();
    const uint8* data = image.data();
    texture_.update(format, width, height, data);
}
