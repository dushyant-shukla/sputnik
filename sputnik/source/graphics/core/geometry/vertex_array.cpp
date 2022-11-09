#include "pch.h"
#include "vertex_array.h"
#include "graphics/api/renderer.h"
#include "graphics/glcore/gl_vertex_array.h"

namespace sputnik::graphics::core
{

//////////////////////////////////////////// VERTEX ATTRIBUTE ////////////////////////////////////////////

VertexAttribute::VertexAttribute() {}

VertexAttribute::VertexAttribute(const VertexAttributeDataType& type, const std::string& name, bool normalized)
    : m_type(type)
    , m_name(name)
    , m_offset(0)
    , m_normalized(normalized)
    , m_size(GetAttributeDataTypeSize(type))
{
}

//////////////////////////////////////// VERTEX ARRAY LAYOUT ///////////////////////////////////////////

VertexArrayLayout::VertexArrayLayout() {}

VertexArrayLayout::VertexArrayLayout(const std::initializer_list<VertexAttribute>& attributes)
    : m_attributes(attributes)
{
    ui32 current_offset = 0;
    m_stride            = 0;
    for(auto& attribute : m_attributes)
    {
        attribute.m_offset = current_offset;
        m_stride += attribute.m_size;
        current_offset += attribute.m_size; // update offset for the next attribute
    }
}

void VertexArrayLayout::AddAttribute(const VertexAttribute& attribute)
{
    m_attributes.emplace_back(attribute);
}

const std::vector<VertexAttribute>& VertexArrayLayout::GetAttributes() const
{
    return m_attributes;
}

ui32 VertexArrayLayout::GetStride() const
{
    return m_stride;
}

std::vector<VertexAttribute>::iterator VertexArrayLayout::begin()
{
    return m_attributes.begin();
}

std::vector<VertexAttribute>::iterator VertexArrayLayout::end()
{
    return m_attributes.end();
}

std::vector<VertexAttribute>::const_iterator VertexArrayLayout::begin() const
{
    return m_attributes.begin();
}

std::vector<VertexAttribute>::const_iterator VertexArrayLayout::end() const
{
    return m_attributes.end();
}

///////////////////////////////////////////// VERTEX ARRAY /////////////////////////////////////////////

VertexArray::VertexArray() {}

VertexArray::~VertexArray() {}

// Todo:: default vertex attribute layout (to be done later)
std::shared_ptr<VertexArray> VertexArray::Create()
{
    std::shared_ptr<VertexArray> vertex_array = nullptr;
    switch(api::Renderer::GetCurrentGraphicsSubsystemType())
    {
    case GraphicsSubsystemType::OPENGL:
        vertex_array = std::make_shared<glcore::GlVertexArray>();
        break;
    default:
        break;
    }

    SPUTNIK_ASSERT(vertex_array != nullptr,
                   "Encounted invalid graphics subsustem type: {1}",
                   api::Renderer::GetCurrentGraphicsSubsystemType());

    return vertex_array;
}

std::shared_ptr<VertexArray> VertexArray::Create(const VertexArrayLayout& layout)
{
    std::shared_ptr<VertexArray> vertex_array = nullptr;
    switch(api::Renderer::GetCurrentGraphicsSubsystemType())
    {
    case GraphicsSubsystemType::OPENGL:
        vertex_array = std::make_shared<glcore::GlVertexArray>(layout);
        break;
    default:
        break;
    }

    SPUTNIK_ASSERT(vertex_array != nullptr,
                   "Encounted invalid graphics subsustem type: {1}",
                   api::Renderer::GetCurrentGraphicsSubsystemType());

    return vertex_array;
}

} // namespace sputnik::graphics::core