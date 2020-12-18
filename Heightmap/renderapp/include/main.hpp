// main.hpp

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

#include <camera.hpp>
#include<vector>
#include"HeightMap.h"



using std::vector;

namespace avocado {

   struct renderapp final : application {
      renderapp();

      virtual bool on_init();
      virtual void on_exit();
      virtual bool on_tick(const time &deltatime);
      virtual void on_draw();

      renderer renderer_;

      camera camera_;
      frustum frustum_;
      controller controller_;

      HeightMap heightmap;

   };
} // !avocado
