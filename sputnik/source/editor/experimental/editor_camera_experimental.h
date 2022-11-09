#pragma once

#include "core/core.h"

#include <stdint.h>
#include <vector3.h>
#include <vector2.h>
#include <matrix4.h>
#include <quaternion.h>
#include <transform.h>
#include <core/time_step.h>

struct GLFWwindow;

namespace sputnik::graphics::api
{

class EditorCameraExperimental
{

public:
    NON_COPYABLE(EditorCameraExperimental);

    ~EditorCameraExperimental();

    static EditorCameraExperimental*      Initialize(GLFWwindow* window);
    static EditorCameraExperimental*      GetInstance();
    void                      Update(sputnik::core::TimeStep time_step);
    float                     GetDistance();
    void                      SetDistance(float distance);
    void                      SetViewportSize(float width, float height);
    void                      SetPosition(const ramanujan::Vector3& position);
    void                      SetPerspective(float fov, float aspect, float znear, float zfar);
    const ramanujan::Matrix4& GetCameraView();
    const ramanujan::Matrix4& GetCameraPerspective();
    const ramanujan::Vector3& GetCameraPosition();
    const ramanujan::Vector3  GetUpDirection() const;
    const ramanujan::Vector3  GetRightDirection() const;
    const ramanujan::Vector3  GetForwardDirection() const;
    float                     GetPitch() const;
    float                     GetYaw() const;
    void                      OnMouseScroll(float y_offset);
    ramanujan::Quaternion     GetOrientation() const;

private:
    EditorCameraExperimental(GLFWwindow* window);

    void                    UpdateProjection();
    void                    UpdateView();
    void                    MousePan(const ramanujan::Vector2& delta);
    void                    MouseRotate(const ramanujan::Vector2& delta);
    void                    MouseZoom(float delta);
    ramanujan::Vector3      CalculatePosition() const;
    std::pair<float, float> PanSpeed() const;
    float                   RotationSpeed() const;
    float                   ZoomSpeed() const;

private:
    float m_fov{45.0f};
    float m_aspect_ratio{1.778f};
    float m_near_clip{0.1f};
    float m_far_clip{1000.0f};

    float m_distance{10.0f};
    float m_pitch{0.0f};
    float m_yaw{0.0f};

    float m_viewport_width{1600.0f};
    float m_viewport_height{900.0f};

    // ramanujan::Vector3 m_position{0.0f, 0.0f, 0.0f};
    ramanujan::Vector3 m_position{0.0f, 5.0f, 7.0f};
    ramanujan::Vector3 m_focal_point{0.0f, 0.0f, 0.0f};

    // ramanujan::Vector3 m_rotation{};
    ramanujan::Matrix4   m_view;
    ramanujan::Matrix4   m_projection;

    // glm::mat4           m_view;
    ramanujan::Vector2 m_previous_mouse_position{0.0f, 0.0f};
    float              m_rotation_speed{3.5f};

    static EditorCameraExperimental* s_instance;
    GLFWwindow*          m_window;
};

} // namespace sputnik::graphics::api