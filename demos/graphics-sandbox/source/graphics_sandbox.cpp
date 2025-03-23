#include "graphics_sandbox.h"

#include <core/core.h>
#include <editor/editor_camera.h>
#include <graphics/api/camera.h>
#include <graphics/api/color_material.h>
#include <graphics/glcore/gl_vertex_array.h>
#include <vector.hpp>
#include <matrix.hpp>
#include <imgui.h>
#include <editor/editor.hpp>
#include <core/systems/render_system.h>

#include <glad/glad.h>

namespace sputnik::demos
{

using namespace sputnik::core::systems;

struct VertexData
{
    alignas(16) vec3 position;
    alignas(16) vec3 normal;
    alignas(8) vec2 uv;
};

std::vector<VertexData> cube_verts = {
    {.position = {-0.5f, -0.5f, -0.5f}, .normal = {0.0f, 0.0f, -1.0f}, .uv = {0.0f, 0.0f}},
    {.position = {0.5f, -0.5f, -0.5f}, .normal = {0.0f, 0.0f, -1.0f}, .uv = {1.0f, 0.0f}},
    {.position = {0.5f, 0.5f, -0.5f}, .normal = {0.0f, 0.0f, -1.0f}, .uv = {1.0f, 1.0f}},
    {.position = {0.5f, 0.5f, -0.5f}, .normal = {0.0f, 0.0f, -1.0f}, .uv = {1.0f, 1.0f}},
    {.position = {-0.5f, 0.5f, -0.5f}, .normal = {0.0f, 0.0f, -1.0f}, .uv = {0.0f, 1.0f}},
    {.position = {-0.5f, -0.5f, -0.5f}, .normal = {0.0f, 0.0f, -1.0f}, .uv = {0.0f, 0.0f}},

    {.position = {-0.5f, -0.5f, 0.5f}, .normal = {0.0f, 0.0f, 1.0f}, .uv = {0.0f, 0.0f}},
    {.position = {0.5f, -0.5f, 0.5f}, .normal = {0.0f, 0.0f, 1.0f}, .uv = {1.0f, 0.0f}},
    {.position = {0.5f, 0.5f, 0.5f}, .normal = {0.0f, 0.0f, 1.0f}, .uv = {1.0f, 1.0f}},
    {.position = {0.5f, 0.5f, 0.5f}, .normal = {0.0f, 0.0f, 1.0f}, .uv = {1.0f, 1.0f}},
    {.position = {-0.5f, 0.5f, 0.5f}, .normal = {0.0f, 0.0f, 1.0f}, .uv = {0.0f, 1.0f}},
    {.position = {-0.5f, -0.5f, 0.5f}, .normal = {0.0f, 0.0f, 1.0f}, .uv = {0.0f, 0.0f}},

    {.position = {-0.5f, 0.5f, 0.5f}, .normal = {-1.0f, 0.0f, 0.0f}, .uv = {1.0f, 0.0f}},
    {.position = {-0.5f, 0.5f, -0.5f}, .normal = {-1.0f, 0.0f, 0.0f}, .uv = {1.0f, 1.0f}},
    {.position = {-0.5f, -0.5f, -0.5f}, .normal = {-1.0f, 0.0f, 0.0f}, .uv = {0.0f, 1.0f}},
    {.position = {-0.5f, -0.5f, -0.5f}, .normal = {-1.0f, 0.0f, 0.0f}, .uv = {0.0f, 1.0f}},
    {.position = {-0.5f, -0.5f, 0.5f}, .normal = {-1.0f, 0.0f, 0.0f}, .uv = {0.0f, 0.0f}},
    {.position = {-0.5f, 0.5f, 0.5f}, .normal = {-1.0f, 0.0f, 0.0f}, .uv = {1.0f, 0.0f}},

    {.position = {0.5f, 0.5f, 0.5f}, .normal = {1.0f, 0.0f, 0.0f}, .uv = {1.0f, 0.0f}},
    {.position = {0.5f, 0.5f, -0.5f}, .normal = {1.0f, 0.0f, 0.0f}, .uv = {1.0f, 1.0f}},
    {.position = {0.5f, -0.5f, -0.5f}, .normal = {1.0f, 0.0f, 0.0f}, .uv = {0.0f, 1.0f}},
    {.position = {0.5f, -0.5f, -0.5f}, .normal = {1.0f, 0.0f, 0.0f}, .uv = {0.0f, 1.0f}},
    {.position = {0.5f, -0.5f, 0.5f}, .normal = {1.0f, 0.0f, 0.0f}, .uv = {0.0f, 0.0f}},
    {.position = {0.5f, 0.5f, 0.5f}, .normal = {1.0f, 0.0f, 0.0f}, .uv = {1.0f, 0.0f}},

    {.position = {-0.5f, -0.5f, -0.5f}, .normal = {0.0f, -1.0f, 0.0f}, .uv = {0.0f, 1.0f}},
    {.position = {0.5f, -0.5f, -0.5f}, .normal = {0.0f, -1.0f, 0.0f}, .uv = {1.0f, 1.0f}},
    {.position = {0.5f, -0.5f, 0.5f}, .normal = {0.0f, -1.0f, 0.0f}, .uv = {1.0f, 0.0f}},
    {.position = {0.5f, -0.5f, 0.5f}, .normal = {0.0f, -1.0f, 0.0f}, .uv = {1.0f, 0.0f}},
    {.position = {-0.5f, -0.5f, 0.5f}, .normal = {0.0f, -1.0f, 0.0f}, .uv = {0.0f, 0.0f}},
    {.position = {-0.5f, -0.5f, -0.5f}, .normal = {0.0f, -1.0f, 0.0f}, .uv = {0.0f, 1.0f}},

    {.position = {-0.5f, 0.5f, -0.5f}, .normal = {0.0f, 1.0f, 0.0f}, .uv = {0.0f, 1.0f}},
    {.position = {0.5f, 0.5f, -0.5f}, .normal = {0.0f, 1.0f, 0.0f}, .uv = {1.0f, 1.0f}},
    {.position = {0.5f, 0.5f, 0.5f}, .normal = {0.0f, 1.0f, 0.0f}, .uv = {1.0f, 0.0f}},
    {.position = {0.5f, 0.5f, 0.5f}, .normal = {0.0f, 1.0f, 0.0f}, .uv = {1.0f, 0.0f}},
    {.position = {-0.5f, 0.5f, 0.5f}, .normal = {0.0f, 1.0f, 0.0f}, .uv = {0.0f, 0.0f}},
    {.position = {-0.5f, 0.5f, -0.5f}, .normal = {0.0f, 1.0f, 0.0f}, .uv = {0.0f, 1.0f}}};

// clang-format off
const float kCubeVertices[] = {
			-0.5f, -0.5f, -0.5f,		0.0f,  0.0f, -1.0f,			0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,		0.0f,  0.0f, -1.0f,			1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f,			1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f,			1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f,			0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,		0.0f,  0.0f, -1.0f,			0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,		0.0f,  0.0f, 1.0f,			0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,		0.0f,  0.0f, 1.0f,			1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,		0.0f,  0.0f, 1.0f,			1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,		0.0f,  0.0f, 1.0f,			1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,		0.0f,  0.0f, 1.0f,			0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,		0.0f,  0.0f, 1.0f,			0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,		-1.0f,  0.0f,  0.0f,		1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,		-1.0f,  0.0f,  0.0f,		1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,		-1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,		-1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,		-1.0f,  0.0f,  0.0f,		0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,		-1.0f,  0.0f,  0.0f,		1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,		1.0f,  0.0f,  0.0f,         1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,		1.0f,  0.0f,  0.0f,         1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,		1.0f,  0.0f,  0.0f,         0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,		1.0f,  0.0f,  0.0f,         0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,		1.0f,  0.0f,  0.0f,         0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,		1.0f,  0.0f,  0.0f,         1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,		0.0f, -1.0f,  0.0f,			0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,		0.0f, -1.0f,  0.0f,			1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,		0.0f, -1.0f,  0.0f,			1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,		0.0f, -1.0f,  0.0f,			1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,		0.0f, -1.0f,  0.0f,			0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,		0.0f, -1.0f,  0.0f,			0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,		0.0f,  1.0f,  0.0f,			0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,		0.0f,  1.0f,  0.0f,			1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,			1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,			1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,			0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,		0.0f,  1.0f,  0.0f,			0.0f, 1.0f
		};
// clang-format on

GraphicsSandboxDemoLayer::GraphicsSandboxDemoLayer(const std::string& name) : core::Layer{name} {}

GraphicsSandboxDemoLayer::~GraphicsSandboxDemoLayer() {}

void GraphicsSandboxDemoLayer::OnAttach()
{
    // Renderer::SetCameraType(CameraType::Camera);
    auto camera_position = Camera::GetInstance()->GetCameraPosition();
    camera_position.y    = 0.0f;
    sputnik::graphics::api::EditorCamera::GetInstance()->SetPosition(
        {camera_position.x, camera_position.y, camera_position.z});

    sputnik::graphics::gl::VertexAttributeType type = sputnik::graphics::gl::VertexAttributeType::Float;
    ENGINE_INFO("Graphics Sandbox Demo Layer Attached: {}", sputnik::graphics::gl::attributeTypeToString(type));

    m_vertex_buffer = std::make_shared<OglBuffer>((void*)kCubeVertices, sizeof(kCubeVertices));

    // Ideally all this must be read from the shader maybe?
    // Be more dynamic and less hardcoded
    // Maybe this is handled when creating pipeline infrastructure
    std::initializer_list<VertexInputAttributeSpecification> vertex_input_attributes{
        {.name = "position", .location = 0, .type = VertexAttributeType::Float3, .normalized = false},
        {.name = "normal", .location = 1, .type = VertexAttributeType::Float3, .normalized = false},
        {.name = "uv", .location = 2, .type = VertexAttributeType::Float2, .normalized = false},
    };

    VertexInputBindingSpecification vertex_input_binding{0, 8 * sizeof(float)};

    m_vertex_array = std::make_shared<OglVertexArray>();
    m_vertex_array->addVertexBuffer(*m_vertex_buffer.get(), vertex_input_binding, vertex_input_attributes);

    auto  render_system = RenderSystem::getInstance();
    auto& light         = render_system->getLight();
    light.position      = vec3(0.0f, 5.0f, 5.0f);
    light.ambient       = vec3(1.0f, 1.0f, 1.0f);
    light.diffuse       = vec3(1.0f, 1.0f, 1.0f);
    light.specular      = vec3(1.0f, 1.0f, 1.0f);

    m_cloth_diff_texture = std::make_shared<OglTexture2D>("../../data/assets/fabric_basecolor.jpg", false);

    m_animated_model     = Model::LoadModel("../../data/assets/Woman.gltf");
    //m_animated_model     = Model::LoadModel("../../data/assets/suzanne_blender_monkey.glb");
    m_diff_texture_woman = std::make_shared<OglTexture2D>("../../data/assets/Woman.png", false);

    // binding point of VertexData SSBO in blinn phong pvp program is 0
    m_pvp_vertex_buffer = std::make_shared<OglBuffer>((void*)cube_verts.data(), sizeof(VertexData) * cube_verts.size());
    m_pvp_vertex_buffer->bind(BufferBindTarget::ShaderStorageBuffer, 0);
}

void GraphicsSandboxDemoLayer::OnDetach()
{
    m_pvp_vertex_buffer->unbind();
}

void GraphicsSandboxDemoLayer::OnUpdate(const core::TimeStep& time_step)
{

    RenderSystem* render_system = RenderSystem::getInstance();

    // glEnable(GL_DEPTH_TEST);

    // render using vao shader
    {
        m_vertex_array->bind();

        // render cube
        {
            mat4 model{};
            model = model.translate({3.0, 3.0, -5.0});
            model = model.scale({2.0f});
            Material material_cloth;
            material_cloth.diff_texture = m_cloth_diff_texture;
            render_system->drawTriangles(36, material_cloth, model);
        }

        // render light box
        {
            mat4 model = {};
            model      = model.translate(render_system->getLight().position);
            model      = model.scale({0.15f});
            render_system->drawTriangles(36, material_ruby, model);
        }

        m_vertex_array->unbind();
    }

    // render using pvp shader
    {
        // draw#1
        mat4     model{};
        Material cloth     = {};
        cloth.shader_name  = "blinn_phong_pvp";
        cloth.diff_texture = m_cloth_diff_texture;
        render_system->drawTriangles(36, cloth, model);
        // draw#1 ends

        // draw#2
        model                = {};
        model                = model.translate({5.0f, 0.0f, 0.0f});
        model                = model.rotate({0.0f, 1.0f, 0.0f}, 45.0f * kDegToRad);
        model                = model.scale({0.5f});
        Material material    = material_white;
        material.shader_name = "blinn_phong_pvp";
        render_system->drawTriangles(36, material, model);
        // draw#2 ends
    }
    // render using pvp ends

    // render woman
    {
        mat4 model            = {};
        model                 = model.rotate({0.0, 1.0, 0.0}, -90.0f * kDegToRad);
        Material material     = {};
        material.diff_texture = m_diff_texture_woman;
        material.shader_name  = "blinn_phong";
        m_animated_model->draw(material, model);
    }
    // render woman ends

    // glDisable(GL_DEPTH_TEST);
}

void GraphicsSandboxDemoLayer::OnEvent() {}

void GraphicsSandboxDemoLayer::OnUpdateUI(const core::TimeStep& time_step) {}

} // namespace sputnik::demos
