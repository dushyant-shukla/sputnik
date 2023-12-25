#include "pch.h"
#include "renderer.h"
#include "graphics/core/render_command.h"
#include "graphics/api/PreethamSkyModel.h"
#include "graphics/api/atmospheric-scattering/atmospheric_scattering.h"
#include "main/application.h"

// Todo:: This is a temporary solution to get the sky rendering working
#include <glad/glad.h>

namespace sputnik::graphics::api
{

using namespace sputnik::graphics::glcore;

graphics::core::GraphicsSubsystemType Renderer::s_graphics_subsystem;

void Renderer::Init(const sputnik::graphics::core::GraphicsSubsystemType& subsystem_type)
{
    s_graphics_subsystem = subsystem_type;
    sputnik::graphics::core::RenderCommand::Init(subsystem_type);

    Instance()->InitInstance(subsystem_type);

    main::Application::GetInstance()->PushLayer(std::make_shared<AtmosphericScatteringLayer>());
}

void Renderer::Update(const sputnik::core::TimeStep& time_step)
{
    m_camera->Update(time_step, m_is_camera_update_blocked);
    m_editor_camera->Update(time_step);
}

void Renderer::LateUpdate(const sputnik::core::TimeStep& time_step)
{
    sputnik::graphics::core::RenderCommand::Update(time_step);
}

void Renderer::Clear()
{
    core::RenderCommand::Clear();
}

void Renderer::SetClearColor(float r, float g, float b, float a)
{
    core::RenderCommand::SetClearColor(r, g, b, a);
}

void Renderer::SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    core::RenderCommand::SetViewPort(x, y, width, height);
}

void Renderer::Shutdown() {}

void Renderer::OnWindowResize(uint32_t width, uint32_t height)
{
    core::RenderCommand::SetViewPort(0, 0, width, height);
}

const graphics::core::GraphicsSubsystemType& Renderer::GetCurrentGraphicsSubsystemType()
{
    return s_graphics_subsystem;
}

std::pair<uint32_t, uint32_t> Renderer::GetViewportDimensions()
{
    return core::RenderCommand::GetViewportDimensions();
}

GLFWwindow* Renderer::GetNativeWindow()
{
    return core::RenderCommand::GetNativeWindow();
}

void Renderer::SetCameraType(const CameraType& camera_type)
{
    m_camera_type = camera_type;
}

void Renderer::BlockCameraUpdate(const bool& block)
{
    m_is_camera_update_blocked = block;
}

// TODO:: This is only temporary. It is not the correct place to render the sky or for that matter any other object.
void Renderer::RenderAtmoshericScattering()
{
    auto renderer = Instance();
    glBindVertexArray(renderer->m_vao); // Todo:: Temporary solution

    renderer->m_preetham_sky_model.SetDirection(renderer->m_light_direction);
    renderer->m_preetham_sky_model.Update();

    mat4 camera_projection{};
    mat4 camera_view{};
    vec3 camera_position{};

    if(m_camera_type == CameraType::Camera)
    {
        camera_projection = renderer->m_camera->GetCameraPerspective();
        camera_view       = renderer->m_camera->GetCameraView();
        camera_position   = renderer->m_camera->GetCameraPosition();
    }
    else
    {
        camera_projection = renderer->m_editor_camera->GetCameraPerspective();
        camera_view       = renderer->m_editor_camera->GetCameraView();
        camera_position   = renderer->m_editor_camera->GetCameraPosition();
    }

    // removing translation from the view matrix
    mat4 cubemap_view_mat4{camera_view};
    cubemap_view_mat4.m[3]  = 0;
    cubemap_view_mat4.m[7]  = 0;
    cubemap_view_mat4.m[11] = 0;
    cubemap_view_mat4.m[15] = 1;
    cubemap_view_mat4.m[12] = 0;
    cubemap_view_mat4.m[13] = 0;
    cubemap_view_mat4.m[14] = 0;

    mat4 cubemap_view_projection_mat4     = camera_projection * cubemap_view_mat4;
    mat4 inv_cubemap_view_projection_mat4 = cubemap_view_projection_mat4.inverted();

    // setup pipeline state, there should be a PipelineState API with set() and reset(), bind/unbind methods
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    renderer->m_sky_shader->bind();

    // Uniform<mat4>::Set(renderer->m_sky_shader->GetUniform("inv_view_projection"), inv_cubemap_view_projection_mat4);

    renderer->m_sky_shader->setMat4("inv_view_projection", inv_cubemap_view_projection_mat4);

    renderer->m_preetham_sky_model.SetRenderUniforms(renderer->m_sky_shader);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    renderer->m_sky_shader->unbind();

    // reset the pipeline state
    glDepthFunc(GL_LESS); // This is the default depth function
    glDisable(GL_DEPTH_TEST);

    glBindVertexArray(0); // Todo:: Temporary solution
}

Renderer* Renderer::Instance()
{
    static Renderer instance;
    return &instance;
}

void Renderer::InitInstance(const sputnik::graphics::core::GraphicsSubsystemType& subsystem_type)
{
    // Todo:: Paths to assets are with respect to the current executable.
    // m_sky_shader = std::make_shared<Shader>("../../data/shaders/sky-rendering/sky.vert",
    //                                        "../../data/shaders/sky-rendering/sky.frag");

    // m_sky_shader = std::make_shared<OglShaderProgram>("../data/shaders/sky-rendering/sky.vert",
    //                                                   "../data/shaders/sky-rendering/sky.frag");

    m_sky_shader = std::make_shared<OglShaderProgram>();
    m_sky_shader->addShaderStage("../data/shaders/sky-rendering/sky.vert");
    m_sky_shader->addShaderStage("../data/shaders/sky-rendering/sky.frag");
    m_sky_shader->configure();

    m_light_direction = vec3(0.0f, sin(m_sun_angle), cos(m_sun_angle)).normalized();

    glGenVertexArrays(1, &m_vao);
}

} // namespace sputnik::graphics::api