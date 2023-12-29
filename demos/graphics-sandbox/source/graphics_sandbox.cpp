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

#include <glad/glad.h>

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
    // Renderer::SetCameraType(CameraType::Camera);
    auto camera_position = Camera::GetInstance()->GetCameraPosition();
    camera_position.y    = 0.0f;
    sputnik::graphics::api::EditorCamera::GetInstance()->SetPosition(
        {camera_position.x, camera_position.y, camera_position.z});

    sputnik::graphics::gl::VertexAttributeType type = sputnik::graphics::gl::VertexAttributeType::Float;
    ENGINE_INFO("Graphics Sandbox Demo Layer Attached: {}", sputnik::graphics::gl::attributeTypeToString(type));

    m_static_program = std::make_shared<OglShaderProgram>();
    m_static_program->addShaderStage("../../data/shaders/glsl/blinn_phong.vert");
    m_static_program->addShaderStage("../../data/shaders/glsl/blinn_phong.frag");
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

    u32 white = 0xffffffff;
    u32 red   = 0xff0000ff;
    u32 green = 0xff00ff00;
    u32 blue  = 0xffff0000;
    // m_diff_texture = std::make_shared<OglTexture2D>(1, 1, TextureFormat::RGBA8);
    // m_diff_texture->setData(&green, sizeof(u32));
    m_diff_texture = std::make_shared<OglTexture2D>("../../data/assets/fabric_basecolor.jpg", false);
    m_spec_texture = std::make_shared<OglTexture2D>(1, 1, &white, TextureFormat::RGBA8);

    m_animated_model     = Model::LoadModel("../../data/assets/Woman.gltf");
    m_diff_texture_woman = std::make_shared<OglTexture2D>("../../data/assets/Woman.png", false);

    m_shader_program = std::make_shared<OglShaderProgram>();
    m_shader_program->addShaderStage("../../data/shaders/simple.vert");
    m_shader_program->addShaderStage("../../data/shaders/simple.frag");
    m_shader_program->configure();

    // glCreateVertexArrays(1, &m_grid_vao);
    m_grid_program = std::make_shared<OglShaderProgram>();
    m_grid_program->addShaderStage("../../data/shaders/glsl/grid.vert");
    m_grid_program->addShaderStage("../../data/shaders/glsl/grid.frag");
    m_grid_program->configure();
}

void GraphicsSandboxDemoLayer::OnDetach()
{
    // glDeleteVertexArrays(1, &m_grid_vao);
}

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

    // m_static_program->setFloat3("material.ambient", material_emerald.ambient);
    m_static_program->setFloat3("material.diffuse", material_white.diffuse);
    m_static_program->setFloat3("material.specular", material_white.specular);
    m_static_program->setFloat("material.shininess", material_white.shininess);

    m_diff_texture->bind(0);
    m_static_program->setInt("material.diffuse_texture", 0);
    m_spec_texture->bind(1);
    m_static_program->setInt("material.specular_texture", 1);

    m_vertex_array->bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);

    model         = {};
    model         = model.translate(m_light.position);
    model         = model.scale({0.15f});
    normal_matrix = model.inverted().transpose();
    m_static_program->setMat4("model", model);
    m_static_program->setMat4("normal_matrix", normal_matrix);

    // m_static_program->setFloat3("material.ambient", material_ruby.ambient);
    m_static_program->setFloat3("material.diffuse", material_ruby.diffuse);
    m_static_program->setFloat3("material.specular", material_ruby.specular);
    m_static_program->setFloat("material.shininess", material_ruby.shininess);
    m_diff_texture->bind(0);
    m_static_program->setInt("material.diffuse_texture", 0);
    m_spec_texture->bind(1);
    m_static_program->setInt("material.specular_texture", 1);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    m_vertex_array->unbind();

    m_static_program->unbind();

    // render woman
    {
        const auto& editor_camera = sputnik::graphics::api::EditorCamera::GetInstance();
        mat4        projection    = editor_camera->GetCameraPerspective();
        mat4        view          = editor_camera->GetCameraView();
        model                     = {};
        model                     = model.rotate({0.0, 1.0, 0.0}, -90.0f * kDegToRad);

        m_shader_program->bind();
        m_shader_program->setMat4("projection", projection);
        m_shader_program->setMat4("model", model);
        m_shader_program->setMat4("view", view);

        m_diff_texture_woman->bind(0);
        m_shader_program->setInt("diffuse", 0);

        m_shader_program->setFloat3("light", {0.0f, 5.0f, 5.0f});

        m_animated_model->Draw();

        m_shader_program->unbind();
    }
    // render woman ends

    // render grid
    {
        m_grid_vao.bind();
        m_grid_program->bind();
        glEnable(GL_BLEND);
        // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // black lines
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA); // This gives best results - white lines
        glDrawArraysInstancedBaseInstance(GL_TRIANGLES, 0, 6, 1, 0);
        glDisable(GL_BLEND);
        m_grid_program->unbind();
        m_grid_vao.unbind();
    }
    // render grid ends

    glDisable(GL_DEPTH_TEST);

    // sputnik::editor::Editor::getInstance()->OnUpdate(time_step);
    // if(ImGui::Begin("Lighting"))
    //{
    //    sputnik::editor::Editor::drawWidgetVec3("position", m_light.position, 90.0f);
    //    sputnik::editor::Editor::drawWidgetColor3("ambient", m_light.ambient, 90.0f);
    //    sputnik::editor::Editor::drawWidgetColor3("diffuse", m_light.diffuse, 90.0f);
    //    sputnik::editor::Editor::drawWidgetColor3("specular", m_light.specular, 90.0f);
    //    sputnik::editor::Editor::drawWidgetFloat("constant", m_light.constant, 90.0f);
    //    sputnik::editor::Editor::drawWidgetFloat("linear", m_light.linear, 90.0f);
    //    sputnik::editor::Editor::drawWidgetFloat("quadratic", m_light.quadratic, 90.0f);

    //    // ImGui::SliderFloat("Light X", &m_light.position.x, -50.0f, 50.0f);
    //    // ImGui::SliderFloat("Light Y", &m_light.position.y, -50.0f, 50.0f);
    //    // ImGui::SliderFloat("Light Z", &m_light.position.z, -50.0f, 50.0f);
    //    // ImGui::ColorEdit3("Light ambient", &m_light.ambient.x, ImGuiColorEditFlags_Float);
    //    // ImGui::ColorEdit3("Light diffuse", &m_light.diffuse.x, ImGuiColorEditFlags_Float);
    //    // ImGui::ColorEdit3("Light specular", &m_light.specular.x, ImGuiColorEditFlags_Float);
    //    // ImGui::SliderFloat("Light constant", &m_light.constant, 0.0f, 1.0f);
    //    // ImGui::SliderFloat("Light linear", &m_light.linear, 0.0f, 1.0f);
    //    // ImGui::SliderFloat("Light quadratic", &m_light.quadratic, 0.0f, 1.0f);
    //}
    // ImGui::End();
}

void GraphicsSandboxDemoLayer::OnEvent() {}

void GraphicsSandboxDemoLayer::OnUpdateUI(const core::TimeStep& time_step)
{
    // if(ImGui::Begin("Lighting"))
    //{
    //     ImGui::SliderFloat("Light X", &m_light.position.x, -50.0f, 50.0f);
    //     ImGui::SliderFloat("Light Y", &m_light.position.y, -50.0f, 50.0f);
    //     ImGui::SliderFloat("Light Z", &m_light.position.z, -50.0f, 50.0f);

    //    ImGui::ColorEdit3("Light ambient", &m_light.ambient.x, ImGuiColorEditFlags_Float);
    //    ImGui::ColorEdit3("Light diffuse", &m_light.diffuse.x, ImGuiColorEditFlags_Float);
    //    ImGui::ColorEdit3("Light specular", &m_light.specular.x, ImGuiColorEditFlags_Float);

    //    ImGui::SliderFloat("Light constant", &m_light.constant, 0.0f, 1.0f);
    //    ImGui::SliderFloat("Light linear", &m_light.linear, 0.0f, 1.0f);
    //    ImGui::SliderFloat("Light quadratic", &m_light.quadratic, 0.0f, 1.0f);
    //}
    // ImGui::End();

    // if(ImGui::Begin("Lighting"))
    //{
    //    sputnik::editor::Editor::drawWidgetVec3("position", m_light.position, 90.0f);
    //    sputnik::editor::Editor::drawWidgetColor3("ambient", m_light.ambient, 90.0f);
    //    sputnik::editor::Editor::drawWidgetColor3("diffuse", m_light.diffuse, 90.0f);
    //    sputnik::editor::Editor::drawWidgetColor3("specular", m_light.specular, 90.0f);
    //    sputnik::editor::Editor::drawWidgetFloat("constant", m_light.constant, 90.0f);
    //    sputnik::editor::Editor::drawWidgetFloat("linear", m_light.linear, 90.0f);
    //    sputnik::editor::Editor::drawWidgetFloat("quadratic", m_light.quadratic, 90.0f);

    //    // ImGui::SliderFloat("Light X", &m_light.position.x, -50.0f, 50.0f);
    //    // ImGui::SliderFloat("Light Y", &m_light.position.y, -50.0f, 50.0f);
    //    // ImGui::SliderFloat("Light Z", &m_light.position.z, -50.0f, 50.0f);
    //    // ImGui::ColorEdit3("Light ambient", &m_light.ambient.x, ImGuiColorEditFlags_Float);
    //    // ImGui::ColorEdit3("Light diffuse", &m_light.diffuse.x, ImGuiColorEditFlags_Float);
    //    // ImGui::ColorEdit3("Light specular", &m_light.specular.x, ImGuiColorEditFlags_Float);
    //    // ImGui::SliderFloat("Light constant", &m_light.constant, 0.0f, 1.0f);
    //    // ImGui::SliderFloat("Light linear", &m_light.linear, 0.0f, 1.0f);
    //    // ImGui::SliderFloat("Light quadratic", &m_light.quadratic, 0.0f, 1.0f);
    //}
    // ImGui::End();
}

} // namespace sputnik::demos
