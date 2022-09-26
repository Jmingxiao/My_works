/*
 * Copyright 2011-2022 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */

#include <bx/uint32_t.h>
#include "common.h"
#include "bgfx_utils.h"
#include "imgui/imgui.h"
#include "camera.h"
#include <iostream>
#include "Camera.hpp"


static const char* options[]{ "Blinn Phong","PBR" };

static int selectedOption = 0;

static const float quadVertices[] = {
	-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
	-1.0f,	0.0f, 0.0f, 0.0f, 0.0f,
	-0.0f,  0.0f, 0.0f, 1.0f, 0.0f,
	-0.0f,  0.0f, 0.0f, 1.0f, 0.0f,
	-0.0f,  1.0f, 0.0f, 1.0f, 1.0f,
	-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
};




static const float skyboxVertices[] = {
	// positions          
-1.0f,  1.0f,  1.0f,
 1.0f,  1.0f,  1.0f,
-1.0f, -1.0f,  1.0f,
 1.0f, -1.0f,  1.0f,
-1.0f,  1.0f, -1.0f,
 1.0f,  1.0f, -1.0f,
-1.0f, -1.0f, -1.0f,
 1.0f, -1.0f, -1.0f
};


static const uint16_t trianglelist[] =
{
	0, 1, 2, // 0
	1, 3, 2,
	4, 6, 5, // 2
	5, 6, 7,
	0, 2, 4, // 4
	4, 2, 6,
	1, 5, 3, // 6
	5, 7, 3,
	0, 4, 1, // 8
	4, 5, 1,
	2, 3, 6, // 10
	6, 3, 7,
};


namespace
{

class EStarHomework : public entry::AppI
{
public:
	EStarHomework(const char* _name, const char* _description, const char* _url)
		: entry::AppI(_name, _description, _url)
	{
		m_width = 0;
		m_height = 0;
		m_debug = BGFX_DEBUG_NONE;
		m_reset = BGFX_RESET_NONE;
	}

	void init(int32_t _argc, const char* const* _argv, uint32_t _width, uint32_t _height) override
	{
		Args args(_argc, _argv);

		m_width  = _width;
		m_height = _height;
		m_debug  = BGFX_DEBUG_TEXT;
		m_reset  = BGFX_RESET_VSYNC;

		bgfx::Init init;
		init.type     = bgfx::RendererType::OpenGL;
		init.vendorId = args.m_pciId;
		init.resolution.width  = m_width;
		init.resolution.height = m_height;
		init.resolution.reset  = m_reset;
		bgfx::init(init);


		// Set view 0 clear state.
		bgfx::setViewClear(0
			, BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
			, 0x303030ff
			, 1.0f
			, 0
			);

		m_stone = meshLoad("../resource/pbr_stone/pbr_stone_mes.bin");
		m_platform = meshLoad("../resource/basic_meshes/platform.bin");


		m_pbrshader = loadProgram("vs_pbr", "fs_pbr");
		m_blinnshader = loadProgram("vs_blinnphong", "fs_blinnphong");
		texColor = bgfx::createUniform("texColor", bgfx::UniformType::Sampler);
		texNormal = bgfx::createUniform("texNormal", bgfx::UniformType::Sampler);	
		texaorm = bgfx::createUniform("texaorm", bgfx::UniformType::Sampler);
		lut = bgfx::createUniform("Lut", bgfx::UniformType::Sampler);
		light = bgfx::createUniform("light", bgfx::UniformType::Vec4);

		m_texColor = loadTexture("../resource/pbr_stone/pbr_stone_base_color.dds");
		m_texNormal = loadTexture("../resource/pbr_stone/pbr_stone_normal.dds");
		m_texaorm = loadTexture("../resource/pbr_stone/pbr_stone_aorm.dds");
		m_texlut = loadTexture("../resource/textures/lut.dds");

		//skybox
		m_layout.begin().add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float).end();
		m_vb = bgfx::createVertexBuffer(bgfx::makeRef(skyboxVertices, sizeof(skyboxVertices)), m_layout);
		m_ib = bgfx::createIndexBuffer(
			// Static data can be passed with bgfx::makeRef
			bgfx::makeRef(trianglelist, sizeof(trianglelist))
		);
		m_skyboxshader = loadProgram("vs_skybox", "fs_skybox");
		m_texskybox = loadTexture("../resource/skybox/kyoto_lod.dds");
		m_texskyboxirr = loadTexture("../resource/skybox/kyoto_irr.dds");
		skybox = bgfx::createUniform("skybox", bgfx::UniformType::Sampler);
		skyboxirr = bgfx::createUniform("skyboxIrr", bgfx::UniformType::Sampler);

		cam.set_position({ 0.0f, 1.0f, -5.5f });


		imguiCreate();
	}

	virtual int shutdown() override
	{
		imguiDestroy();

		meshUnload(m_stone);
		meshUnload(m_platform);
		bgfx::destroy(m_pbrshader);
		bgfx::destroy(m_blinnshader);
		bgfx::destroy(lut);
		bgfx::destroy(texaorm);
		bgfx::destroy(texColor);
		bgfx::destroy(texNormal);
		bgfx::destroy(m_texColor);
		bgfx::destroy(m_texNormal);
		bgfx::destroy(m_texaorm);
		bgfx::destroy(m_texlut);


		bgfx::destroy(m_ib);
		bgfx::destroy(m_vb);
		bgfx::destroy(skybox);
		bgfx::destroy(m_skyboxshader);
		bgfx::destroy(m_texskybox);
		bgfx::destroy(m_texskyboxirr);
		bgfx::destroy(skyboxirr);


		bgfx::shutdown();

		return 0;
	}

	bool update() override
	{
		if (!entry::processEvents(m_width, m_height, m_debug, m_reset, &m_mouseState) )
		{
			double currentframe = static_cast<float>(ImGui::GetTime());
			deltaTime = currentframe - lastFrame;
			lastFrame = currentframe;

			//std::cout << "\r" << deltaTime << "  " << std::flush;

			imguiBeginFrame(m_mouseState.m_mx
				,  m_mouseState.m_my
				, (m_mouseState.m_buttons[entry::MouseButton::Left  ] ? IMGUI_MBUT_LEFT   : 0)
				| (m_mouseState.m_buttons[entry::MouseButton::Right ] ? IMGUI_MBUT_RIGHT  : 0)
				| (m_mouseState.m_buttons[entry::MouseButton::Middle] ? IMGUI_MBUT_MIDDLE : 0)
				,  m_mouseState.m_mz
				, uint16_t(m_width)
				, uint16_t(m_height)
				);

			showExampleDialog(this);

			ImGui::SetNextWindowPos(
				ImVec2(m_width - m_width / 5.0f - 10.0f, 10.0f)
				, ImGuiCond_FirstUseEver
			);
			ImGui::SetNextWindowSize(
				ImVec2(m_width / 5.0f, m_height / 3.0f)
				, ImGuiCond_FirstUseEver
			);
			ImGui::Begin("Settings"
				, NULL
				, 0
			);

			ImGui::Combo("Options", &selectedOption, options, IM_ARRAYSIZE(options));
			
			ImGui::Checkbox("Light", &lighton);
			if (lighton) {
				ImGui::SliderFloat4("LightPos", lightparam, -1.0f, 1.0f);
			}

			ImGui::End();


			imguiEndFrame();

			controller.Update(m_mouseState, deltaTime);
			

			//model matrix
			float p_scale[16];
			float p_pos[16];
			float p_mtx[16];
			bx::mtxTranslate(p_pos, 0.0f, -1.0f, 0.0f);
			bx::mtxScale(p_scale, 5.0f, 1.0f, 5.0f);
			bx::mtxMul(p_mtx, p_scale, p_pos);

			float s_mtx[16];
			bx::mtxScale(s_mtx, 0.5f);
			float mtx[16];
			bx::mtxScale(mtx, 1.0f);

			bgfx::ViewId sceneviewid = 0;
			bgfx::ViewId skyboxviewid = 1;
			

			//scene view setup
			bgfx::setViewRect(sceneviewid, 0, 0, uint16_t(m_width), uint16_t(m_height) );
			float view[16];
			cam.getviewmtx(view);

			float proj[16];
			bx::mtxProj(proj, 60.0f, float(m_width) / float(m_height), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
			bgfx::setViewTransform(sceneviewid,view, proj);

			bgfx::touch(0);

			{
				bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_DEPTH_TEST_LESS);
				
				lightparam[3] = lighton ? lightparam[3] : 0.0f;
				bgfx::setUniform(light, lightparam);

				switch (selectedOption)
				{
				case 0:
					bgfx::setTexture(0, texColor, m_texColor);
					bgfx::setTexture(1, texNormal, m_texNormal);
					meshSubmit(m_stone, sceneviewid, m_blinnshader, mtx);
					break;
				case 1:
					
					bgfx::setTexture(0, texColor, m_texColor);
					bgfx::setTexture(1, texNormal, m_texNormal);
					bgfx::setTexture(2, texaorm, m_texaorm);
					bgfx::setTexture(3, skyboxirr, m_texskyboxirr);
					bgfx::setTexture(4, skybox, m_texskybox);
					bgfx::setTexture(5, lut, m_texlut);

					meshSubmit(m_stone, sceneviewid, m_pbrshader, mtx);

					break;
				default:
					break;
				}
			}

			
			//skybox
			{
				bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_DEPTH_TEST_LEQUAL);
				for (int i = 0; i < 16; i++) {
					if ((i + 1 % 4 == 0 && i != 0) || i >= 12) {
						view[i] = 0.0f;
					}
				}
				bgfx::setViewRect(skyboxviewid, 0, 0, uint16_t(m_width), uint16_t(m_height));
				bgfx::setViewTransform(skyboxviewid, view, proj);
				bgfx::setTransform(mtx);
				bgfx::setVertexBuffer(0, m_vb);
				bgfx::setIndexBuffer(m_ib);
				bgfx::setTexture(0, skybox, m_texskybox);
				bgfx::submit(skyboxviewid, m_skyboxshader);
			}
			


			bgfx::frame();

			return true;
		}

		return false;
	}

	entry::MouseState m_mouseState;

	bool lighton{ true };
	float lightparam[4]{ 0.5f,1.0f,0.0f,0.8f };
	uint16_t shadowmapsize{ 512 };

	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_debug;
	uint32_t m_reset;
	
	//
	Mesh* m_stone;
	Mesh* m_platform;
	bgfx::UniformHandle texColor{};
	bgfx::UniformHandle texNormal{};
	bgfx::UniformHandle texaorm{};
	bgfx::TextureHandle m_texColor{};
	bgfx::TextureHandle m_texNormal{};
	bgfx::TextureHandle m_texaorm{};
	bgfx::ProgramHandle m_pbrshader{};
	bgfx::ProgramHandle m_blinnshader{};
	bgfx::UniformHandle skyboxirr{};
	bgfx::TextureHandle m_texskyboxirr{};
	bgfx::UniformHandle lut{};
	bgfx::UniformHandle light{};
	bgfx::TextureHandle m_texlut{};



	//skybox
	bgfx::VertexBufferHandle m_vb;
	bgfx::VertexLayout m_layout;
	bgfx::IndexBufferHandle m_ib;
	bgfx::UniformHandle skybox{};
	bgfx::ProgramHandle m_skyboxshader{};
	bgfx::TextureHandle m_texskybox{};


	float deltaTime{};
	float lastFrame{};

	Camera cam{};
	Controller controller{cam};

	
};

} // namespace

int _main_(int _argc, char** _argv)
{
	EStarHomework app("e-star-homework", "", "");
	
	
	return entry::runApp(&app, _argc, _argv);
}

