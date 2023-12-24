#include "pch.h"

#include "gl_framebuffer.h"

#include <glad/glad.h>

namespace sputnik::graphics::gl
{

OglFramebuffer::OglFramebuffer(const FramebufferSpecification& spec) : m_id{0}, m_specification{spec}
{
    reset();
}

OglFramebuffer::~OglFramebuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &m_id);
}

OglFramebuffer::OglFramebuffer(OglFramebuffer&& other) noexcept : m_id{0}
{
    *this = std::move(other);
}

OglFramebuffer& OglFramebuffer::operator=(OglFramebuffer&& other) noexcept
{
    if(this != &other)
    {
        m_id       = other.m_id;
        other.m_id = 0;
    }
    return *this;
}

void OglFramebuffer::reset()
{
    // If we already have a framebuffer, delete it
    if(m_id != 0)
    {
        glDeleteFramebuffers(1, &m_id);
        m_depth_attachment.reset();
        m_color_attachments.clear();
        m_id = 0;
    }

    // recreate the framebuffer
    glCreateFramebuffers(1, &m_id);

    // Todo:: Not implemented yet.
    bool is_multisampled = m_specification.samples > 1;
    if(!m_specification.attachments.empty())
    {
        for(const auto& attachment_spec : m_specification.attachments)
        {
            switch(attachment_spec.texture_format)
            {
            case FramebufferTextureFormat::RGBA8:
            {
                attachColorAttachment(attachment_spec, TextureFormat::RGBA8);
                break;
            }
            case FramebufferTextureFormat::RedInteger:
            {
                attachColorAttachment(attachment_spec, TextureFormat::R32I);
                break;
            }
            case FramebufferTextureFormat::Depth24Stencil8:
            {
                m_depth_attachment = std::make_unique<OglTexture2D>(m_specification.width,
                                                                    m_specification.height,
                                                                    TextureFormat::Depth24Stencil8,
                                                                    TextureWrap::ClampToEdge,
                                                                    TextureWrap::ClampToEdge,
                                                                    TextureWrap::ClampToEdge,
                                                                    TextureFilter::Linear,
                                                                    TextureFilter::Linear);
                // texture->bind();
                glNamedFramebufferTexture(m_id, GL_DEPTH_STENCIL_ATTACHMENT, m_depth_attachment->getId(), 0);
                break;
            }
            default:
                SPUTNIK_ASSERT_MESSAGE(false,
                                       "Unknown framebuffer texture format: {}",
                                       static_cast<u32>(attachment_spec.texture_format));
                break;
            }
        }
    }

    if(m_color_attachments.size() > 1)
    {
        SPUTNIK_ASSERT(m_color_attachments.size() <= 4, "Framebuffer does not support more than 4 color attachments!");
        GLenum buffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
        glDrawBuffers((GLsizei)m_color_attachments.size(), buffers);
    }
    else if(m_color_attachments.empty())
    {
        // Only depth-pass
        glDrawBuffer(GL_NONE);
    }

    SPUTNIK_ASSERT(glCheckNamedFramebufferStatus(m_id, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
                   "Frame buffer is incomplete!");
}

void OglFramebuffer::resize(u32 width, u32 height)
{
    m_specification.width  = width;
    m_specification.height = height;
    reset();
}

void OglFramebuffer::clear(const vec4& rgba, const float& d, const int& s)
{
    // glBindFramebuffer(GL_FRAMEBUFFER, m_id);
    // glClearColor(rgba.x, rgba.y, rgba.z, rgba.w);
    // glClearDepth(d);
    // glClearStencil(s);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    for(size_t i = 0; i < m_color_attachments.size(); ++i)
    {
        glClearNamedFramebufferfv(m_id, GL_COLOR, (GLint)(GL_DRAW_BUFFER0 + i), static_cast<const f32*>(&rgba.data[0]));
    }
    glClearNamedFramebufferfi(m_id, GL_DEPTH_STENCIL, 0, d, s);
}

void OglFramebuffer::clear(const u32& color_attachment_index, const void* value)
{
    SPUTNIK_ASSERT_MESSAGE(color_attachment_index < m_color_attachments.size(),
                           "Unknown framebuffer texture format: {}",
                           color_attachment_index);
    switch(m_specification.attachments[color_attachment_index].texture_format)
    {
    case FramebufferTextureFormat::RGBA8:
    {
        glClearNamedFramebufferfv(m_id,
                                  GL_COLOR,
                                  GL_DRAW_BUFFER0 + color_attachment_index,
                                  static_cast<const f32*>(value));
        break;
    }
    case FramebufferTextureFormat::RedInteger:
    {
        glClearNamedFramebufferiv(m_id,
                                  GL_COLOR,
                                  GL_DRAW_BUFFER0 + color_attachment_index,
                                  static_cast<const i32*>(value));
        break;
    }
    default:
        SPUTNIK_ASSERT_MESSAGE(false,
                               "The attachment is not a color attachment. Attachment index: {}",
                               color_attachment_index);
    }
}

FramebufferSpecification& OglFramebuffer::getSpecification()
{
    return m_specification;
}

const FramebufferSpecification& OglFramebuffer::getSpecification() const
{
    return m_specification;
}

u32 OglFramebuffer::getColorAttachmentId(u32 index) const
{
    return m_color_attachments[index]->getId();
}

u32 OglFramebuffer::getColorAttachmentId(u32 index)
{
    return m_color_attachments[index]->getId();
}

u32 OglFramebuffer::getDepthAttachmentId() const
{
    return m_depth_attachment->getId();
}

void OglFramebuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);
    glViewport(0, 0, m_specification.width, m_specification.height);
}

void OglFramebuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OglFramebuffer::attachColorAttachment(const FramebufferAttachmentSpecification& attachment_spec,
                                           const TextureFormat&                      texture_format)
{
    std::unique_ptr<OglTexture2D> texture = std::make_unique<OglTexture2D>(m_specification.width,
                                                                           m_specification.height,
                                                                           texture_format,
                                                                           TextureWrap::ClampToEdge,
                                                                           TextureWrap::ClampToEdge,
                                                                           TextureWrap::ClampToEdge,
                                                                           TextureFilter::Linear,
                                                                           TextureFilter::Linear);
    // texture->bind();
    glNamedFramebufferTexture(m_id,
                              GL_COLOR_ATTACHMENT0 + static_cast<u32>(m_color_attachments.size()),
                              texture->getId(),
                              0);
    m_color_attachments.push_back(std::move(texture));
}

} // namespace sputnik::graphics::gl
