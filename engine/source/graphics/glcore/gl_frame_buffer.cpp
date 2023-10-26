#include "pch.h"
#include "gl_frame_buffer.h"
#include "core/core.h"

namespace sputnik::graphics::glcore
{
FrameBuffer::FrameBuffer(const FrameBufferSpecification& frame_buffer_spec) : m_frame_buffer_spec(frame_buffer_spec)
{
    for(auto& spec : m_frame_buffer_spec.m_attachments.m_attachment_specs)
    {
        if(IsDepthFormat(spec.m_texture_format))
        {
            m_depth_attachment_spec = spec;
        }
        else
        {
            m_color_attachment_specs.emplace_back(spec);
        }
    }
    Invalidate();
}

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &m_renderer_id);
    glDeleteTextures(m_color_attachment_ids.size(), m_color_attachment_ids.data());
    m_color_attachment_ids.clear();
    glDeleteTextures(1, &m_depth_attachment_id);
    m_depth_attachment_id = 0;
}

void FrameBuffer::Invalidate()
{
    if(m_renderer_id)
    {
        glDeleteFramebuffers(1, &m_renderer_id);
        glDeleteTextures(m_color_attachment_ids.size(), m_color_attachment_ids.data());
        m_color_attachment_ids.clear();
        glDeleteTextures(1, &m_depth_attachment_id);
        m_depth_attachment_id = 0;
    }

    glCreateFramebuffers(1, &m_renderer_id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id);

    bool isMultiSampled = m_frame_buffer_spec.samples > 1;
    if(!m_color_attachment_specs.empty())
    {
        m_color_attachment_ids.resize(m_color_attachment_specs.size());
        CreateTextures(isMultiSampled, m_color_attachment_ids.size(), m_color_attachment_ids.data());

        for(size_t i = 0; i < m_color_attachment_specs.size(); ++i)
        {
            BindTexture(isMultiSampled, m_color_attachment_ids[i]);
            switch(m_color_attachment_specs[i].m_texture_format)
            {
            case FrameBufferTextureFormat::RGBA8:
            {
                AttachColorTexture(m_color_attachment_ids[i],
                                   m_frame_buffer_spec.samples,
                                   GL_RGBA8,
                                   m_frame_buffer_spec.width,
                                   m_frame_buffer_spec.height,
                                   GL_RGBA,
                                   i);
                break;
            }

            case FrameBufferTextureFormat::RED_INTEGER:
            {
                AttachColorTexture(m_color_attachment_ids[i],
                                   m_frame_buffer_spec.samples,
                                   GL_R32I,
                                   m_frame_buffer_spec.width,
                                   m_frame_buffer_spec.height,
                                   GL_RED_INTEGER,
                                   i);
                break;
            }
            }
        }
    }

    if(m_depth_attachment_spec.m_texture_format != FrameBufferTextureFormat::NONE)
    {
        CreateTextures(isMultiSampled, 1, &m_depth_attachment_id);
        BindTexture(isMultiSampled, m_depth_attachment_id);
        switch(m_depth_attachment_spec.m_texture_format)
        {
        case FrameBufferTextureFormat::DEPTH24_STENCIL8:
        {
            AttachDepthTexture(m_depth_attachment_id,
                               m_frame_buffer_spec.samples,
                               GL_DEPTH24_STENCIL8,
                               GL_DEPTH_STENCIL_ATTACHMENT,
                               m_frame_buffer_spec.width,
                               m_frame_buffer_spec.height);
            break;
        }
        }
    }

    if(m_color_attachment_ids.size() > 1)
    {
        SPUTNIK_ASSERT(m_color_attachment_ids.size() <= 4,
                       "Framebuffer does not support more than 4 color attachments!");
        GLenum buffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
        glDrawBuffers(m_color_attachment_ids.size(), buffers);
    }
    else if(m_color_attachment_ids.empty())
    {
        glDrawBuffer(GL_NONE); // only depth pass
    }

    SPUTNIK_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Frame buffer is incomplete!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Resize(uint32_t width, uint32_t height)
{
    m_frame_buffer_spec.width  = width;
    m_frame_buffer_spec.height = height;
    Invalidate();
}

int FrameBuffer::ReadPixel(uint32_t attachment_index, int x, int y)
{
    SPUTNIK_ASSERT(attachment_index < m_color_attachment_ids.size(), "Invalid index for color attachment!");
    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment_index);
    int pixel_data;
    glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixel_data);
    return pixel_data;
}

void FrameBuffer::ClearColorAttachment(uint32_t attachment_index, int value)
{
    SPUTNIK_ASSERT(attachment_index < m_color_attachment_ids.size(), "Invalid index for color attachment!");

    auto& specification = m_color_attachment_specs[attachment_index];
    glClearTexImage(m_color_attachment_ids[attachment_index],
                    0,
                    ConvertTextureFormatToGLFormat(specification.m_texture_format),
                    GL_INT,
                    &value);
}

void FrameBuffer::ClearColorAttachment(uint32_t attachment_index, const Vector4& color)
{
    SPUTNIK_ASSERT(attachment_index < m_color_attachment_ids.size(), "Invalid index for color attachment!");

    auto& specification = m_color_attachment_specs[attachment_index];
    glClearTexImage(m_color_attachment_ids[attachment_index],
                    0,
                    ConvertTextureFormatToGLFormat(specification.m_texture_format),
                    GL_FLOAT,
                    &(color.v[0]));
}

void FrameBuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id);
    glViewport(0, 0, m_frame_buffer_spec.width, m_frame_buffer_spec.height);
}

void FrameBuffer::UnBind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBufferSpecification& FrameBuffer::GetSpecification()
{
    return m_frame_buffer_spec;
}

const FrameBufferSpecification& FrameBuffer::GetSpecification() const
{
    return m_frame_buffer_spec;
}

uint32_t FrameBuffer::GetColorAttachmentRendererId(int index) const
{
    return m_color_attachment_ids[index];
}

uint32_t FrameBuffer::GetColorAttachmentRendererId(int index)
{
    return m_color_attachment_ids[index];
}

uint32_t FrameBuffer::GetDepthAttachmentRendererId()
{
    return m_depth_attachment_id;
}

bool FrameBuffer::IsDepthFormat(FrameBufferTextureFormat format)
{
    switch(format)
    {
    case FrameBufferTextureFormat::DEPTH24_STENCIL8:
        return true;
    }
    return false;
}

void FrameBuffer::CreateTextures(bool multi_sampled, uint32_t count, uint32_t* out_texture_ids)
{
    glCreateTextures(TextureTarget(multi_sampled), count, out_texture_ids);
}

GLenum FrameBuffer::TextureTarget(bool multi_sampled)
{
    return multi_sampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
}

void FrameBuffer::BindTexture(bool multi_sampled, uint32_t texture_id)
{
    glBindTexture(TextureTarget(multi_sampled), texture_id);
}

void FrameBuffer::AttachColorTexture(uint32_t id,
                                     int      samples,
                                     GLenum   internal_format,
                                     uint32_t width,
                                     uint32_t height,
                                     GLenum   format,
                                     int      index)
{
    bool   multisampled   = samples > 1;
    GLenum texture_target = TextureTarget(multisampled);
    if(multisampled)
    {
        glTexImage2DMultisample(texture_target, samples, internal_format, width, height, GL_FALSE);
    }
    else
    {
        glTexImage2D(texture_target, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(texture_target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(texture_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(texture_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, texture_target, id, 0);
}

void FrameBuffer::AttachDepthTexture(uint32_t id,
                                     int      samples,
                                     GLenum   format,
                                     GLenum   attachment_type,
                                     uint32_t width,
                                     uint32_t height)
{
    bool   multisampled   = samples > 1;
    GLenum texture_target = TextureTarget(multisampled);
    if(multisampled)
    {
        glTexImage2DMultisample(texture_target, samples, format, width, height, GL_FALSE);
    }
    else
    {
        glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
        glTexParameteri(texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(texture_target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(texture_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(texture_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment_type, GL_TEXTURE_2D, id, 0);
}

GLenum FrameBuffer::ConvertTextureFormatToGLFormat(FrameBufferTextureFormat format)
{
    switch(format)
    {
    case FrameBufferTextureFormat::RGBA8:
        return GL_RGBA;
    case FrameBufferTextureFormat::RED_INTEGER:
        return GL_RED_INTEGER;
    }

    SPUTNIK_ASSERT(false, "Unknown framebuffer texture format found!");
    return 0;
}

} // namespace sputnik::graphics::glcore
