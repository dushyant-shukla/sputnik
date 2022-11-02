#pragma once

#include "core/core.h"

#include <stdint.h>
#include <vector3.h>
#include <vector2.h>
#include <matrix4.h>

namespace sputnik::graphics::api
{

class EditorCamera
{

public:

    NON_COPYABLE(EditorCamera);

    ~EditorCamera();

    static EditorCamera& GetInstance();

    void Update();

    void UpdateMousePosition(int32_t x, int32_t y);

    void SetPosition(const ramanujan::Vector3& position);
    void SetRotation(const ramanujan::Vector3& rotation);
    void SetPerspective(float fov, float aspect, float znear, float zfar);

    const ramanujan::Matrix4& GetCameraView();
    const ramanujan::Matrix4& GetCameraPerspective();
    const ramanujan::Vector3& GetCameraPosition();


private:
    EditorCamera();

private:
    ramanujan::Vector3 m_position;
    ramanujan::Vector3 m_rotation;
    ramanujan::Matrix4 m_projection;
    ramanujan::Matrix4 m_view;
    ramanujan::IVector2 m_mouse_position_current;
    ramanujan::IVector2 m_mouse_position_previous;
    float               m_rotation_speed;
};

} // namespace sputnik::graphics::api