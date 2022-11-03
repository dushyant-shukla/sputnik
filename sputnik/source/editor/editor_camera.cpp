#include "pch.h"
#include "editor_camera.h"

#include <transform.h>
#include <camera_transforms.h>
#include <quaternion.h>
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

namespace sputnik::graphics::api
{

EditorCamera::EditorCamera()
{
    m_input_manager = sputnik::core::InputManager::GetInstance();
    UpdateView();
    UpdateProjection();
}

EditorCamera::~EditorCamera() {}

EditorCamera* EditorCamera::GetInstance()
{
    static EditorCamera instance;
    return &instance;
}

void EditorCamera::Update(sputnik::core::TimeStep time_step)
{
    const ramanujan::Vector2 mouse = m_input_manager->GetCursorPosition();
    if(m_input_manager->IsKeyPressed(KEY_LEFT_ALT))
    {
        const ramanujan::Vector2 delta =
            ramanujan::Vector2((mouse.x - m_previous_mouse_position.x), (mouse.y - m_previous_mouse_position.y));
        if(m_input_manager->IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE))
        {
            MousePan(delta);
        }
        else if(m_input_manager->IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            MouseRotate(delta);
        }
    }

    m_previous_mouse_position = mouse;

    OnMouseScroll(m_input_manager->GetScrollOffset().y);

    UpdateView();
}

void EditorCamera::SetViewportSize(float width, float height)
{
    m_viewport_width  = width;
    m_viewport_height = height;
    UpdateProjection();
}

void EditorCamera::SetPosition(const ramanujan::Vector3& position)
{
    m_position = position;
}

void EditorCamera::SetPerspective(float fov, float aspect, float znear, float zfar)
{
    m_projection = ramanujan::Perspective(fov, aspect, znear, zfar);
}

const ramanujan::Matrix4& EditorCamera::GetCameraView()
{
    return m_view;
}

const ramanujan::Matrix4& EditorCamera::GetCameraPerspective()
{
    return m_projection;
}

const ramanujan::Vector3& EditorCamera::GetCameraPosition()
{
    return m_position;
}

const ramanujan::Vector3 EditorCamera::GetUpDirection() const
{
    return GetOrientation() * ramanujan::Vector3(0.0f, 1.0f, 0.0f);
}

const ramanujan::Vector3 EditorCamera::GetRightDirection() const
{
    return GetOrientation() * ramanujan::Vector3(1.0f, 0.0f, 0.0f);
}

const ramanujan::Vector3 EditorCamera::GetForwardDirection() const
{
    return GetOrientation() * ramanujan::Vector3(0.0f, 0.0f, -1.0f);
}

float EditorCamera::GetPitch() const
{
    return m_pitch;
}

float EditorCamera::GetYaw() const
{
    return m_yaw;
}

void EditorCamera::OnMouseScroll(float y_offset)
{
    m_position = m_position + ramanujan::Vector3(0.0f, 0.0f, y_offset * 1.0f);
}

ramanujan::Quaternion EditorCamera::GetOrientation() const
{
    return ramanujan::FromEulerAnglesRadians(m_pitch, m_yaw, 0.0f);
}

void EditorCamera::UpdateProjection()
{
    m_aspect_ratio = m_viewport_width / m_viewport_height;
    m_projection   = ramanujan::Perspective(m_fov, m_aspect_ratio, m_near_clip, m_far_clip);
}

void EditorCamera::UpdateView()
{
    ramanujan::Transform transform;
    transform.position                = m_position;
    ramanujan::Quaternion orientation = GetOrientation();
    m_view                            = ramanujan::ToMatrix4(orientation) * ramanujan::ToMatrix4(transform);
    m_view                            = ramanujan::Inverse(m_view);
}

void EditorCamera::MousePan(const ramanujan::Vector2& delta)
{
    m_position = m_position + ramanujan::Vector3(-delta.x * 0.05f, delta.y * 0.05f, 0.0f);
}

void EditorCamera::MouseRotate(const ramanujan::Vector2& delta)
{
    m_yaw   = m_yaw + (-delta.x * RotationSpeed());
    m_pitch = m_pitch - (delta.y * RotationSpeed());
}

float EditorCamera::RotationSpeed() const
{
    return m_rotation_speed;
}

} // namespace sputnik::graphics::api