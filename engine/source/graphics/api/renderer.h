#pragma once

#include "core/core.h"
#include "core/time_step.h"
#include "graphics/core/graphics_subsystem_type.h"
#include "graphics/api/camera_types.h"
#include "graphics/glcore/shader.h"
#include <graphics/api/PreethamSkyModel.h>
#include "editor/editor_camera.h"
#include "graphics/api/camera.h"

#include <memory>

struct GLFWwindow;

namespace sputnik::graphics::api
{

using namespace sputnik::graphics::glcore;

class Renderer
{

public:
    static void Init(const sputnik::graphics::core::GraphicsSubsystemType& subsystem_type);
    static void Update(const sputnik::core::TimeStep& time_step);
    static void LateUpdate(const sputnik::core::TimeStep& time_step);
    static void Clear();
    static void SetClearColor(float r, float g, float b, float a);
    static void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
    static void Shutdown();
    static void OnWindowResize(uint32_t width, uint32_t height);
    static const graphics::core::GraphicsSubsystemType& GetCurrentGraphicsSubsystemType();
    static std::pair<uint32_t, uint32_t>                GetViewportDimensions();
    static GLFWwindow*                                  GetNativeWindow();

    static void SetCameraType(const CameraType& camera_type);
    static void BlockCameraUpdate(const bool& block = true);
    static void RenderAtmoshericScattering();

protected:
    static Renderer* Instance();
    void             InitInstance(const sputnik::graphics::core::GraphicsSubsystemType& subsystem_type);

private:
    static graphics::core::GraphicsSubsystemType s_graphics_subsystem;

    // Todo:: This is temporary. Ideally camera should have a polymorphic interface.
    inline static CameraType    m_camera_type              = CameraType::Camera;
    inline static Camera*       m_camera                   = Camera::GetInstance();
    inline static EditorCamera* m_editor_camera            = EditorCamera::GetInstance();
    inline static bool          m_is_camera_update_blocked = false;

    // atmospheric scattering
    float                   m_exposure        = 1.0f;
    float                   m_sun_angle       = -1.45f; // radians (~ (-83) degress)
    vec3                    m_light_direction = vec3(0.0f, 0.0f, 1.0f);
    PreethamSkyModel        m_preetham_sky_model;
    std::shared_ptr<Shader> m_sky_shader;
};

} // namespace sputnik::graphics::api