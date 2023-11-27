#include "pch.h"
#include "camera.h"

#include <transform.h>
#include <camera_transforms.h>

#include <imgui.h>

namespace sputnik::graphics::api
{

Camera::Camera()
{
    m_input_manager = sputnik::core::InputManager::GetInstance();
    UpdateView();
    UpdateProjection();
}

void graphics::api::Camera::UpdateProjection()
{
    m_aspect_ratio = m_viewport_width / m_viewport_height;
    const auto& m  = ramanujan::Perspective(m_fov, m_aspect_ratio, m_near_clip, m_far_clip);
    for(unsigned int i = 0; i < 16; ++i)
    {
        m_projection.m[i] = m.v[i];
    }
}

void graphics::api::Camera::UpdateView()
{
    // const auto& orientation = GetOrientation();
    const auto& target = m_position + GetForwardDirection();
    const auto& up     = GetUpDirection();
    const auto& view   = ramanujan::LookAt({m_position.x, m_position.y, m_position.z},
                                           {target.x, target.y, target.z},
                                           {up.x, up.y, up.z});
    for(unsigned int i = 0; i < 16; ++i)
    {
        m_view.m[i] = view.v[i];
    }
}

void graphics::api::Camera::MousePan(const ramanujan::Vector2& delta) {}

void graphics::api::Camera::MouseRotate(const ramanujan::Vector2& delta)
{
    m_yaw   = m_yaw + (-delta.x * RotationSpeed());
    m_pitch = m_pitch - (delta.y * RotationSpeed());
}

vec3 graphics::api::Camera::CalculatePosition() const
{
    return vec3();
}

float graphics::api::Camera::RotationSpeed() const
{
    return m_rotation_speed;
}

float graphics::api::Camera::ZoomSpeed() const
{
    return m_zoom_speed;
}

float graphics::api::Camera::MovementSpeed() const
{
    return m_movement_speed;
}

Camera* Camera::GetInstance()
{
    static Camera instance;
    return &instance;
}

void Camera::Update(sputnik::core::TimeStep time_step, const bool& block_update)
{
    if(block_update)
    {
        return;
    }

    const ramanujan::Vector2 mouse = m_input_manager->GetCursorPosition();
    const ramanujan::Vector2 delta =
        ramanujan::Vector2((mouse.x - m_previous_mouse_position.x), (mouse.y - m_previous_mouse_position.y));

    if(m_input_manager->IsKeyPressed(KEY_W))
    {
        m_position = m_position + GetForwardDirection() * time_step.GetSeconds() * MovementSpeed();
    }
    if(m_input_manager->IsKeyPressed(KEY_S))
    {
        m_position = m_position - GetForwardDirection() * time_step.GetSeconds() * MovementSpeed();
    }
    if(m_input_manager->IsKeyPressed(KEY_A))
    {
        m_position = m_position - GetRightDirection() * time_step.GetSeconds() * MovementSpeed();
    }
    if(m_input_manager->IsKeyPressed(KEY_D))
    {
        m_position = m_position + GetRightDirection() * time_step.GetSeconds() * MovementSpeed();
    }
    if(m_input_manager->IsKeyPressed(KEY_Q))
    {
        m_position = m_position + GetUpDirection() * time_step.GetSeconds() * MovementSpeed();
    }
    if(m_input_manager->IsKeyPressed(KEY_E))
    {
        m_position = m_position - GetUpDirection() * time_step.GetSeconds() * MovementSpeed();
    }

    if(m_input_manager->IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        MouseRotate(delta);
    }

    m_previous_mouse_position = {mouse.x, mouse.y};

    UpdateView();
}

void Camera::SetViewportSize(float width, float height)
{
    m_viewport_width  = width;
    m_viewport_height = height;
    UpdateProjection();
}

void Camera::SetPosition(const vec3& position)
{
    m_position = position;
    UpdateView();
}

void Camera::SetPerspective(float fov, float aspect, float znear, float zfar)
{
    const auto& m = ramanujan::Perspective(fov, aspect, znear, zfar);
    for(unsigned int i = 0; i < 16; ++i)
    {
        m_projection.m[i] = m.v[i];
    }
}

const mat4& Camera::GetCameraView() const
{
    return m_view;
}

const mat4& Camera::GetCameraPerspective() const
{
    return m_projection;
}

const vec3& Camera::GetCameraPosition() const
{
    return m_position;
}

const vec3& Camera::GetUpDirection() const
{
    const auto& up_dir = GetOrientation() * ramanujan::Vector3(0.0f, 1.0f, 0.0f);
    return {up_dir.x, up_dir.y, up_dir.z};
}

mat4 graphics::api::Camera::GetCameraView()
{
    return m_view;
    // This is allowed because current function is non-const, and this object is non-const. Therefore, we can drop the
    // const qualifier.
    // return const_cast<mat4&>(const_cast<const Camera*>(this)->GetCameraView());
}

mat4 graphics::api::Camera::GetCameraPerspective()
{
    return m_projection;
    // return const_cast<mat4&>(const_cast<const Camera*>(this)->GetCameraPerspective());
}

vec3 graphics::api::Camera::GetCameraPosition()
{
    return m_position;
    // return const_cast<vec3&>(const_cast<const Camera*>(this)->GetCameraPosition());
}

vec3 graphics::api::Camera::GetUpDirection()
{
    return const_cast<vec3&>(const_cast<const Camera*>(this)->GetUpDirection());
}

const vec3 Camera::GetRightDirection() const
{
    const auto& right_dir = GetOrientation() * ramanujan::Vector3(1.0f, 0.0f, 0.0f);
    return {right_dir.x, right_dir.y, right_dir.z};
}

const vec3 Camera::GetForwardDirection() const
{
    const auto& forward_dir = GetOrientation() * ramanujan::Vector3(0.0f, 0.0f, -1.0f);
    return {forward_dir.x, forward_dir.y, forward_dir.z};
}

float Camera::GetPitch() const
{
    return m_pitch;
}

float Camera::GetYaw() const
{
    return m_yaw;
}

void Camera::OnMouseScroll(float y_offset)
{
    m_position = m_position + vec3{0.0f, 0.0f, y_offset * 1.0f};
}

ramanujan::Quaternion Camera::GetOrientation() const
{
    return ramanujan::FromEulerAnglesRadians(m_pitch, m_yaw, 0.0f);
}

} // namespace sputnik::graphics::api
