// main.cc
#define SCR_WIDTH 1920
#define SCR_HEIGHT 1080
#include "main.hpp"

void log();
namespace avocado {
   // note: application create implementation
   application *application::create(settings &settings)
   {
      settings.title_      = "renderapp";
      settings.width_      = SCR_WIDTH;
      settings.height_     = SCR_HEIGHT;
      settings.center_     = true;

      return new renderapp;
   }

   // note: renderapp class
   renderapp::renderapp():controller_(camera_)
   {
   }

   bool renderapp::on_init()
   {
      if (!renderer_.initialize()) {
         return false;
      }

      { // note: load vertex and fragment shaders and create shader program
          string vertex_source;
          if (!file_system::read_file_content("assets/SceneVS.shader", vertex_source)) {
              return false;
          }
          string fragment_source;
          if (!file_system::read_file_content("assets/SceneFS.shader", fragment_source)) {
              return false;
          }
          if (!shader_.create(vertex_source.c_str(),fragment_source.c_str()))
          {
              return false;
          }
      }

      {
          string vertex_source;
          if (!file_system::read_file_content("assets/DepthVS.shader", vertex_source)) {
             return false;
          }
          string fragment_source;
          if (!file_system::read_file_content("assets/DepthFS.shader", fragment_source)) {
             return false;
          }
          if (!depthShader_.create(vertex_source.c_str(), fragment_source.c_str()))
          {
             return false;
          }
      }

      {
          string vertex_source;
          if (!file_system::read_file_content("assets/debugVS.shader", vertex_source)) {
              return false;
          }
          string fragment_source;
          if (!file_system::read_file_content("assets/debugFS.shader", fragment_source)) {
              return false;
          }
          if (!debugshader_.create(vertex_source.c_str(), fragment_source.c_str()))
          {
              return false;
          }
      }

     
      if (!cube_.Init(glm::translate(mat4(1.0f),vec3(3.0f, 0.0f, -3.0)))) {
          return on_error("Cube can not initialize");
      }
      if (!cube_a.Init( glm::translate(mat4(1.0f), vec3(1.0f, 1.5f, -0.5)))) {
          return on_error("Cube1 can not initialize");
      }
      if (!cube_b.Init(glm::translate(mat4(1.0f), vec3(-1.0f, 0.0f, 0.5)))) {
          return on_error("Cube1 can not initialize");
      }
      if (!light_.Init()) {
          return on_error("light can not initialize");
      }
      if (!back_.Init()) {
          return on_error("backdrop can not initialize");
      }
      shadowmap.init();
      {
          create_famebuffer();
      }
     
      glm::mat4 projection = glm::perspective(3.141592f * 0.25f, 16.0f / 9.0f, 1.0f, 100.0f);
      glm::vec3 view = glm::vec3(0.0f, 2.0f, 3.0f);
      camera_.set_projection(projection);
      camera_.set_position(view);
      //camera_.view_ = light_.lightView;
      cube_.camera_ = std::move(camera_);
      cube_a.camera_ = std::move(camera_);
      cube_b.camera_ = std::move(camera_);
      back_.camera_ = std::move(camera_);
      
      renderer_.set_shader_program(shader_);
      renderer_.set_shader_uniform(shader_, UNIFORM_TYPE_INT, "u_texture", 0, nullptr);
      renderer_.set_shader_uniform(shader_, UNIFORM_TYPE_INT, "shadowMap", 1, nullptr);
      
      return true;


   }

   void renderapp::on_exit()
   {
       cube_.Delete();
       cube_a.Delete();
       cube_b.Delete();
       back_.Delete();
       shader_.destroy();
       shadowmap.destroy();
       depthShader_.destroy();
       debugshader_.destroy();

       
   }

   bool renderapp::on_tick(const time &deltatime)
   {
       if (keyboard_.key_pressed(keyboard::key::escape)) {
           return false;
       }
       controller_.update(keyboard_, mouse_, deltatime);
       cube_.camera_ = std::move(camera_);
       cube_a.camera_ = std::move(camera_);
       cube_b.camera_ = std::move(camera_);
       back_.camera_ = std::move(camera_);
       return true;
   }

   void renderapp::on_draw()
   {
      renderer_.clear(0.5f, 0.3f, 0.4f, 1.0f);
      renderer_.set_shader_program(depthShader_);
      renderer_.set_shader_uniform(depthShader_, UNIFORM_TYPE_MATRIX, "lightSpaceMatrix", 1, glm::value_ptr(light_.lightSpaceMatrix));
      renderer_.set_viewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
      glBindFramebuffer(GL_FRAMEBUFFER, fb_);
      renderer_.clear(0.5f, 0.3f, 0.4f, 1.0f);
        
          renderer_.set_depth_state(true, true);
          renderer_.set_texture(back_.texture_);
          renderer_.set_sampler_state(back_.ss_);
          back_.DrawDepth(depthShader_);
          renderer_.set_texture(cube_.texture_);
          renderer_.set_sampler_state(cube_.ss_);
          cube_.DrawDepth(depthShader_);
          cube_a.DrawDepth(depthShader_);
          cube_b.DrawDepth(depthShader_);

      renderer_.reset_framebuffer();
      renderer_.set_viewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

  
      renderer_.set_shader_program(debugshader_);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, depthmap);
      renderer_.set_shader_uniform(debugshader_, UNIFORM_TYPE_INT, "DepthMap", 0, nullptr);
      shadowmap.renderquad();



      renderer_.set_depth_state(true, true);
      renderer_.set_shader_program(shader_);
      renderer_.set_shader_uniform(shader_, UNIFORM_TYPE_MATRIX, "lightSpaceMatrix", 1, glm::value_ptr(light_.lightSpaceMatrix));
      renderer_.set_shader_uniform(shader_, UNIFORM_TYPE_VEC3, "lightDirection", 1, glm::value_ptr(light_.Dir));
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, depthmap);
      renderer_.set_texture(back_.texture_);
      renderer_.set_sampler_state(back_.ss_);
      back_.OnDraw(shader_);
      renderer_.set_texture(cube_.texture_);
      renderer_.set_sampler_state(cube_.ss_);
      cube_.OnDraw(shader_);
      cube_a.OnDraw(shader_);
      cube_b.OnDraw(shader_);


   }
   void renderapp::create_famebuffer()
   {
       glGenFramebuffers(1, &fb_);
       // create depth texture
       glGenTextures(1, &depthmap);
       glBindTexture(GL_TEXTURE_2D, depthmap);
       glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
       float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
       glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
       // attach depth texture as FBO's depth buffer
       glBindFramebuffer(GL_FRAMEBUFFER, fb_);
       glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthmap, 0);
       glDrawBuffer(GL_NONE);
       glReadBuffer(GL_NONE);
       glBindFramebuffer(GL_FRAMEBUFFER, 0);
   }
} // !avocado
