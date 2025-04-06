#include "pch.h"
#include "render_system.h"
#include "graphics/glcore/gl_renderer.h"
#include "editor/editor.hpp"

#include <GLFW/glfw3.h>

namespace sputnik::core::systems
{

RenderSystem::RenderSystem() : m_ogl_renderer{nullptr}, m_active_render_system_type{RenderSystemType::NONE} {}

RenderSystem::~RenderSystem() {}

RenderSystem::RenderSystem(RenderSystem&& other) noexcept : m_ogl_renderer{nullptr}
{
    *this = std::move(other);
}

RenderSystem& RenderSystem::operator=(RenderSystem&& other) noexcept
{
    if(this != &other)
    {
        m_gl_pipeline  = other.m_gl_pipeline;
        m_ogl_renderer = other.m_ogl_renderer;
    }
    return *this; // TODO: insert return statement here
}

RenderSystem* RenderSystem::getInstance()
{
    static RenderSystem instance;
    return &instance;
}

void RenderSystem::initialize(const RenderSystemType& render_system_type)
{
    switch(render_system_type)
    {
    case RenderSystemType::OPEN_GL:
    {
        m_window       = std::make_unique<Window>();
        m_ogl_renderer = OglRenderer::getInstance(m_window->GetNativeWindow());
        setupOglPipeline();
        break;
    }
    default:
        SPUTNIK_ASSERT_MESSAGE(false, "Invalid render system type: {}", render_system_type);
        break;
    }
    m_active_render_system_type = render_system_type;
    ENGINE_INFO("Render system initialized with {}", m_active_render_system_type);
}

void RenderSystem::setupOglPipeline()
{
    m_gl_pipeline = [this](double timestep) -> void { ENGINE_TRACE("OPENGL PIPELINE EXECUTED..."); };
}

void RenderSystem::update(const TimeStep& timestep)
{
    // Todo:: update cameras (temporary)
    m_camera->Update(timestep, m_is_camera_update_blocked);
    m_editor_camera->Update(timestep);
    // renderAtmosphericScattering();
    //  m_gl_pipeline(timestep);

    // send in the camera matrices (camera pointer), and the
    if(InputManager::GetInstance()->IsKeyTriggered(KEY_C))
    {
        if(m_camera_type == CameraType::Camera)
        {
            m_camera_type = CameraType::EditorCamera;
        }
        else
        {
            m_camera_type = CameraType::Camera;
        }
    }

    if(m_camera_type == CameraType::Camera)
    {
        m_ogl_renderer->render(m_camera->GetCameraPerspective(),
                               m_camera->GetCameraView(),
                               m_camera->GetCameraPosition(),
                               m_light);
    }
    else
    {
        m_ogl_renderer->render(m_editor_camera->GetCameraPerspective(),
                               m_editor_camera->GetCameraView(),
                               m_editor_camera->GetCameraPosition(),
                               m_light);
    }

    // SceneManager will register it's render method as a callback for drawing all the geometries in the scene
    // This gives the engine a flexibility to use a client provided draw method
    // SceneManager::render()
    //      for each geometry
    //          call RenderSystem::Draw(geometry, model, material)
    //               call OglRenderer::Draw() | OglRenderer::DrawIndirect(geometry, model, material)
    // Todo:: Figure out the best way to achieve this
}

void RenderSystem::lateUpdate(const TimeStep& timestep)
{
    m_window->OnUpdate(timestep);
    m_ogl_renderer->lateUpdate(timestep, m_window->GetNativeWindow());
}

void RenderSystem::onWindowResize(const u32& width, const u32& height)
{
    // Todo:: Temporary until we have event pub/sub system
    // do whatever that's needed when resizing the window
    EditorCamera::GetInstance()->SetViewportSize(static_cast<float>(width), static_cast<float>(height));
    Camera::GetInstance()->SetViewportSize(static_cast<float>(width), static_cast<float>(height));
    m_ogl_renderer->resizeViewport(width, height);
}

void RenderSystem::updateLightGpuBuffer()
{
    // render light imgui widget
    // call ogl renderer to update the light uniform buffer
}

// const std::unique_ptr<Window>& RenderSystem::getWindow()
//{
//     return m_window;
// }

std::shared_ptr<Window> RenderSystem::getWindow()
{
    return m_window;
}

void core::systems::RenderSystem::clear()
{
    m_ogl_renderer->clear();
}

void core::systems::RenderSystem::setClearColor(float r, float g, float b, float a)
{
    m_ogl_renderer->setClearColor(r, g, b, a);
}

void core::systems::RenderSystem::preUpdate(TimeStep& timestep)
{
    // setClearColor(0.16f, 0.16f, 0.16f, 1.00f);
    // clear();
}

void RenderSystem::blockCameraUpdate(const bool& block)
{
    m_is_camera_update_blocked = block;
}

// void RenderSystem::renderAtmosphericScattering()
//{
//     m_ogl_renderer->renderAtmosphericScattering();
// }

void RenderSystem::setViewportToCurrentWindowSize()
{
    m_window->setViewportToCurrentWindowSize();
    const auto& [width, height] = m_window->GetWindowWidthAndHeight();
    EditorCamera::GetInstance()->SetViewportSize(static_cast<float>(width), static_cast<float>(height));
    Camera::GetInstance()->SetViewportSize(static_cast<float>(width), static_cast<float>(height));
    m_ogl_renderer->resizeViewport(width, height);
}

void core::systems::RenderSystem::drawUI()
{
    if(ImGui::Begin("Lighting"))
    {
        sputnik::editor::Editor::drawWidgetVec3("position", m_light.position, 90.0f);
        sputnik::editor::Editor::drawWidgetColor3("ambient", m_light.ambient, 90.0f);
        sputnik::editor::Editor::drawWidgetColor3("diffuse", m_light.diffuse, 90.0f);
        sputnik::editor::Editor::drawWidgetColor3("specular", m_light.specular, 90.0f);
        sputnik::editor::Editor::drawWidgetFloat("constant", m_light.constant, 90.0f);
        sputnik::editor::Editor::drawWidgetFloat("linear", m_light.linear, 90.0f);
        sputnik::editor::Editor::drawWidgetFloat("quadratic", m_light.quadratic, 90.0f);
    }
    ImGui::End();
}

Light& RenderSystem::getLight()
{
    return m_light;
}

const mat4 RenderSystem::getCameraProjection() const
{
    if(m_camera_type == CameraType::Camera)
    {
        return m_camera->GetCameraPerspective();
    }
    else
    {
        return m_editor_camera->GetCameraPerspective();
    }
}

const mat4 RenderSystem::getCameraView() const
{
    if(m_camera_type == CameraType::Camera)
    {
        return m_camera->GetCameraView();
    }
    else
    {
        return m_editor_camera->GetCameraView();
    }
}

void RenderSystem::setCameraType(CameraType type)
{
    m_camera_type = type;
}

////////////////////////////////////////// Drawing API //////////////////////////////////////////

void RenderSystem::drawTriangles(const u64& vertex_count, const Material& material, const mat4& model)
{
    m_ogl_renderer->drawTriangles(vertex_count, material, model);
}

void RenderSystem::drawTrianglesIndexed(const u64& index_count, const Material& material, const mat4& model)
{
    m_ogl_renderer->drawTrianglesIndexed(index_count, material, model);
}

void RenderSystem::drawTrianglesIndexed(const u64&                  vertex_count,
                                        const Material&             material,
                                        const mat4&                 model,
                                        const std::vector<Matrix4>& skin_transformations)
{
    m_ogl_renderer->drawTrianglesIndexed(vertex_count, material, model, skin_transformations);
}

void RenderSystem::drawTrianglesInstanced(const u64& vertex_count, const Material& material, const u32& instance_count)
{
    m_ogl_renderer->drawTrianglesInstanced(vertex_count, instance_count, material);
}

void RenderSystem::drawTrianglesIndexedInstanced(const u64&      index_count,
                                                 const Material& material,
                                                 const u32&      instance_count)
{
    m_ogl_renderer->drawTrianglesIndexedInstanced(index_count, instance_count, material);
}

void RenderSystem::drawDebugLines(const std::vector<vec4>& vertices, const vec3& color, const float& line_width)
{
    m_ogl_renderer->drawDebugLines(vertices, color, line_width);
}

void RenderSystem::drawDebugPoints(const std::vector<vec4>& vertices, const vec3& color, const float& point_size)
{
    m_ogl_renderer->drawDebugPoints(vertices, color, point_size);
}

void RenderSystem::drawDebugLines(const std::vector<vec4>& vertices,
                                  const vec3&              color,
                                  const mat4&              model,
                                  const float&             line_width)
{
    m_ogl_renderer->drawDebugLines(vertices, color, model, line_width);
}

void RenderSystem::drawDebugLines(const std::vector<vec4>& vertices,
                                  const vec3&              color,
                                  const glm::mat4&         model,
                                  const float&             line_width)
{
    m_ogl_renderer->drawDebugLines(vertices, color, model, line_width);
}

void RenderSystem::drawDebugPoints(const std::vector<vec4>& vertices,
                                   const vec3&              color,
                                   const mat4&              model,
                                   const float&             point_size)
{
    m_ogl_renderer->drawDebugPoints(vertices, color, model, point_size);
}

void RenderSystem::drawDebugPoints(const std::vector<vec4>& vertices,
                                   const vec3&              color,
                                   const glm::mat4&         model,
                                   const float&             point_size)
{
    m_ogl_renderer->drawDebugPoints(vertices, color, model, point_size);
}

} // namespace sputnik::core::systems