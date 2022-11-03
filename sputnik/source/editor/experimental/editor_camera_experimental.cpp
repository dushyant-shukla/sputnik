#include "pch.h"
#include "editor_camera_experimental.h"

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

EditorCameraExperimental* EditorCameraExperimental::s_instance = nullptr;

EditorCameraExperimental::EditorCameraExperimental(GLFWwindow* window) : m_window(window)
{
}

EditorCameraExperimental::~EditorCameraExperimental() {}

EditorCameraExperimental* EditorCameraExperimental::Initialize(GLFWwindow* window)
{
    if(s_instance == nullptr)
    {
        s_instance = new EditorCameraExperimental(window);
        s_instance->UpdateView();
        s_instance->UpdateProjection();
    }
    return s_instance;
}

EditorCameraExperimental* EditorCameraExperimental::GetInstance()
{
    return s_instance;
}

void EditorCameraExperimental::Update(sputnik::core::TimeStep time_step)
{
    if(GLFW_PRESS == glfwGetKey(m_window, GLFW_KEY_LEFT_ALT) || GLFW_REPEAT == glfwGetKey(m_window, GLFW_KEY_LEFT_ALT))
    {
        double mouse_pos_x, mouse_pos_y;
        glfwGetCursorPos(m_window, &mouse_pos_x, &mouse_pos_y);
        const ramanujan::Vector2 mouse(static_cast<float>(mouse_pos_x), static_cast<float>(mouse_pos_y));
        const ramanujan::Vector2 delta = ramanujan::Vector2((mouse.x - m_previous_mouse_position.x) * 0.003f,
                                                            (mouse.y - m_previous_mouse_position.y) * 0.003f);
        m_previous_mouse_position      = mouse;

        if(GLFW_PRESS == glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_MIDDLE))
        {
            MousePan(delta);
        }
        else if(GLFW_PRESS == glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT))
        {
            MouseRotate(delta);
        }
        else if(GLFW_PRESS == glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT))
        {
            MouseZoom(delta.y);
        }
    }

    UpdateView();
}

float EditorCameraExperimental::GetDistance()
{
    return m_distance;
}

void EditorCameraExperimental::SetDistance(float distance)
{
    m_distance = distance;
}

void EditorCameraExperimental::SetViewportSize(float width, float height)
{
    m_viewport_width  = width;
    m_viewport_height = height;
    UpdateProjection();
}

void EditorCameraExperimental::SetPosition(const ramanujan::Vector3& position)
{
    m_position = position;
}

void EditorCameraExperimental::SetPerspective(float fov, float aspect, float znear, float zfar)
{
    m_projection = ramanujan::Perspective(fov, aspect, znear, zfar);
}

const ramanujan::Matrix4& EditorCameraExperimental::GetCameraView()
{
    return m_view;
}

const ramanujan::Matrix4& EditorCameraExperimental::GetCameraPerspective()
{
    return m_projection;
}

const ramanujan::Vector3& EditorCameraExperimental::GetCameraPosition()
{
    return m_position;
}

const ramanujan::Vector3 EditorCameraExperimental::GetUpDirection() const
{
    return GetOrientation() * ramanujan::Vector3(0.0f, 1.0f, 0.0f);
}

const ramanujan::Vector3 EditorCameraExperimental::GetRightDirection() const
{
    return GetOrientation() * ramanujan::Vector3(1.0f, 0.0f, 0.0f);
}

const ramanujan::Vector3 EditorCameraExperimental::GetForwardDirection() const
{
    return GetOrientation() * ramanujan::Vector3(0.0f, 0.0f, -1.0f);
}

float EditorCameraExperimental::GetPitch() const
{
    return m_pitch;
}

float EditorCameraExperimental::GetYaw() const
{
    return m_yaw;
}

void EditorCameraExperimental::OnMouseScroll(float y_offset)
{
    float delta = y_offset * 0.1f;
    MouseZoom(delta);
    UpdateView();
}

ramanujan::Quaternion EditorCameraExperimental::GetOrientation() const
{
    return ramanujan::FromEulerAnglesRadians(-m_pitch, -m_yaw, 0.0f);
}

void EditorCameraExperimental::UpdateProjection()
{
    m_aspect_ratio = m_viewport_width / m_viewport_height;
    m_projection   = ramanujan::Perspective(m_fov, m_aspect_ratio, m_near_clip, m_far_clip);
}

void EditorCameraExperimental::UpdateView()
{
    // m_yaw = m_pitch = 0.0f; // lock camera's rotation
    m_position                        = CalculatePosition();
    ramanujan::Quaternion orientation = GetOrientation();
    ramanujan::Transform  transform;
    transform.position = m_position;
    m_view             = ramanujan::ToMatrix4(transform) * ramanujan::ToMatrix4(orientation);
    m_view             = ramanujan::Inverse(m_view);
}

void EditorCameraExperimental::MousePan(const ramanujan::Vector2& delta)
{
    auto [xSpeed, ySpeed] = PanSpeed();
    m_focal_point         = m_focal_point + (-1 * GetRightDirection() * delta.x * xSpeed * m_distance);
    m_focal_point         = m_focal_point + (GetUpDirection() * delta.y * ySpeed * m_distance);
}

void EditorCameraExperimental::MouseRotate(const ramanujan::Vector2& delta)
{
    float yaw_sign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
    m_yaw += yaw_sign * delta.x * RotationSpeed();
    m_pitch += delta.y * RotationSpeed();
}

void EditorCameraExperimental::MouseZoom(float delta)
{
    m_distance -= delta * ZoomSpeed();
    if(m_distance < 1.0f)
    {
        m_focal_point = m_focal_point + GetForwardDirection();
        m_distance    = 1.0f;
    }
}

ramanujan::Vector3 EditorCameraExperimental::CalculatePosition() const
{
    return m_focal_point - GetForwardDirection() * m_distance;
}

std::pair<float, float> EditorCameraExperimental::PanSpeed() const
{
    float x        = std::min(m_viewport_width / 1000.0f, 2.4f);
    float x_factor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

    float y        = std::min(m_viewport_height / 1000.0f, 2.4f);
    float y_factor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

    return {x_factor, y_factor};
}

float EditorCameraExperimental::RotationSpeed() const
{
    return m_rotation_speed;
}

float EditorCameraExperimental::ZoomSpeed() const
{
    float distance = std::max(m_distance * 0.2f, 0.0f);
    return std::min(distance * distance, 100.0f);
}

} // namespace sputnik::graphics::api