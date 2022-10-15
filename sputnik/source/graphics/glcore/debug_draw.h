#pragma once

#include "pch.h"
#include "core/core.h"
#include "graphics/api/animation/pose.h"
#include "graphics/glcore/vertex_attribute.h"
#include "graphics/glcore/shader.h"

#include <vector3.h>
#include <matrix4.h>

namespace sputnik
{
namespace glcore
{
enum class DebugDrawMode
{
    Lines,
    Loop,
    Strip,
    Points
};

class DebugDraw
{

public:
    NON_COPYABLE(DebugDraw);

    DebugDraw();
    DebugDraw(unsigned int size);
    ~DebugDraw();

    unsigned int        Size();
    void                Resize(unsigned int newSize);
    ramanujan::Vector3& operator[](unsigned int index);
    void                Push(const ramanujan::Vector3& v);

    void FromPose(api::animation::Pose& pose);

    void UpdateOpenGLBuffers();
    void Draw(DebugDrawMode mode, const ramanujan::Vector3& color, const ramanujan::Matrix4& mvp);

protected:
    std::vector<ramanujan::Vector3>      m_points;
    VertexAttribute<ramanujan::Vector3>* m_vertex_attributes;
    Shader*                              m_shader;
};
} // namespace glcore
} // namespace sputnik