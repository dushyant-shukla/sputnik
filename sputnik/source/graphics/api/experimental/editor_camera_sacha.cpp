#include "pch.h"
#include "editor_camera_sacha.h"

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

EditorCameraSacha* EditorCameraSacha::s_instance = nullptr;

EditorCameraSacha::EditorCameraSacha(GLFWwindow* window) : m_window(window) {}

EditorCameraSacha::~EditorCameraSacha() {}

EditorCameraSacha* EditorCameraSacha::Initialize(GLFWwindow* window)
{
    if(s_instance == nullptr)
    {
        s_instance = new EditorCameraSacha(window);
        s_instance->UpdateView();
        s_instance->UpdateProjection();
    }
    return s_instance;
}

EditorCameraSacha* EditorCameraSacha::GetInstance()
{
    return s_instance;
}

void EditorCameraSacha::Update(sputnik::core::TimeStep time_step)
{
    double mouse_pos_x, mouse_pos_y;
    glfwGetCursorPos(m_window, &mouse_pos_x, &mouse_pos_y);
    const ramanujan::Vector2 mouse(static_cast<float>(mouse_pos_x), static_cast<float>(mouse_pos_y));
    if(GLFW_PRESS == glfwGetKey(m_window, GLFW_KEY_LEFT_ALT) || GLFW_REPEAT == glfwGetKey(m_window, GLFW_KEY_LEFT_ALT))
    {

        const ramanujan::Vector2 delta =
            ramanujan::Vector2((mouse.x - m_previous_mouse_position.x), (mouse.y - m_previous_mouse_position.y));

        if(GLFW_PRESS == glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_MIDDLE))
        {
            MousePan(delta);
        }
        else if(GLFW_PRESS == glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT))
        {
            MouseRotate(delta);
        }
        //else if(GLFW_PRESS == glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT))
        //{
        //    MouseZoom(delta.y);
        //}
    }
    m_previous_mouse_position = mouse;
    UpdateView();
}

void EditorCameraSacha::SetViewportSize(float width, float height)
{
    m_viewport_width  = width;
    m_viewport_height = height;
    UpdateProjection();
}

void EditorCameraSacha::SetPosition(const ramanujan::Vector3& position)
{
    ex_position = position;
}

void EditorCameraSacha::SetPerspective(float fov, float aspect, float znear, float zfar)
{
    m_projection = ramanujan::Perspective(fov, aspect, znear, zfar);
}

const ramanujan::Matrix4& EditorCameraSacha::GetCameraView()
{
    return m_view;
}

const ramanujan::Matrix4& EditorCameraSacha::GetCameraPerspective()
{
    return m_projection;
}

const ramanujan::Vector3& EditorCameraSacha::GetCameraPosition()
{
    return ex_position;
}

const ramanujan::Vector3 EditorCameraSacha::GetUpDirection() const
{
    return GetOrientation() * ramanujan::Vector3(0.0f, 1.0f, 0.0f);
}

const ramanujan::Vector3 EditorCameraSacha::GetRightDirection() const
{
    return GetOrientation() * ramanujan::Vector3(1.0f, 0.0f, 0.0f);
}

const ramanujan::Vector3 EditorCameraSacha::GetForwardDirection() const
{
    return GetOrientation() * ramanujan::Vector3(0.0f, 0.0f, -1.0f);
}

float EditorCameraSacha::GetPitch() const
{
    return ex_pitch;
}

float EditorCameraSacha::GetYaw() const
{
    return ex_yaw;
}

void EditorCameraSacha::OnMouseScroll(float y_offset)
{
    ex_position = ex_position + ramanujan::Vector3(0.0f, 0.0f, y_offset * 1.0f);
    UpdateView();
}

ramanujan::Quaternion EditorCameraSacha::GetOrientation() const
{
    return ramanujan::FromEulerAnglesRadians(ex_pitch, ex_yaw, 0.0f);
}

void EditorCameraSacha::UpdateProjection()
{
    m_aspect_ratio = m_viewport_width / m_viewport_height;
    m_projection   = ramanujan::Perspective(m_fov, m_aspect_ratio, m_near_clip, m_far_clip);
}

void EditorCameraSacha::UpdateView()
{
    ramanujan::Transform transform;
    transform.position                = ex_position;
    ramanujan::Quaternion orientation = GetOrientation();
    m_view                            = ramanujan::ToMatrix4(orientation) * ramanujan::ToMatrix4(transform);
    m_view                            = ramanujan::Inverse(m_view);
}

void EditorCameraSacha::MousePan(const ramanujan::Vector2& delta)
{
    ex_position = ex_position + ramanujan::Vector3(-delta.x * 0.05f, delta.y * 0.05f, 0.0f);
}

void EditorCameraSacha::MouseRotate(const ramanujan::Vector2& delta)
{
    ex_yaw   = ex_yaw + (-delta.x * RotationSpeed());
    ex_pitch = ex_pitch - (delta.y * RotationSpeed());
}

std::pair<float, float> EditorCameraSacha::PanSpeed() const
{
    float x        = std::min(m_viewport_width / 1000.0f, 2.4f);
    float x_factor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

    float y        = std::min(m_viewport_height / 1000.0f, 2.4f);
    float y_factor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

    return {x_factor, y_factor};
}

float EditorCameraSacha::RotationSpeed() const
{
    return m_rotation_speed;
}

} // namespace sputnik::graphics::api