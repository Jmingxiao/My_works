#include "HeightMap.h"
namespace avocado {
    avocado::Chunk::Chunk(unsigned int chunkX_, unsigned int chunkZ_, const uint8* data, const unsigned int size_x, const unsigned int size_z)
        :chunkX(chunkX_), chunkZ(chunkZ_)
    {//calculate each verticies in the chunk ;
        unsigned int chunksize_ = size * size;
        unsigned int indices_offset = chunksize_ * chunkZ * (size_x / size) + chunksize_ * chunkX;
        unsigned int offsetX = chunkX * (size-1);
        unsigned int offsetZ = chunkZ * (size-1);
        for (unsigned int j = 0; j < size; ++j) {
            for (unsigned int i = 0; i < size; ++i) {
                float x = (float)i + (float)offsetX;
                float z = (float)j + (float)offsetZ;
                unsigned int index = (unsigned int)z*size_x+ (unsigned int)x;
                float y = data[index] * 0.1f;
                vertices.push_back({ vec3(x/2,y/2,-z/2),vec2(x / size_x,z / size_z),vec3(0.0f,1.0f,0.0f) });
            }
        }
        // calculate each idicies in the chunk  
        for (uint32 i = 0; i < vertices.size()-size; i++)
        {
            if (i % size == size - 1) {
                continue;
            }
            indices.push_back(i + 1 + indices_offset);
            indices.push_back(i + indices_offset);
            indices.push_back(i + size + indices_offset);
            indices.push_back(i + size + indices_offset);
            indices.push_back(i + size + 1 + indices_offset);
            indices.push_back(i + 1 + indices_offset);
        }
        indicies_count_ = static_cast<unsigned int>(indices.size());
       // the normal calculation is a bit not so percise so the light looks a little wired ;
        for (int i = 0; i < (int)indices.size(); i += 3) {
            int k = i + 1;
            int q = i + 2;
            int a = indices[i] - indices_offset;
            int b = indices[k] - indices_offset;
            int c = indices[q] - indices_offset;
            vec3 v0 = vertices[a].position_;
            vec3 v1 = vertices[b].position_;
            vec3 v2 = vertices[c].position_;

            vec3 normal = normalize(cross(v2 - v0,v1 - v0));
            vertices[a].normal_ = normal;
            normal = normalize(cross(v0 - v1, v2 - v1));
            vertices[b].normal_ = normal;
            normal = normalize(cross(v1 - v2, v0 - v2));
            vertices[c].normal_ = normal;
        }
    }
    
    avocado::Chunk::~Chunk()
    {
        indices.clear();
        vertices.clear();
    }
    avocado::HeightMap::HeightMap():drawmode(triangles),lightPos(vec3(100.0f, 50.0f, -100.0f))
    {
    }

    avocado::HeightMap::~HeightMap()
    {

    }
    bool HeightMap::Init()
    { 
        //read shader
        string vertex_source;
        file_system::read_file_content("assets/heightMapVS.shader", vertex_source);

        string fragment_source;
        file_system::read_file_content("assets/heightMapFS.shader", fragment_source);

        shader_.create(vertex_source.c_str(), fragment_source.c_str());

        //creat texture 
        bitmap image;
        if (!image.create("assets/heightmap.png")) {
            return application::on_error("could not load bitmap image!");
        }

        texture_format format = texture::from_bitmap_format(image.pixel_format());
        int32 width = image.width();
        int32 height = image.height();
        const uint8* data = image.data();
        if (!texture_.create(format, width, height, data)) {
            return application::on_error("could not create texture!");
        } 


        //create chunk grid;
        vector<Chunk* > chunks_;
        unsigned int chunkNum_x = width / chunkSize;
        unsigned int chunkNum_z = height / chunkSize;
        vis.Init(chunkNum_x, chunkNum_z);
        for (unsigned int i = 0; i < chunkNum_z; i++)
        {
            for (unsigned int j = 0; j < chunkNum_x; j++)
            {
                chunks_.push_back(new Chunk(j, i, image.data_, width, height));
            }
            chunks_grid.push_back(chunks_);
            chunks_.clear();
        }

        //indicies and vertices

        vector<vertex> vertices_;
        vector<unsigned int> indices_;
         for (size_t i = 0; i < chunkNum_z; i++)
         {

             for (size_t p = 0; p < chunkNum_x; p++)
             {
                 unsigned int chunksize_ = (unsigned int)chunks_grid[i][p]->vertices.size();
                 for (size_t j = 0; j < (size_t)chunksize_; j++)
                 {
                     vertices_.push_back(chunks_grid[i][p]->vertices[j]);
                 }
                 size_t id_size = chunks_grid[i][p]->indices.size();
                 for (size_t k = 0; k < id_size; k++)
                 {
                     indices_.push_back(chunks_grid[i][p]->indices[k]);
                 }
             }
         }
         indicies_count = indices_.size();
         //bind two buffers
         if (!buffer_.create(BUFFER_ACCESS_MODE_STATIC, (int32)vertices_.size() * sizeof(vertex), &vertices_[0])) {
             return application::on_error("could not create vertex buffer!");
         }
         //indicies I used dynamic draw so that I could munipulate the chunk visibility and spatial partitioning 
         if (!ib.create((int32)indices_.size() * sizeof(unsigned int), nullptr)) {
             return application::on_error("could not create vertex buffer!");
         }
        //laout attributs
        {
            layout_.add_attribute(0, vertex_layout::ATTRIBUTE_FORMAT_FLOAT, 3, false);
            layout_.add_attribute(1, vertex_layout::ATTRIBUTE_FORMAT_FLOAT, 2, false);
            layout_.add_attribute(2, vertex_layout::ATTRIBUTE_FORMAT_FLOAT, 3, false);
        }
        // sampler
        {
            sampler_.create(SAMPLER_FILTER_MODE_LINEAR,
                SAMPLER_ADDRESS_MODE_CLAMP,
                SAMPLER_ADDRESS_MODE_CLAMP);
        } 
       
        return true;
    }

    void HeightMap::OnDraw()
    {
        renderer_.clear(0.1f, 0.2f, 0.4f, 1.0f);
        mat4 model = mat4(1.0f);
        mat4 mvp = camera_.projection_ * camera_.view_*model;
        renderer_.set_shader_program(shader_);
        renderer_.set_sampler_state(sampler_);
        renderer_.set_vertex_buffer(buffer_);
        renderer_.set_vertex_layout(layout_);
        renderer_.set_index_buffer(ib);
        DataControl();
        renderer_.set_depth_state(true, true);
        renderer_.set_texture(texture_);
        renderer_.set_shader_uniform(shader_, UNIFORM_TYPE_VEC3, "lightcolor", 1, glm::value_ptr(vec3(1.0f)));
        renderer_.set_shader_uniform(shader_, UNIFORM_TYPE_VEC3, "lightPos", 1, glm::value_ptr(lightPos));
        renderer_.set_shader_uniform(shader_, UNIFORM_TYPE_MATRIX, "u_MVP", 1, glm::value_ptr(mvp));
        renderer_.set_shader_uniform(shader_, UNIFORM_TYPE_MATRIX, "u_model", 1, glm::value_ptr(model));
        if (drawmode==triangles)// simple terrain mode
        {
            renderer_.draw_indexed(PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, INDEX_TYPE_UNSIGNED_INT, 0,(int)indicies_count); 
        }
        else                   //wire frame mode
        {
            renderer_.draw_indexed(PRIMITIVE_TOPOLOGY_LINE_LIST, INDEX_TYPE_UNSIGNED_INT, 0, (int)indicies_count);
        }
        vis.OnDraw();
    }

    void HeightMap::DataControl()
    {
        //grid spatial partitioning I think, the effect though looks good
        uint32 rightTop = chunkSize - 1;
        uint32 leftBot = chunkSize * (chunkSize - 1);
        uint32 count = 0;
        vector<unsigned int> tempIndex(indicies_count);
        
        for (uint32 i = 0; i < chunks_grid.size(); i++)
        {
            for ( uint32 j = 0; j < chunks_grid[0].size(); j++)
            {
                  vis.image.set_pixel(j, i, 0x00ffffff);
                if (frustum_.is_inside(chunks_grid[i][j]->vertices.front().position_)||frustum_.is_inside(chunks_grid[i][j]->vertices.back().position_)||
                    frustum_.is_inside(chunks_grid[i][j]->vertices[rightTop].position_) || frustum_.is_inside(chunks_grid[i][j]->vertices[leftBot].position_)) {
                    uint32 current = count * chunks_grid[i][j]->indicies_count_;
                    count++;
                    vis.image.set_pixel(j, i, 0x00ff0000);
                    memcpy(&tempIndex[current], &chunks_grid[i][j]->indices[0], chunks_grid[i][j]->indices.size() * sizeof(int));
                }
            }
        }
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, tempIndex.size()*sizeof(unsigned int),&tempIndex[0]);
    }
}