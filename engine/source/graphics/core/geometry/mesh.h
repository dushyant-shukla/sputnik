#pragma once

#include "pch.h"
#include "graphics/core/animation/skeleton.h"
#include "graphics/api/color_material.h"

#include <vector2.h>
#include <vector3.h>
#include <vector4.h>
#include <matrix4.h>

#include <memory>

namespace sputnik::graphics::gl
{
class OglVertexArray;
class OglBuffer;
} // namespace sputnik::graphics::gl

namespace sputnik::graphics::core
{

using namespace sputnik::graphics::gl;
using namespace sputnik::graphics::api;

/**
 * This is a naive implementation of a mesh construct. It is not production ready.
 */
class Mesh
{

public:
    // public interface

    Mesh();
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    Mesh(const Mesh&)            = delete;
    Mesh& operator=(const Mesh&) = delete;

    ~Mesh();

    std::vector<ramanujan::Vector3>&  GetPosition();
    std::vector<ramanujan::Vector3>&  GetNormal();
    std::vector<ramanujan::Vector2>&  GetTexCoord();
    std::vector<ramanujan::Vector4>&  GetWeights();
    std::vector<ramanujan::IVector4>& GetInfluences();
    std::vector<unsigned int>&        GetIndices();

    void CpuSkin(const Skeleton& skeleton, const Pose& pose);
    void CpuSkin(const std::vector<ramanujan::Matrix4>& skin_transform);
    void ResetOpenglBuffersToBindPose();
    // void Bind(int position_slot, int normal_slot, int uv_slot, int weight_slot, int influence_slot);

    void Draw();
    void draw(const Material& material, const mat4& model);
    void draw(const Material& material, const mat4& model, const std::vector<Matrix4>& skin_transformations);
    void drawInstanced(const Material& material, const u32& num_instances);
    void DrawInstanced(unsigned int num_instances);
    // void Unbind(int position_slot, int normal_slot, int uv_slot, int weight_slot, int influence_slot);

    void initializeGpuBuffers();

    std::shared_ptr<OglVertexArray> getVertexArray() const;

    void updatePositionBuffer(void* data, const u64& byte);
    void updateNormalBuffer(void* data, const u64& byte);

protected:
    // cpu data
    std::vector<ramanujan::Vector3>  m_position;
    std::vector<ramanujan::Vector3>  m_normal;
    std::vector<ramanujan::Vector2>  m_uv;
    std::vector<ramanujan::Vector4>  m_weights;
    std::vector<ramanujan::IVector4> m_influences;
    std::vector<unsigned int>        m_indices;

    // vertex buffers : gpu data
    // std::shared_ptr<glcore::VertexAttribute<ramanujan::Vector3>>  m_position_attribute;
    // std::shared_ptr<glcore::VertexAttribute<ramanujan::Vector3>>  m_normal_attribute;
    // std::shared_ptr<glcore::VertexAttribute<ramanujan::Vector2>>  m_uv_attribute;
    // std::shared_ptr<glcore::VertexAttribute<ramanujan::Vector4>>  m_weight_attribute;
    // std::shared_ptr<glcore::VertexAttribute<ramanujan::IVector4>> m_influence_attribute;
    // std::shared_ptr<glcore::IndexBuffer>                          m_index_buffer;

    // vertex buffers : gpu data
    std::shared_ptr<OglBuffer>      m_position_buffer;
    std::shared_ptr<OglBuffer>      m_normal_buffer;
    std::shared_ptr<OglBuffer>      m_uv_buffer;
    std::shared_ptr<OglBuffer>      m_weight_buffer;
    std::shared_ptr<OglBuffer>      m_influence_buffer;
    std::shared_ptr<OglBuffer>      m_index_buffer;
    std::shared_ptr<OglVertexArray> m_vertex_array;

    // additional copy of pose and normal data, and matrix palette is required for CPU skinning
    std::vector<ramanujan::Vector3> m_skinned_position;
    std::vector<ramanujan::Vector3> m_skinned_normal;
    std::vector<ramanujan::Matrix4> m_pose_palette;
};

} // namespace sputnik::graphics::core