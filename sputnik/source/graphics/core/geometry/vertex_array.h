#pragma once

#include "pch.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "graphics/core/geometry/vertex_attribute_data_type.h"

namespace sputnik::graphics::core
{

struct VertexAttribute
{
    std::string             m_name;
    ui32                    m_offset{0};
    ui32                    m_size{0};
    VertexAttributeDataType m_type{VertexAttributeDataType::NONE};
    bool                    m_normalized{false};

    VertexAttribute();
    VertexAttribute(const VertexAttributeDataType& type, const std::string& name, bool normalized = false);
};

class VertexArrayLayout
{

public:
    VertexArrayLayout();
    VertexArrayLayout(const std::initializer_list<VertexAttribute>& attributes);

    void                                         AddAttribute(const VertexAttribute& attribute);
    const std::vector<VertexAttribute>&          GetAttributes() const;
    ui32                                         GetStride() const;
    std::vector<VertexAttribute>::iterator       begin();
    std::vector<VertexAttribute>::iterator       end();
    std::vector<VertexAttribute>::const_iterator begin() const;
    std::vector<VertexAttribute>::const_iterator end() const;

private:
    std::vector<VertexAttribute> m_attributes;
    ui32                         m_stride;
};

class VertexArray
{
public:
    VertexArray();
    virtual ~VertexArray();

    virtual void Bind() const                                                          = 0;
    virtual void Unbind() const                                                        = 0;
    virtual void AddVertexBuffer(std::shared_ptr<VertexBuffer>& vertexBuffer)          = 0;
    virtual void SetIndexBuffer(std::shared_ptr<IndexBuffer>& indexBuffer)             = 0;
    virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;
    virtual const std::shared_ptr<IndexBuffer>&               GetIndexBuffer() const   = 0;

    static std::shared_ptr<VertexArray> Create();
    static std::shared_ptr<VertexArray> Create(const VertexArrayLayout& layout);
};

} // namespace sputnik::graphics::core
