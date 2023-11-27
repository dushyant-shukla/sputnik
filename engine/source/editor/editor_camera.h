#pragma once

#include "core/core.h"
#include "core/input/input_manager.h"

#include <stdint.h>
#include <vector3.h>
#include <vector2.h>
#include <matrix4.h>
#include <quaternion.h>
#include <transform.h>
#include <core/time_step.h>
#include <vector.hpp>
#include <matrix.hpp>

struct GLFWwindow;

namespace sputnik::graphics::api
{

using namespace ramanujan::experimental;

class EditorCamera
{

public:
    NON_COPYABLE(EditorCamera)

    ~EditorCamera();

    static EditorCamera*      GetInstance();
    void                      Update(sputnik::core::TimeStep time_step);
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
    EditorCamera();

    void               UpdateProjection();
    void               UpdateView();
    void               MousePan(const ramanujan::Vector2& delta);
    void               MouseRotate(const ramanujan::Vector2& delta);
    ramanujan::Vector3 CalculatePosition() const;
    float              RotationSpeed() const;
    float              ZoomSpeed() const;

private:
    float                 m_fov{45.0f};
    float                 m_aspect_ratio{1.778f};
    float                 m_near_clip{0.1f};
    float                 m_far_clip{1000.0f};
    ramanujan::Vector3    m_position{0.0f, 2.5f, 10.0f};
    float                 m_pitch{0.0f};
    float                 m_yaw{0.0f};
    ramanujan::Quaternion m_orientation;

    float m_viewport_width{1600.0f};
    float m_viewport_height{900.0f};

    ramanujan::Matrix4 m_view;
    ramanujan::Matrix4 m_projection;

    ramanujan::Vector2 m_previous_mouse_position{0.0f, 0.0f};
    float              m_rotation_speed{0.005f};

    sputnik::core::InputManager* m_input_manager;
};

} // namespace sputnik::graphics::api