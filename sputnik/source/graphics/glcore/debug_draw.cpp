#include "pch.h"
#include "debug_draw.h"
#include "graphics/glcore/uniform.h"
#include "graphics/glcore/renderer.h"

sputnik::graphics::glcore::DebugDraw::DebugDraw()
{
    m_vertex_attributes = new VertexAttribute<ramanujan::Vector3>();

    std::string vertex_shader_source = R"(
        #version 460 core

        uniform mat4 mvp;
        in vec3 position;
        out vec4 position_color;
        void main()
        {
            position_color = vec4(abs(position.x), abs(position.y), abs(position.z), 1.0);
            //position_color = vec4(position, 1.0);
            gl_Position = mvp * vec4(position, 1.0);
        }
    )";

    std::string fragment_shader_source = R"(
        #version 460 core

        in vec4 position_color;
        uniform vec3 color;
        out vec4 FragColor;
        void main()
        {
            FragColor = vec4(color, 1);
            //FragColor = position_color;
        }
    )";

    m_shader = new Shader(vertex_shader_source, fragment_shader_source);
}

sputnik::graphics::glcore::DebugDraw::DebugDraw(unsigned int size)
{
    m_vertex_attributes = new VertexAttribute<ramanujan::Vector3>();

    std::string vertex_shader_source = R"(
        #version 460 core

        uniform mat4 mvp;
        in vec3 position;
        out vec4 position_color;
        void main()
        {
            position_color = vec4(position, 1.0);
            gl_Position = mvp * vec4(position, 1.0);
        }
    )";

    std::string fragment_shader_source = R"(
        #version 460 core

        in vec4 position_color;
        uniform vec3 color;
        out vec4 FragColor;
        void main()
        {
            FragColor = vec4(color, 1);
            //FragColor = position_color;
        }
    )";
    m_shader                           = new Shader(vertex_shader_source, fragment_shader_source);

    Resize(size);
}

sputnik::graphics::glcore::DebugDraw::~DebugDraw()
{
    delete m_shader;
    delete m_vertex_attributes;
}

unsigned int sputnik::graphics::glcore::DebugDraw::Size()
{
    return (unsigned int)m_points.size();
}

void sputnik::graphics::glcore::DebugDraw::Resize(unsigned int newSize)
{
    m_points.resize(newSize);
}

ramanujan::Vector3& sputnik::graphics::glcore::DebugDraw::operator[](unsigned int index)
{
    return m_points[index];
}

void sputnik::graphics::glcore::DebugDraw::Push(const ramanujan::Vector3& v)
{
    m_points.push_back(v);
}

void sputnik::graphics::glcore::DebugDraw::FromPose(const sputnik::graphics::core::Pose& pose)
{
    unsigned int required_verts = 0;
    size_t       num_joints     = pose.GetNumJoints();
    for(size_t i = 0; i < num_joints; ++i)
    {
        if(pose.GetParent(i) < 0)
        {
            continue;
        }

        required_verts += 2;
    }

    m_points.resize(required_verts);
    for(unsigned int i = 0; i < num_joints; ++i)
    {
        if(pose.GetParent(i) < 0)
        {
            continue;
        }

        m_points.push_back(pose.GetGlobalTransform(i).position);
        m_points.push_back(pose.GetGlobalTransform(pose.GetParent(i)).position);
    }
}

void sputnik::graphics::glcore::DebugDraw::LinesFromIKSolver(CCDSolver& solver)
{
    if(solver.Size() < 2)
    {
        return;
    }
    unsigned int requiredVerts = (solver.Size() - 1) * 2;
    m_points.resize(requiredVerts);

    unsigned int index = 0;
    for(unsigned int i = 0, size = solver.Size(); i < size - 1; ++i)
    {
        m_points[index++] = solver.GetGlobalTransform(i).position;
        m_points[index++] = solver.GetGlobalTransform(i + 1).position;
    }
}

void sputnik::graphics::glcore::DebugDraw::PointsFromIKSolver(CCDSolver& solver)
{
    unsigned int requiredVerts = solver.Size();
    m_points.resize(requiredVerts);

    for(unsigned int i = 0, size = solver.Size(); i < size; ++i)
    {
        m_points[i] = solver.GetGlobalTransform(i).position;
    }
}

void sputnik::graphics::glcore::DebugDraw::LinesFromIKSolver(FabrikSolver& solver)
{
    if(solver.Size() < 2)
    {
        return;
    }
    unsigned int requiredVerts = (solver.Size() - 1) * 2;
    m_points.resize(requiredVerts);

    unsigned int index = 0;
    for(unsigned int i = 0, size = solver.Size(); i < size - 1; ++i)
    {
        m_points[index++] = solver.GetGlobalTransform(i).position;
        m_points[index++] = solver.GetGlobalTransform(i + 1).position;
    }
}

void sputnik::graphics::glcore::DebugDraw::PointsFromIKSolver(FabrikSolver& solver)
{
    unsigned int requiredVerts = solver.Size();
    m_points.resize(requiredVerts);

    for(unsigned int i = 0, size = solver.Size(); i < size; ++i)
    {
        m_points[i] = solver.GetGlobalTransform(i).position;
    }
}

void sputnik::graphics::glcore::DebugDraw::UpdateOpenGLBuffers()
{
    m_vertex_attributes->Set(m_points);
}

void sputnik::graphics::glcore::DebugDraw::Draw(DebugDrawMode             mode,
                                                const ramanujan::Vector3& color,
                                                const ramanujan::Matrix4& mvp)
{
    m_shader->Bind();
    sputnik::graphics::glcore::Uniform<ramanujan::Matrix4>::Set(m_shader->GetUniform("mvp"), mvp);
    sputnik::graphics::glcore::Uniform<ramanujan::Vector3>::Set(m_shader->GetUniform("color"), color);
    m_vertex_attributes->Bind(m_shader->GetAttribute("position"));
    if(mode == DebugDrawMode::Lines)
    {
        sputnik::graphics::glcore::Renderer::Draw(Size(), sputnik::graphics::glcore::DrawMode::LINES);
    }
    else if(mode == DebugDrawMode::Loop)
    {
        sputnik::graphics::glcore::Renderer::Draw(Size(), sputnik::graphics::glcore::DrawMode::LINE_LOOP);
    }
    else if(mode == DebugDrawMode::Strip)
    {
        sputnik::graphics::glcore::Renderer::Draw(Size(), sputnik::graphics::glcore::DrawMode::LINE_STRIP);
    }
    else
    {
        sputnik::graphics::glcore::Renderer::Draw(Size(), sputnik::graphics::glcore::DrawMode::POINTS);
    }
    m_vertex_attributes->Unbind(m_shader->GetAttribute("position"));
    m_shader->Unbind();
}
