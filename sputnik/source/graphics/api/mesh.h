#pragma once

#include "pch.h"
#include "graphics/glcore/vertex_attribute.h"
#include "graphics/glcore/index_buffer.h"
#include "graphics/api/animation/skeleton.h"

#include <vector2.h>
#include <vector3.h>
#include <vector4.h>
#include <matrix4.h>

#include <memory>

namespace sputnik::api
{

/**
 * This is a naive implementation of a mesh construct. It is not production ready.
 */
class Mesh
{

public:
    // public interface

    Mesh();
    Mesh(const Mesh& other);
    ~Mesh();

    Mesh& operator=(const Mesh& other);

    std::vector<ramanujan::Vector3>&  GetPosition();
    std::vector<ramanujan::Vector3>&  GetNormal();
    std::vector<ramanujan::Vector2>&  GetTexCoord();
    std::vector<ramanujan::Vector4>&  GetWeights();
    std::vector<ramanujan::IVector4>& GetInfluences();
    std::vector<unsigned int>&        GetIndices();

    void CpuSkin(const animation::Skeleton& skeleton, const animation::Pose& pose);
    void CpuSkin(const std::vector<ramanujan::Matrix4>& skin_transform);
    void UpdateOpenglBuffers();
    void Bind(int position_slot, int normal_slot, int uv_slot, int weight_slot, int influence_slot);

    void Draw();
    void DrawInstanced(unsigned int num_instances);
    void Unbind(int position_slot, int normal_slot, int uv_slot, int weight_slot, int influence_slot);

protected:
    // cpu data
    std::vector<ramanujan::Vector3>  m_position;
    std::vector<ramanujan::Vector3>  m_normal;
    std::vector<ramanujan::Vector2>  m_uv;
    std::vector<ramanujan::Vector4>  m_weights;
    std::vector<ramanujan::IVector4> m_influences;
    std::vector<unsigned int>        m_indices;

    // vertex buffers : gpu data
    std::shared_ptr<sputnik::glcore::VertexAttribute<ramanujan::Vector3>>  m_position_attribute;
    std::shared_ptr<sputnik::glcore::VertexAttribute<ramanujan::Vector3>>  m_normal_attribute;
    std::shared_ptr<sputnik::glcore::VertexAttribute<ramanujan::Vector2>>  m_uv_attribute;
    std::shared_ptr<sputnik::glcore::VertexAttribute<ramanujan::Vector4>>  m_weight_attribute;
    std::shared_ptr<sputnik::glcore::VertexAttribute<ramanujan::IVector4>> m_influence_attribute;
    std::shared_ptr<sputnik::glcore::IndexBuffer>                          m_index_buffer;

    // additional copy of pose and normal data, and matrix palette is required for CPU skinning
    std::vector<ramanujan::Vector3> m_skinned_position;
    std::vector<ramanujan::Vector3> m_skinned_normal;
    std::vector<ramanujan::Matrix4> m_pose_palette;
};

} // namespace sputnik::api