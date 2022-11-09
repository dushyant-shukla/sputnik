#pragma once

#include "pch.h"
#include "graphics/core/geometry/vertex_attribute_data_type.h"

namespace sputnik::graphics::core
{

// Todo: Have default implementations for Vertex

// struct VertexAttribute
//{
//     std::string             m_name;
//     ui32                    m_offset{0};
//     ui32                    m_size{0};
//     VertexAttributeDataType m_type{VertexAttributeDataType::NONE};
//     bool                    m_normalized{false};
//
//     VertexAttribute();
//     VertexAttribute(const VertexAttributeDataType& type, const std::string& name, bool normalized = false);
// };
//
// class VertexBufferLayout
//{
//
// public:
//     VertexBufferLayout();
//     VertexBufferLayout(const std::initializer_list<VertexAttribute>& attributes);
//
//     void                                         AddAttribute(const VertexAttribute& attribute);
//     const std::vector<VertexAttribute>&          GetAttributes() const;
//     ui32                                         GetStride() const;
//     std::vector<VertexAttribute>::iterator       begin();
//     std::vector<VertexAttribute>::iterator       end();
//     std::vector<VertexAttribute>::const_iterator begin() const;
//     std::vector<VertexAttribute>::const_iterator end() const;
//
// private:
//     std::vector<VertexAttribute> m_attributes;
//     ui32                         m_stride;
// };

class VertexBuffer
{

public:
    VertexBuffer() = default;
    virtual ~VertexBuffer();

    virtual void                         Bind() const                                  = 0;
    virtual void                         Unbind() const                                = 0;
    virtual void                         UpdateData(void const* data, ui32 size) const = 0;
    static std::shared_ptr<VertexBuffer> CreateBuffer(ui32 size);
    static std::shared_ptr<VertexBuffer> CreateBuffer(float* data, ui32 size);

    // virtual void                         SetLayout(const VertexBufferLayout& layout) = 0;
    // virtual const VertexBufferLayout&    GetBufferLayout() const                     = 0;
};

} // namespace sputnik::graphics::core