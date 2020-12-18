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
#include"camera.hpp"
#include"ChunkVisible.h"


using std::vector;
using glm::vec3;
using glm::vec2;
using glm::mat4;
namespace avocado {
	enum DrawMode
	{
		triangles , wireframe
	};
	struct vertex {
		glm::vec3 position_;
		glm::vec2 texcoord_;
		glm::vec3 normal_;
	};
	struct Chunk
	{
		Chunk(unsigned int chunkX_, unsigned int chunkZ_, const uint8* data, const unsigned int size_x, const unsigned int size_z);
		~Chunk();

		unsigned int chunkX, chunkZ;
		const unsigned int size = 8;
		unsigned int indicies_count_;
		vector<vertex> vertices;
		vector<unsigned int> indices;

	};
	class HeightMap
	{
	public:
		HeightMap();
		~HeightMap();
		bool Init();
		void OnDraw();
		void DataControl();
		void ChunkVisibility();

		camera camera_;
		DrawMode drawmode;
		frustum frustum_;
		vec3 lightPos;
	private:
		const unsigned int chunkSize = 8;
		ChunkVisible vis;
		renderer renderer_;
		shader_program shader_;
		vertex_buffer buffer_;
		index_buffer ib;
		vertex_layout layout_;
		texture texture_;
		sampler_state sampler_;
		vector<vector<Chunk*>> chunks_grid;
		size_t indicies_count;
	};


}
