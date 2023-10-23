#pragma once

#include <core/time_step.h>
#include <core/input/input_manager.h>

#include <vector.hpp>
#include <matrix.hpp>
#include <quaternion.h>

namespace sputnik::graphics::api
{

using namespace ramanujan;
using namespace ramanujan::experimental;

class Camera
{
public:
    virtual ~Camera() = default;

    static Camera*        GetInstance();
    void                  Update(sputnik::core::TimeStep time_step, const bool& block_update);
    void                  SetViewportSize(float width, float height);
    void                  SetPosition(const vec3& position);
    void                  SetPerspective(float fov, float aspect, float znear, float zfar);
    const mat4&           GetCameraView();
    const mat4&           GetCameraPerspective();
    const vec3&           GetCameraPosition();
    const vec3            GetUpDirection() const;
    const vec3            GetRightDirection() const;
    const vec3            GetForwardDirection() const;
    float                 GetPitch() const;
    float                 GetYaw() const;
    void                  OnMouseScroll(float y_offset);
    ramanujan::Quaternion GetOrientation() const;

private:
    Camera();

    void  UpdateProjection();
    void  UpdateView();
    void  MousePan(const ramanujan::Vector2& delta);
    void  MouseRotate(const ramanujan::Vector2& delta);
    vec3  CalculatePosition() const;
    float RotationSpeed() const;
    float ZoomSpeed() const;
    float MovementSpeed() const;

private:
    float      m_fov{45.0f};
    float      m_aspect_ratio{1.778f};
    float      m_near_clip{0.1f};
    float      m_far_clip{1000.0f};
    vec3       m_position{0.0f, 2.5f, 10.0f};
    float      m_pitch{0.0f};
    float      m_yaw{0.0f};
    Quaternion m_orientation;

    float m_viewport_width{1600.0f};
    float m_viewport_height{900.0f};

    mat4 m_view;
    mat4 m_projection;

    vec2  m_previous_mouse_position{0.0f, 0.0f};
    float m_rotation_speed{0.005f};
    float m_movement_speed{5.0f};
    float m_zoom_speed{5.50f};

    sputnik::core::InputManager* m_input_manager;
};
} // namespace sputnik::graphics::api
