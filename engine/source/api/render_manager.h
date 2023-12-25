#pragma once

#include "core/core.h"
#include "graphics/core/graphics_subsystem_type.h"
#include "graphics/api/camera_types.h"
#include "graphics/glcore/gl_shader.h"
#include <graphics/api/PreethamSkyModel.h>
#include "editor/editor_camera.h"
#include "graphics/api/camera.h"

namespace sputnik::engine::api
{

using namespace sputnik::core;
using namespace sputnik::graphics::api;

/**
 * Handles higher level rendering concepts.
 * Configure pipelines, render passes, etc.
 */
class RenderManager
{

public:
    // public interface
    RenderManager* instance();
    ~RenderManager();

    void                                         initialize();
    void                                         update(const TimeStep& timestep);
    void                                         lateUpdate(const TimeStep& timestep);
    void                                         shutdown();
    void                                         clear();
    void                                         setClearColor(float r, float g, float b, float a);
    void                                         setViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
    void                                         onWindowResize(uint32_t width, uint32_t height);
    void                                         SetCameraType(const CameraType& camera_type);
    void                                         BlockCameraUpdate(const bool& block = true);
    void                                         RenderAtmoshericScattering();
    const graphics::core::GraphicsSubsystemType& getCurrentGraphicsSubsystemType();
    std::pair<uint32_t, uint32_t>                getViewportDimensions();
    GLFWwindow*                                  getNativeWindow();

protected:
    // protected methods
    RenderManager();

private:
    // private methods

private:
    // private data
    static graphics::core::GraphicsSubsystemType s_graphics_subsystem;

    // Todo:: This is temporary. Ideally camera should have a polymorphic interface.
    inline static CameraType    m_camera_type              = CameraType::Camera;
    inline static Camera*       m_camera                   = Camera::GetInstance();
    inline static EditorCamera* m_editor_camera            = EditorCamera::GetInstance();
    inline static bool          m_is_camera_update_blocked = false;

    // atmospheric scattering
    float                             m_exposure        = 1.0f;
    float                             m_sun_angle       = -1.45f; // radians (~ (-83) degress)
    vec3                              m_light_direction = vec3(0.0f, 0.0f, 1.0f);
    PreethamSkyModel                  m_preetham_sky_model;
    std::shared_ptr<OglShaderProgram> m_sky_shader;

    u32 m_vao;
};

} // namespace sputnik::engine::api