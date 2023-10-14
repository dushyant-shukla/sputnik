#include "pch.h"
#include "mesh.h"
#include "graphics/glcore/renderer.h"

namespace sputnik::graphics::core
{

Mesh::Mesh()
{
    m_position_attribute  = std::make_shared<glcore::VertexAttribute<ramanujan::Vector3>>();
    m_normal_attribute    = std::make_shared<glcore::VertexAttribute<ramanujan::Vector3>>();
    m_uv_attribute        = std::make_shared<glcore::VertexAttribute<ramanujan::Vector2>>();
    m_weight_attribute    = std::make_shared<glcore::VertexAttribute<ramanujan::Vector4>>();
    m_influence_attribute = std::make_shared<glcore::VertexAttribute<ramanujan::IVector4>>();
    m_index_buffer        = std::make_shared<glcore::IndexBuffer>();
}

Mesh::Mesh(const Mesh& other)
{
    m_position_attribute  = std::make_shared<glcore::VertexAttribute<ramanujan::Vector3>>();
    m_normal_attribute    = std::make_shared<glcore::VertexAttribute<ramanujan::Vector3>>();
    m_uv_attribute        = std::make_shared<glcore::VertexAttribute<ramanujan::Vector2>>();
    m_weight_attribute    = std::make_shared<glcore::VertexAttribute<ramanujan::Vector4>>();
    m_influence_attribute = std::make_shared<glcore::VertexAttribute<ramanujan::IVector4>>();
    m_index_buffer        = std::make_shared<glcore::IndexBuffer>();
    *this                 = other;
}

Mesh::~Mesh() {}

Mesh& Mesh::operator=(const Mesh& other)
{
    if(this == &other)
    {
        return *this;
    }

    m_position   = other.m_position;
    m_normal     = other.m_normal;
    m_uv         = other.m_uv;
    m_weights    = other.m_weights;
    m_influences = other.m_influences;
    m_indices    = other.m_indices;
    ResetOpenglBuffersToBindPose();
    return *this;
}

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
    m_position_attribute->Set(m_skinned_position);
    m_normal_attribute->Set(m_skinned_normal);
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
    m_position_attribute->Set(m_skinned_position);
    m_normal_attribute->Set(m_skinned_normal);
}

void Mesh::ResetOpenglBuffersToBindPose()
{
    if(m_position.size() > 0)
    {
        m_position_attribute->Set(m_position);
    }
    if(m_normal.size() > 0)
    {
        m_normal_attribute->Set(m_normal);
    }
    if(m_uv.size() > 0)
    {
        m_uv_attribute->Set(m_uv);
    }
    if(m_weights.size() > 0)
    {
        m_weight_attribute->Set(m_weights);
    }
    if(m_influences.size() > 0)
    {
        m_influence_attribute->Set(m_influences);
    }
    if(m_indices.size() > 0)
    {
        m_index_buffer->Set(m_indices);
    }
}

void Mesh::Bind(int position_slot, int normal_slot, int uv_slot, int weight_slot, int influence_slot)
{
    if(position_slot >= 0)
    {
        m_position_attribute->Bind(position_slot);
    }
    if(normal_slot >= 0)
    {
        m_normal_attribute->Bind(normal_slot);
    }
    if(uv_slot >= 0)
    {
        m_uv_attribute->Bind(uv_slot);
    }
    if(weight_slot >= 0)
    {
        m_weight_attribute->Bind(weight_slot);
    }
    if(influence_slot >= 0)
    {
        m_influence_attribute->Bind(influence_slot);
    }
}

void Mesh::Draw()
{
    if(m_indices.size() > 0)
    {
        glcore::Renderer::Draw(*m_index_buffer, glcore::DrawMode::TRIANGLES);
    }
    else
    {
        glcore::Renderer::Draw(static_cast<unsigned int>(m_position.size()), glcore::DrawMode::TRIANGLES);
    }
}

void Mesh::DrawInstanced(unsigned int num_instances)
{
    if(m_indices.size() > 0)
    {
        glcore::Renderer::DrawInstanced(*m_index_buffer, glcore::DrawMode::TRIANGLES, num_instances);
    }
    else
    {
        glcore::Renderer::DrawInstanced(static_cast<unsigned int>(m_position.size()),
                                        glcore::DrawMode::TRIANGLES,
                                        num_instances);
    }
}

void Mesh::Unbind(int position_slot, int normal_slot, int uv_slot, int weight_slot, int influence_slot)
{
    if(position_slot >= 0)
    {
        m_position_attribute->Unbind(position_slot);
    }
    if(normal_slot >= 0)
    {
        m_normal_attribute->Unbind(normal_slot);
    }
    if(uv_slot >= 0)
    {
        m_uv_attribute->Unbind(uv_slot);
    }
    if(weight_slot >= 0)
    {
        m_weight_attribute->Unbind(weight_slot);
    }
    if(influence_slot >= 0)
    {
        m_influence_attribute->Unbind(influence_slot);
    }
}

} // namespace sputnik::graphics::core