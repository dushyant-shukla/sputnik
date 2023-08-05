#pragma once

#include "pch.h"
#include "core/core.h"
#include "graphics/core/animation/pose.h"
#include "graphics/glcore/vertex_attribute.h"
#include "graphics/glcore/shader.h"
#include "graphics/core/animation/ccd_solver.h"
#include "graphics/core/animation/fabrik_solver.h"

#include <vector3.h>
#include <matrix4.h>

namespace sputnik::graphics::glcore
{

using namespace sputnik::graphics::core;
using namespace ramanujan;

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

    unsigned int Size();
    void         Resize(unsigned int newSize);
    Vector3&     operator[](unsigned int index);
    void         Push(const Vector3& v);

    void FromPose(const Pose& pose);
    void LinesFromIKSolver(CCDSolver& solver);
    void PointsFromIKSolver(CCDSolver& solver);
    void LinesFromIKSolver(FabrikSolver& solver);
    void PointsFromIKSolver(FabrikSolver& solver);

    void UpdateOpenGLBuffers();
    void Draw(DebugDrawMode mode, const Vector3& color, const Matrix4& mvp);

protected:
    std::vector<Vector3>      m_points;
    VertexAttribute<Vector3>* m_vertex_attributes;
    Shader*                   m_shader;
};

} // namespace sputnik::graphics::glcore