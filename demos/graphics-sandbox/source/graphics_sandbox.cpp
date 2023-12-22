#include "graphics_sandbox.h"

#include <core/core.h>
#include <graphics/api/renderer.h>
#include <graphics/api/camera.h>
#include <graphics/api/color_material.h>
#include <graphics/glcore/v2/gl_vertex_array.h>
#include <vector.hpp>
#include <matrix.hpp>
#include <imgui.h>

namespace sputnik::demos
{

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
    Renderer::SetCameraType(CameraType::Camera);
    auto camera_position = Camera::GetInstance()->GetCameraPosition();
    camera_position.y    = 0.0f;
    EditorCamera::GetInstance()->SetPosition({camera_position.x, camera_position.y, camera_position.z});

    sputnik::graphics::gl::VertexAttributeType type = sputnik::graphics::gl::VertexAttributeType::Float;
    ENGINE_INFO("Graphics Sandbox Demo Layer Attached: {}", sputnik::graphics::gl::attributeTypeToString(type));

    m_static_program = std::make_shared<OglShaderProgram>();
    m_static_program->addShaderStage("../../data/shaders/glsl/static.vert");
    m_static_program->addShaderStage("../../data/shaders/glsl/static.frag");
    m_static_program->configure();

    m_per_frame_data_buffer = std::make_shared<OglBuffer>(sizeof(PerFrameData));
    m_per_frame_data_buffer->bind(BufferBindTarget::UniformBuffer, 0);

    m_light_data_buffer = std::make_shared<OglBuffer>(sizeof(Light));
    m_light_data_buffer->bind(BufferBindTarget::UniformBuffer, 1);

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

    m_light.position = vec3(0.0f, 5.0f, 5.0f);
    m_light.ambient  = vec3(1.0f, 1.0f, 1.0f);
    m_light.diffuse  = vec3(1.0f, 1.0f, 1.0f);
    m_light.specular = vec3(1.0f, 1.0f, 1.0f);
}

void GraphicsSandboxDemoLayer::OnDetach() {}

void GraphicsSandboxDemoLayer::OnUpdate(const core::TimeStep& time_step)
{
    // const auto& camera               = EditorCamera::GetInstance();
    const auto& camera               = Camera::GetInstance();
    m_per_frame_data.projection      = camera->GetCameraPerspective();
    m_per_frame_data.view            = camera->GetCameraView();
    m_per_frame_data.camera_position = camera->GetCameraPosition();

    m_per_frame_data_buffer->setData(&m_per_frame_data, sizeof(PerFrameData));
    m_light_data_buffer->setData(&m_light, sizeof(Light));

    glEnable(GL_DEPTH_TEST);

    m_static_program->bind();

    mat4 model{};
    // model              = model.translate({0.0f, 0.0f, 0.0f});
    // model              = model.scale({0.15f});
    mat4 normal_matrix = model.inverted().transpose();
    m_static_program->setMat4("model", model);
    m_static_program->setMat4("normal_matrix", normal_matrix);

    m_static_program->setFloat3("material.ambient", material_emerald.ambient);
    m_static_program->setFloat3("material.diffuse", material_emerald.diffuse);
    m_static_program->setFloat3("material.specular", material_emerald.specular);
    m_static_program->setFloat("material.shininess", material_emerald.shininess);

    m_vertex_array->bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);

    model         = {};
    model         = model.translate(m_light.position);
    model         = model.scale({0.15f});
    normal_matrix = model.inverted().transpose();
    m_static_program->setMat4("model", model);
    m_static_program->setMat4("normal_matrix", normal_matrix);

    m_static_program->setFloat3("material.ambient", material_ruby.ambient);
    m_static_program->setFloat3("material.diffuse", material_ruby.diffuse);
    m_static_program->setFloat3("material.specular", material_ruby.specular);
    m_static_program->setFloat("material.shininess", material_ruby.shininess);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    m_vertex_array->unbind();

    m_static_program->unbind();

    glDisable(GL_DEPTH_TEST);
}

void GraphicsSandboxDemoLayer::OnEvent() {}

void GraphicsSandboxDemoLayer::OnUpdateUI(const core::TimeStep& time_step)
{
    if(ImGui::Begin("Lighting"))
    {
        ImGui::SliderFloat("Light X", &m_light.position.x, -50.0f, 50.0f);
        ImGui::SliderFloat("Light Y", &m_light.position.y, -50.0f, 50.0f);
        ImGui::SliderFloat("Light Z", &m_light.position.z, -50.0f, 50.0f);

        ImGui::ColorEdit3("Light ambient", &m_light.ambient.x, ImGuiColorEditFlags_Float);
        ImGui::ColorEdit3("Light diffuse", &m_light.diffuse.x, ImGuiColorEditFlags_Float);
        ImGui::ColorEdit3("Light specular", &m_light.specular.x, ImGuiColorEditFlags_Float);

        ImGui::SliderFloat("Light constant", &m_light.constant, 0.0f, 1.0f);
        ImGui::SliderFloat("Light linear", &m_light.linear, 0.0f, 1.0f);
        ImGui::SliderFloat("Light quadratic", &m_light.quadratic, 0.0f, 1.0f);
    }
    ImGui::End();
}

} // namespace sputnik::demos
