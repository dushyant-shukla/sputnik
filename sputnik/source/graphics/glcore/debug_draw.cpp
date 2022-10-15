#include "pch.h"
#include "debug_draw.h"
#include "graphics/glcore/uniform.h"
#include "graphics/api/renderer.h"

sputnik::glcore::DebugDraw::DebugDraw()
{
    m_vertex_attributes = new VertexAttribute<ramanujan::Vector3>();

    m_shader = new Shader("#version 330 core\n"
                          "uniform mat4 mvp;\n"
                          "in vec3 position;\n"
                          "void main() {\n"
                          "	gl_Position = mvp * vec4(position, 1.0);\n"
                          "}",
                          "#version 330 core\n"
                          "uniform vec3 color;\n"
                          "out vec4 FragColor;\n"
                          "void main() {\n"
                          "	FragColor = vec4(color, 1);\n"
                          "}");
}

sputnik::glcore::DebugDraw::DebugDraw(unsigned int size)
{
    m_vertex_attributes = new VertexAttribute<ramanujan::Vector3>();

    m_shader = new Shader("#version 330 core\n"
                          "uniform mat4 mvp;\n"
                          "in vec3 position;\n"
                          "void main() {\n"
                          "	gl_Position = mvp * vec4(position, 1.0);\n"
                          "}",
                          "#version 330 core\n"
                          "uniform vec3 color;\n"
                          "out vec4 FragColor;\n"
                          "void main() {\n"
                          "	FragColor = vec4(color, 1);\n"
                          "}");

    Resize(size);
}

sputnik::glcore::DebugDraw::~DebugDraw()
{
    delete m_shader;
    delete m_vertex_attributes;
}

unsigned int sputnik::glcore::DebugDraw::Size()
{
    return (unsigned int)m_points.size();
}

void sputnik::glcore::DebugDraw::Resize(unsigned int newSize)
{
    m_points.resize(newSize);
}

ramanujan::Vector3& sputnik::glcore::DebugDraw::operator[](unsigned int index)
{
    return m_points[index];
}

void sputnik::glcore::DebugDraw::Push(const ramanujan::Vector3& v)
{
    m_points.push_back(v);
}

void sputnik::glcore::DebugDraw::FromPose(animation::Pose& pose)
{
    unsigned int requiredVerts = 0;
    unsigned int numJoints     = pose.GetNumJoints();
    for(unsigned int i = 0; i < numJoints; ++i)
    {
        if(pose.GetParent(i) < 0)
        {
            continue;
        }

        requiredVerts += 2;
    }

    m_points.resize(requiredVerts);
    for(unsigned int i = 0; i < numJoints; ++i)
    {
        if(pose.GetParent(i) < 0)
        {
            continue;
        }

        m_points.push_back(pose.GetGlobalTransform(i).position);
        m_points.push_back(pose.GetGlobalTransform(pose.GetParent(i)).position);
    }
}

void sputnik::glcore::DebugDraw::UpdateOpenGLBuffers()
{
    m_vertex_attributes->Set(m_points);
}

void sputnik::glcore::DebugDraw::Draw(DebugDrawMode             mode,
                                      const ramanujan::Vector3& color,
                                      const ramanujan::Matrix4& mvp)
{
    m_shader->Bind();
    sputnik::glcore::Uniform<ramanujan::Matrix4>::Set(m_shader->GetUniform("mvp"), mvp);
    sputnik::glcore::Uniform<ramanujan::Vector3>::Set(m_shader->GetUniform("color"), color);
    m_vertex_attributes->Bind(m_shader->GetAttribute("position"));
    if(mode == DebugDrawMode::Lines)
    {
        sputnik::glcore::Renderer::Draw(Size(), sputnik::glcore::DrawMode::LINES);
    }
    else if(mode == DebugDrawMode::Loop)
    {
        sputnik::glcore::Renderer::Draw(Size(), sputnik::glcore::DrawMode::LINE_LOOP);
    }
    else if(mode == DebugDrawMode::Strip)
    {
        sputnik::glcore::Renderer::Draw(Size(), sputnik::glcore::DrawMode::LINE_STRIP);
    }
    else
    {
        sputnik::glcore::Renderer::Draw(Size(), sputnik::glcore::DrawMode::POINTS);
    }
    m_vertex_attributes->Unbind(m_shader->GetAttribute("position"));
    m_shader->Unbind();
}
