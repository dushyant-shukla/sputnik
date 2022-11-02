#include "pch.h"
#include "editor_camera.h"

#include <transform.h>
#include <camera_transforms.h>
#include <quaternion.h>
#include <imgui.h>

namespace sputnik::graphics::api
{

EditorCamera::EditorCamera()
    : m_mouse_position_current({0, 0})
    , m_mouse_position_previous({0, 0})
    , m_rotation_speed(1.0f)
{
}

EditorCamera::~EditorCamera() {}

EditorCamera& EditorCamera::GetInstance()
{
    static EditorCamera instance;
    return instance;
}

void EditorCamera::Update()
{
    ImGuiIO& io      = ImGui::GetIO();
    bool     handled = io.WantCaptureMouse;
    if(!handled)
    {
        int32_t dx = m_mouse_position_current.x - m_mouse_position_previous.x;
        int32_t dy = m_mouse_position_current.y - m_mouse_position_previous.y;

        ramanujan::Transform transform;
        transform.position = m_position;

        m_rotation = m_rotation + ramanujan::Vector3(dy * m_rotation_speed, -dx * m_rotation_speed, 0.0f);

        ramanujan::Matrix4 rotation_x = ramanujan::ToMatrix4(ramanujan::AngleAxis(m_rotation.x, {1.0f, 0.0f, 0.0f}));
        ramanujan::Matrix4 rotation_y = ramanujan::ToMatrix4(ramanujan::AngleAxis(m_rotation.y, {0.0f, 1.0f, 0.0f}));
        ramanujan::Matrix4 rotation_z = ramanujan::ToMatrix4(ramanujan::AngleAxis(m_rotation.z, {0.0f, 0.0f, 1.0f}));
        ramanujan::Matrix4 rotation   = rotation_x * rotation_y * rotation_z;

        //m_view = ramanujan::ToMatrix4(transform) * rotation;
    }
    //m_mouse_position_previous = m_mouse_position_current;
}

void EditorCamera::UpdateMousePosition(int32_t x, int32_t y)
{
    m_mouse_position_previous = m_mouse_position_current;
    m_mouse_position_current = {x, y};
}

void EditorCamera::SetPosition(const ramanujan::Vector3& position)
{
    m_position = position;
}

void EditorCamera::SetRotation(const ramanujan::Vector3& rotation)
{
    m_rotation = rotation;
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

} // namespace sputnik::graphics::api