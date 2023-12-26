#include "pch.h"
#include "atmospheric_scattering.h"
#include "graphics/api/renderer.h"

#include <vector3.h>

#include <glad/glad.h>

namespace sputnik::graphics::api
{

AtmosphericScatteringLayer::AtmosphericScatteringLayer() : Layer("Atmospheric Scattering") {}

AtmosphericScatteringLayer::~AtmosphericScatteringLayer() {}

void AtmosphericScatteringLayer::OnAttach()
{
    int a = sizeof(ramanujan::Vector3);
    int b = sizeof(ramanujan::IVector4);

    m_animated_model = Model::LoadModel("../../data/assets/Woman.gltf");
    m_diff_texture   = std::make_shared<OglTexture2D>("../../data/assets/Woman.png", false);

    m_shader_program = std::make_shared<OglShaderProgram>();
    m_shader_program->addShaderStage("../../data/shaders/simple.vert");
    m_shader_program->addShaderStage("../../data/shaders/simple.frag");
    m_shader_program->configure();
}

void AtmosphericScatteringLayer::OnDetach() {}

void AtmosphericScatteringLayer::OnUpdate(const sputnik::core::TimeStep& time_step)
{

    graphics::api::Renderer::RenderAtmoshericScattering();

    glEnable(GL_DEPTH_TEST);
    const auto& editor_camera = sputnik::graphics::api::EditorCamera::GetInstance();
    mat4        projection    = editor_camera->GetCameraPerspective();
    mat4        view          = editor_camera->GetCameraView();
    mat4        model{};
    model = model.rotate({0.0, 1.0, 0.0}, -90.0f * kDegToRad);

    m_shader_program->bind();
    m_shader_program->setMat4("projection", projection);
    m_shader_program->setMat4("model", model);
    m_shader_program->setMat4("view", view);

    m_diff_texture->bind(0);
    m_shader_program->setInt("diffuse", 0);

    m_shader_program->setFloat3("light", {0.0f, 5.0f, 5.0f});

    m_animated_model->Draw();

    m_shader_program->unbind();
    glDisable(GL_DEPTH_TEST);
}

void AtmosphericScatteringLayer::OnUpdateUI(const sputnik::core::TimeStep& time_step) {}

} // namespace sputnik::graphics::api
