#pragma once

#include "core/core.h"
#include "graphics/api/light.h"
#include "graphics/api/PreethamSkyModel.h"
#include "graphics/api/camera_types.h"
#include "editor/editor_camera.h"
#include "graphics/api/camera.h"
#include "graphics/window/window.h"
#include "graphics/api/color_material.h"

#include <vector.hpp>
#include <matrix.hpp>
#include <glm/glm.hpp>

struct GLFWwindow;

namespace sputnik::graphics::gl
{
class OglRenderer;
}

namespace sputnik::graphics::window
{
class Window;
} // namespace sputnik::graphics::window

namespace sputnik::core::systems
{

using namespace sputnik::core;
using namespace sputnik::graphics::gl;
using namespace sputnik::graphics::api;
using namespace sputnik::graphics::window;
using namespace ramanujan::experimental;

enum class RenderSystemType
{
    NONE,
    OPEN_GL,
    VULKAN,
    DIRECT_X
};

inline std::ostream& operator<<(std::ostream& os, const RenderSystemType& render_system_type)
{
    switch(render_system_type)
    {
        SPUTNIK_CASE_TO_OSTREAM(RenderSystemType::OPEN_GL, os)
        SPUTNIK_CASE_TO_OSTREAM(RenderSystemType::VULKAN, os)
        SPUTNIK_CASE_TO_OSTREAM(RenderSystemType::DIRECT_X, os)
        SPUTNIK_DEFAULT_TO_OSTREAM(os)
    }
    return os;
}

class RenderSystem
{

public:
    // public interface

    ~RenderSystem();

    RenderSystem(RenderSystem&& other) noexcept;
    RenderSystem& operator=(RenderSystem&& other) noexcept;

    static RenderSystem* getInstance();

    void initialize(const RenderSystemType& render_system_type);
    void update(const TimeStep& delta_time);
    void lateUpdate(const TimeStep& delta_time);

    void onWindowResize(const u32& width, const u32& height);

    // const std::unique_ptr<Window>& getWindow();
    std::shared_ptr<Window> getWindow();

    void preUpdate(TimeStep& timestep);

    void clear();
    void setClearColor(float r, float g, float b, float a);

    void blockCameraUpdate(const bool& block = true);

    // void renderAtmosphericScattering();

    void setViewportToCurrentWindowSize();

    void drawUI();

    Light& getLight();

    const mat4 getCameraProjection() const;
    const mat4 getCameraView() const;

    void setCameraType(CameraType type);

    // Drawing API
    void drawTriangles(const u64& vertex_count, const Material& material, const mat4& model);
    void drawTrianglesIndexed(const u64& vertex_count, const Material& material, const mat4& model);
    void drawTrianglesIndexed(const u64&                  vertex_count,
                              const Material&             material,
                              const mat4&                 model,
                              const std::vector<Matrix4>& skin_transformations);
    void drawTrianglesInstanced(const u64& vertex_count, const Material& material, const u32& instance_count);
    void drawTrianglesIndexedInstanced(const u64& index_count, const Material& material, const u32& instance_count);
    void drawDebugLines(const std::vector<vec4>& vertices, const vec3& color, const float& line_width = 2.5f);
    void drawDebugPoints(const std::vector<vec4>& vertices, const vec3& color, const float& point_size = 2.5f);

    void drawDebugLines(const std::vector<vec4>& vertices,
                        const vec3&              color,
                        const mat4&              model      = {},
                        const float&             line_width = 2.5f);
    void drawDebugLines(const std::vector<vec4>& vertices,
                        const vec3&              color,
                        const glm::mat4&         model      = glm::mat4(1.0f),
                        const float&             line_width = 2.5f);

    void drawDebugPoints(const std::vector<vec4>& vertices,
                         const vec3&              color,
                         const mat4&              model      = {},
                         const float&             point_size = 2.5f);

    void drawDebugPoints(const std::vector<vec4>& vertices,
                         const vec3&              color,
                         const glm::mat4&         model      = glm::mat4(1.0f),
                         const float&             point_size = 2.5f);

private:
    RenderSystem();
    RenderSystem(const RenderSystem&)            = delete;
    RenderSystem& operator=(const RenderSystem&) = delete;

    void setupOglPipeline();

    // call ogl_renderer->beginAtmosphericScattering() and ogl_renderer->endAtmosphericScattering()
    // void renderAtmosphere();
    // void renderInfiniteGrid();

    void updateLightGpuBuffer();

private:
    // private data

    float            m_exposure        = 1.0f;
    float            m_sun_angle       = -1.45f; // radians (~ (-83) degress)
    vec3             m_light_direction = vec3(0.0f, 0.0f, 1.0f);
    PreethamSkyModel m_preetham_sky_model;

    // Instances of renderers
    OglRenderer* m_ogl_renderer;

    // Instances of pipelines (vk pipeline, dx pipeline)
    std::function<void(double)> m_gl_pipeline;

    // Camera instances (editor, flyby)

    // Maybe this could be a collection of windows? (left, right, stereoscopic rendering)
    // GLFWwindow* m_window_handle;

    // std::unique_ptr<Window> m_window;
    std::shared_ptr<Window> m_window;

    // Light m_lights[4];
    sputnik::graphics::api::Light        m_light;
    sputnik::graphics::api::PerFrameData m_per_frame_data;

    RenderSystemType m_active_render_system_type;

    inline static CameraType    m_camera_type              = CameraType::Camera;
    inline static Camera*       m_camera                   = Camera::GetInstance();
    inline static EditorCamera* m_editor_camera            = EditorCamera::GetInstance();
    inline static bool          m_is_camera_update_blocked = false;
};

} // namespace sputnik::core::systems