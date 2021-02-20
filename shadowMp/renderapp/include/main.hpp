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
#include"Camera.h"
#include "Cube.h"
#include "Light.h"
#include"BackDrop.h"

namespace avocado {
   struct renderapp final : application {
      renderapp();

      virtual bool on_init();
      virtual void on_exit();
      virtual bool on_tick(const time &deltatime);
      virtual void on_draw();

      void create_famebuffer();
      renderer renderer_;
      Cube cube_;
      Cube cube_a;
      Cube cube_b;
      Light light_;
      depthquad shadowmap;
      BackDrop back_;
      shader_program shader_;
      shader_program depthShader_;
      shader_program debugshader_;
      unsigned int depthmap;
      unsigned int fb_;
      
      
      camera camera_;
      frustum frustum_;
      controller controller_;
      const unsigned int SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;
   };
} // !avocado
