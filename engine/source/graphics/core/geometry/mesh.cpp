#include "pch.h"
#include "mesh.h"
#include <graphics/glcore/gl_buffer.h>
#include <graphics/glcore/gl_vertex_array.h>
#include <graphics/glcore/gl_renderer.h>

namespace sputnik::graphics::core
{

using namespace sputnik::graphics::gl;

Mesh::Mesh() {}

Mesh::Mesh(Mesh&& other) noexcept
{
    *this = std::move(other);
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    if(this != &other)
    {
        m_index_buffer     = std::move(other.m_index_buffer);
        m_position_buffer  = std::move(other.m_position_buffer);
        m_normal_buffer    = std::move(other.m_normal_buffer);
        m_uv_buffer        = std::move(other.m_uv_buffer);
        m_weight_buffer    = std::move(other.m_weight_buffer);
        m_influence_buffer = std::move(other.m_influence_buffer);
        m_vertex_array     = std::move(other.m_vertex_array);
        m_position         = std::move(other.m_position);
        m_normal           = std::move(other.m_normal);
        m_uv               = std::move(other.m_uv);
        m_weights          = std::move(other.m_weights);
        m_influences       = std::move(other.m_influences);
        m_indices          = std::move(other.m_indices);
    }
    return *this;
}

Mesh::~Mesh() {}

std::vector<ramanujan::Vector3>& Mesh::GetPosition()
{
    return m_position;
}

std::vector<ramanujan::Vector3>& Mesh::GetNormal()
{
    return m_normal;
}

std::vector<ramanujan::Vector2>& Mesh::GetTexCoord()
{
    return m_uv;
}

std::vector<ramanujan::Vector4>& Mesh::GetWeights()
{
    return m_weights;
}

std::vector<ramanujan::IVector4>& Mesh::GetInfluences()
{
    return m_influences;
}

std::vector<unsigned int>& Mesh::GetIndices()
{
    return m_indices;
}

void Mesh::CpuSkin(const Skeleton& skeleton, const Pose& pose)
{
    size_t num_vertices = m_position.size();
    if(num_vertices == 0)
    {
        return;
    }

    m_skinned_position.resize(num_vertices);
    m_skinned_normal.resize(num_vertices);
    // const sputnik::api::animation::Pose& bind_pose = skeleton.GetBindPose();
    const std::vector<ramanujan::Matrix4>& inv_bind_pose = skeleton.GetInverseBindPose();

    for(size_t vertex_index = 0; vertex_index < num_vertices; ++vertex_index)
    {
        // The x,y,z,w components of a influencers are indices in the animated pose and inverse bind pose matrix arrays.
        ramanujan::IVector4& joint = m_influences[vertex_index];

        // The x,y,z,w components of influencer weight are the scalar weights to apply to the same components of the
        // influencers.
        ramanujan::Vector4& weight = m_weights[vertex_index];

        // Each vertex has to be skinned four times, once for every joint that affects the vertex (max 4 joints per
        // vertex). A skin transform is the combination of the inverse bind pose and the pose transformations.

        // Skin transform#1
        // ramanujan::Transform skin_0 = ramanujan::Combine(pose[joint.x], ramanujan::Inverse(bind_pose[joint.x]));
        ramanujan::Transform skin_0 = ramanujan::Combine(pose[joint.x], ramanujan::ToTransform(inv_bind_pose[joint.x]));
        ramanujan::Vector3   p_0    = ramanujan::TransformPoint(skin_0, m_position[vertex_index]);
        ramanujan::Vector3   n_0    = ramanujan::TransformVector(skin_0, m_normal[vertex_index]);

        // Skin transform#2
        // ramanujan::Transform skin_1 = ramanujan::Combine(pose[joint.y], ramanujan::Inverse(bind_pose[joint.y]));
        ramanujan::Transform skin_1 = ramanujan::Combine(pose[joint.y], ramanujan::ToTransform(inv_bind_pose[joint.y]));
        ramanujan::Vector3   p_1    = ramanujan::TransformPoint(skin_1, m_position[vertex_index]);
        ramanujan::Vector3   n_1    = ramanujan::TransformVector(skin_1, m_normal[vertex_index]);

        // Skin transform#3
        // ramanujan::Transform skin_2 = ramanujan::Combine(pose[joint.z], ramanujan::Inverse(bind_pose[joint.z]));
        ramanujan::Transform skin_2 = ramanujan::Combine(pose[joint.z], ramanujan::ToTransform(inv_bind_pose[joint.z]));
        ramanujan::Vector3   p_2    = ramanujan::TransformPoint(skin_2, m_position[vertex_index]);
        ramanujan::Vector3   n_2    = ramanujan::TransformVector(skin_2, m_normal[vertex_index]);

        // Skin transform#4
        // ramanujan::Transform skin_3 = ramanujan::Combine(pose[joint.w], ramanujan::Inverse(bind_pose[joint.w]));
        ramanujan::Transform skin_3 = ramanujan::Combine(pose[joint.w], ramanujan::ToTransform(inv_bind_pose[joint.w]));
        ramanujan::Vector3   p_3    = ramanujan::TransformPoint(skin_3, m_position[vertex_index]);
        ramanujan::Vector3   n_3    = ramanujan::TransformVector(skin_3, m_normal[vertex_index]);

        // Combine the above transforms for final vertex positions and normals
        m_skinned_position[vertex_index] = p_0 * weight.x + p_1 * weight.y + p_2 * weight.z + p_3 * weight.w;
        m_skinned_normal[vertex_index]   = n_0 * weight.x + n_1 * weight.y + n_2 * weight.z + n_3 * weight.w;
    }

    // update the gpu buffers
    m_position_buffer->setData((float*)&m_skinned_position, (u64)m_skinned_position.size() * sizeof(float));
    m_normal_buffer->setData((float*)&m_skinned_normal, (u64)m_skinned_normal.size() * sizeof(float));
}

void Mesh::CpuSkin(const std::vector<ramanujan::Matrix4>& skin_transform)
{
    size_t num_vertices = m_position.size();
    if(num_vertices == 0)
    {
        return;
    }

    m_skinned_position.resize(num_vertices);
    m_skinned_normal.resize(num_vertices);

    for(size_t vertex_index = 0; vertex_index < num_vertices; ++vertex_index)
    {
        // The x,y,z,w components of a influencers are indices in the animated pose and inverse bind pose matrix arrays.
        ramanujan::IVector4& joint = m_influences[vertex_index];

        // The x,y,z,w components of influencer weight are the scalar weights to apply to the same components of the
        // influencers.
        ramanujan::Vector4& weight = m_weights[vertex_index];

        ramanujan::Vector3 p_0 = ramanujan::TransformPoint(skin_transform[joint.x], m_position[vertex_index]);
        ramanujan::Vector3 n_0 = ramanujan::TransformVector(skin_transform[joint.x], m_normal[vertex_index]);

        ramanujan::Vector3 p_1 = ramanujan::TransformPoint(skin_transform[joint.y], m_position[vertex_index]);
        ramanujan::Vector3 n_1 = ramanujan::TransformVector(skin_transform[joint.y], m_normal[vertex_index]);

        ramanujan::Vector3 p_2 = ramanujan::TransformPoint(skin_transform[joint.z], m_position[vertex_index]);
        ramanujan::Vector3 n_2 = ramanujan::TransformVector(skin_transform[joint.z], m_normal[vertex_index]);

        ramanujan::Vector3 p_3 = ramanujan::TransformPoint(skin_transform[joint.w], m_position[vertex_index]);
        ramanujan::Vector3 n_3 = ramanujan::TransformVector(skin_transform[joint.w], m_normal[vertex_index]);

        // Combine the above transforms for final vertex positions and normals
        m_skinned_position[vertex_index] = p_0 * weight.x + p_1 * weight.y + p_2 * weight.z + p_3 * weight.w;
        m_skinned_normal[vertex_index]   = n_0 * weight.x + n_1 * weight.y + n_2 * weight.z + n_3 * weight.w;
    }

    // update the gpu buffers
    m_position_buffer->setData((float*)&m_skinned_position, (u64)m_skinned_position.size() * sizeof(float));
    m_normal_buffer->setData((float*)&m_skinned_normal, (u64)m_skinned_normal.size() * sizeof(float));
}

void Mesh::ResetOpenglBuffersToBindPose()
{
    // Update data for the buffers here
    if(m_position.size() > 0)
    {
        m_position_buffer->setData(&m_position[0], m_position.size() * sizeof(ramanujan::Vector3));
    }
    if(m_normal.size() > 0)
    {
        m_normal_buffer->setData(&m_normal[0], m_normal.size() * sizeof(ramanujan::Vector3));
    }
    if(m_uv.size() > 0)
    {
        m_uv_buffer->setData(&m_uv[0], m_uv.size() * sizeof(ramanujan::Vector2));
    }
    if(m_weights.size() > 0)
    {
        m_weight_buffer->setData(&m_weights[0], (u64)m_weights.size() * sizeof(ramanujan::Vector4));
    }
    if(m_influences.size() > 0)
    {
        m_influence_buffer->setData(&m_influences[0], (u64)m_influences.size() * sizeof(ramanujan::IVector4));
    }
    if(m_indices.size() > 0)
    {
        m_index_buffer->setData(&m_indices[0], m_indices.size() * sizeof(unsigned int));
    }
}

void Mesh::Draw()
{
    m_vertex_array->bind();
    if(m_indices.size() > 0)
    {
        OglRenderer::drawElements((u64)m_indices.size(), DrawMode::TRIANGLES);
        // glcore::Renderer::DrawElements((u64)m_indices.size(), glcore::DrawMode::TRIANGLES);
    }
    else
    {
        OglRenderer::drawArrays((u64)m_position.size(), DrawMode::TRIANGLES);
        // glcore::Renderer::DrawArrays(static_cast<unsigned int>(m_position.size()), glcore::DrawMode::TRIANGLES);
    }
    m_vertex_array->unbind();
}

void Mesh::DrawInstanced(unsigned int num_instances)
{
    m_vertex_array->bind();
    if(m_indices.size() > 0)
    {
        OglRenderer::drawElementsInstanced((u64)m_indices.size(), num_instances, DrawMode::TRIANGLES);
        // glcore::Renderer::DrawElementsInstanced((u64)m_indices.size(), num_instances, glcore::DrawMode::TRIANGLES);
    }
    else
    {
        OglRenderer::drawArraysInstanced((u64)m_position.size(), num_instances, DrawMode::TRIANGLES);
        // glcore::Renderer::DrawArraysInstanced(static_cast<unsigned int>(m_position.size()),
        //                                       num_instances,
        //                                       glcore::DrawMode::TRIANGLES);
    }
    m_vertex_array->unbind();
}

void Mesh::initializeGpuBuffers()
{
    m_vertex_array = std::make_shared<OglVertexArray>();

    if(m_position.size() > 0)
    {
        m_position_buffer = std::make_shared<OglBuffer>(&m_position[0], m_position.size() * sizeof(ramanujan::Vector3));
        m_vertex_array->addVertexBuffer(
            *m_position_buffer.get(),
            {.binding_index = 0, .stride = 12},
            {{.name = "position", .location = 0, .type = VertexAttributeType::Float3, .normalized = false}});
    }

    if(m_normal.size() > 0)
    {
        m_normal_buffer = std::make_shared<OglBuffer>(&m_normal[0], m_normal.size() * sizeof(ramanujan::Vector3));
        m_vertex_array->addVertexBuffer(
            *m_normal_buffer.get(),
            {.binding_index = 1, .stride = 12},
            {{.name = "normal", .location = 1, .type = VertexAttributeType::Float3, .normalized = false}});
    }

    if(m_uv.size() > 0)
    {
        m_uv_buffer = std::make_shared<OglBuffer>(&m_uv[0], m_uv.size() * sizeof(ramanujan::Vector2));
        m_vertex_array->addVertexBuffer(
            *m_uv_buffer.get(),
            {.binding_index = 2, .stride = 8},
            {{.name = "uv", .location = 2, .type = VertexAttributeType::Float2, .normalized = false}});
    }

    if(m_weights.size() > 0)
    {
        m_weight_buffer =
            std::make_shared<OglBuffer>(&m_weights[0], (u64)m_weights.size() * sizeof(ramanujan::Vector4));
        m_vertex_array->addVertexBuffer(
            *m_weight_buffer.get(),
            {.binding_index = 3, .stride = 16},
            {{.name = "weights", .location = 3, .type = VertexAttributeType::Float4, .normalized = false}});
    }

    if(m_influences.size() > 0)
    {
        m_influence_buffer =
            std::make_shared<OglBuffer>(&m_influences[0], (u64)m_influences.size() * sizeof(ramanujan::IVector4));
        m_vertex_array->addVertexBuffer(
            *m_influence_buffer.get(),
            {.binding_index = 4, .stride = 16},
            {{.name = "joints", .location = 4, .type = VertexAttributeType::Int4, .normalized = false}});
    }

    if(m_indices.size() > 0)
    {
        m_index_buffer = std::make_shared<OglBuffer>(&m_indices[0], m_indices.size() * sizeof(unsigned int));
        m_vertex_array->setIndexBuffer(*m_index_buffer.get());
    }
}

} // namespace sputnik::graphics::core