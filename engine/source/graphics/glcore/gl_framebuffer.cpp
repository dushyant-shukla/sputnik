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
            switch(attachment_spec.attachment_format)
            {
            case TextureFormat::RGBA8:
            case TextureFormat::R32I:
            {
                attachColorAttachment(attachment_spec);
                break;
            }
            case TextureFormat::Depth24Stencil8:
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
            case TextureFormat::Depth32F:
            {
                m_depth_attachment = std::make_unique<OglTexture2D>(m_specification.width,
                                                                    m_specification.height,
                                                                    TextureFormat::Depth32F,
                                                                    TextureWrap::ClampToBorder,
                                                                    TextureWrap::ClampToBorder,
                                                                    TextureWrap::ClampToBorder,
                                                                    TextureFilter::Nearest,
                                                                    TextureFilter::Nearest);
                // texture->bind();
                glNamedFramebufferTexture(m_id, GL_DEPTH_ATTACHMENT, m_depth_attachment->getId(), 0);
                break;
            }
            default:
                SPUTNIK_ASSERT_MESSAGE(false,
                                       "Unknown framebuffer texture format: {}",
                                       static_cast<u32>(attachment_spec.attachment_format));
                break;
            }
        }
    }

    if(m_color_attachments.size() > 1)
    {
        SPUTNIK_ASSERT(m_color_attachments.size() <= 4, "Framebuffer does not support more than 4 color attachments!");
        GLenum buffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
        // glDrawBuffers((GLsizei)m_color_attachments.size(), buffers);
        glNamedFramebufferDrawBuffers(m_id, (GLsizei)m_color_attachments.size(), buffers);
    }
    else if(m_color_attachments.empty())
    {
        // Only depth-pass
        // glBindFramebuffer(GL_FRAMEBUFFER, m_id);
        // glDrawBuffer(GL_NONE);
        // glReadBuffer(GL_NONE);
        // glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glNamedFramebufferDrawBuffer(m_id, GL_NONE);
        glNamedFramebufferReadBuffer(m_id, GL_NONE);
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

void OglFramebuffer::clear(const vec4& rgba)
{
    // Reference: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glClearDepth.xhtml

    // Non DSA method for clearing framebuffer
    //{
    //    glBindFramebuffer(GL_FRAMEBUFFER, m_id);
    //    glClearColor(rgba.x, rgba.y, rgba.z, rgba.w);
    //    glClearDepth(1);
    //    glClearStencil(1);
    //    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    //}

    // Todo:: For some reason with DSA need to clear color and depth attachments simulataneously.
    // Clearing just the color attachments does not work (viewport does not render anything).
    // Just to be clear, clearing a color attachment does clear the attachment to the clear color.
    for(size_t i = 0; i < m_color_attachments.size(); ++i)
    {
        // does not work
        // glClearNamedFramebufferfv(m_id, GL_COLOR, GL_DRAW_BUFFER0 + (GLint)(i), &rgba.x);
        glClearNamedFramebufferfv(m_id, GL_COLOR, (GLint)(i), &rgba.x);
    }
    glClearNamedFramebufferfi(m_id, GL_DEPTH_STENCIL, 0, 1, 1);

    // This seems to clear the color attachment to the color provided, but causes the viewport to not show anything
    // glClearNamedFramebufferfv(m_id, GL_COLOR, 0, rgba.data.data());
}

void OglFramebuffer::clear(const vec4& rgba, const float& depth_value, const int& stencil_value)
{
    // Reference: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glClearBuffer.xhtml

    // Non DSA method for clearing framebuffer
    //{
    //    glBindFramebuffer(GL_FRAMEBUFFER, m_id);
    //    glClearColor(rgba.x, rgba.y, rgba.z, rgba.w);
    //    glClearDepth(depth_value);
    //    glClearStencil(stencil_value);
    //    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    //}

    for(size_t i = 0; i < m_color_attachments.size(); ++i)
    {
        // does not work
        // glClearNamedFramebufferfv(m_id, GL_COLOR, GL_DRAW_BUFFER0 + (GLint)(i), &rgba.x);
        glClearNamedFramebufferfv(m_id, GL_COLOR, (GLint)(i), &rgba.x);
    }
    glClearNamedFramebufferfi(m_id, GL_DEPTH_STENCIL, 0, depth_value, stencil_value);
}

void OglFramebuffer::clear(const u32& color_attachment_index, const void* value)
{
    // TODO:: High chance this don't work
    SPUTNIK_ASSERT_MESSAGE(color_attachment_index < m_color_attachments.size(),
                           "Unknown framebuffer texture format: {}",
                           color_attachment_index);
    switch(m_specification.attachments[color_attachment_index].attachment_format)
    {
    case TextureFormat::RGBA8:
    {
        glClearNamedFramebufferfv(m_id,
                                  GL_COLOR,
                                  GL_DRAW_BUFFER0 + color_attachment_index,
                                  static_cast<const f32*>(value));
        break;
    }
    case TextureFormat::R32I:
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

void OglFramebuffer::bindDepthAttachmentTexture(const u32& slot) const
{
    m_depth_attachment->bind(slot);
}

void OglFramebuffer::bindColorAttachmentTexture(u32 index, const u32& slot) const
{
    m_color_attachments[index]->bind(slot);
}

void OglFramebuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);
    glViewport(0, 0, m_specification.width, m_specification.height);
}

void OglFramebuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OglFramebuffer::attachColorAttachment(const FramebufferAttachmentSpecification& attachment_spec)
{
    // std::unique_ptr<OglTexture2D> texture = std::make_unique<OglTexture2D>(m_specification.width,
    //                                                                        m_specification.height,
    //                                                                        texture_format,
    //                                                                        TextureWrap::ClampToEdge,
    //                                                                        TextureWrap::ClampToEdge,
    //                                                                        TextureWrap::ClampToEdge,
    //                                                                        TextureFilter::Linear,
    //                                                                        TextureFilter::Linear);

    // texture->bind();

    TextureSpecification texture_spec;
    texture_spec.width                    = m_specification.width;
    texture_spec.height                   = m_specification.height;
    texture_spec.texture_format           = attachment_spec.attachment_format;
    texture_spec.r_wrap                   = TextureWrap::ClampToEdge;
    texture_spec.s_wrap                   = TextureWrap::ClampToEdge;
    texture_spec.t_wrap                   = TextureWrap::ClampToEdge;
    texture_spec.min_filter               = TextureFilter::Nearest;
    texture_spec.mag_filter               = TextureFilter::Nearest;
    texture_spec.r_swizzle                = attachment_spec.swizzle_r;
    texture_spec.g_swizzle                = attachment_spec.swizzle_g;
    texture_spec.b_swizzle                = attachment_spec.swizzle_b;
    texture_spec.a_swizzle                = attachment_spec.swizzle_a;
    std::unique_ptr<OglTexture2D> texture = std::make_unique<OglTexture2D>(texture_spec);

    glNamedFramebufferTexture(m_id,
                              GL_COLOR_ATTACHMENT0 + static_cast<u32>(m_color_attachments.size()),
                              texture->getId(),
                              0);
    m_color_attachments.push_back(std::move(texture));
}

} // namespace sputnik::graphics::gl
