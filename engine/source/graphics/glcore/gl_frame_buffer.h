#pragma once

#include <vector4.h>
#include <glad/glad.h>

namespace sputnik::graphics::glcore
{

using namespace ramanujan;

enum class FrameBufferTextureFormat
{
    NONE = 0,
    RGBA8, // color
    RED_INTEGER,
    DEPTH24_STENCIL8,
    DEPTH = DEPTH24_STENCIL8 // default
};

struct FrameBufferTextureSpecification
{
    FrameBufferTextureSpecification() = default;
    FrameBufferTextureSpecification(FrameBufferTextureFormat texture_format) : m_texture_format(texture_format) {}

    FrameBufferTextureFormat m_texture_format = FrameBufferTextureFormat::NONE;

    // TODO: filter/wrap configuration
};

struct FrameBufferTextureAttachmentSpecification
{
    FrameBufferTextureAttachmentSpecification() = default;
    FrameBufferTextureAttachmentSpecification(std::initializer_list<FrameBufferTextureSpecification> attachment_specs)
        : m_attachment_specs(attachment_specs)
    {
    }

    std::vector<FrameBufferTextureSpecification> m_attachment_specs;
};

struct FrameBufferSpecification
{
    uint32_t width;
    uint32_t height;
    uint32_t samples           = 1;
    bool     swap_chain_target = false;

    FrameBufferTextureAttachmentSpecification m_attachments;
};

class FrameBuffer
{

public:
    FrameBuffer(const FrameBufferSpecification& spec);
    ~FrameBuffer();

    void Invalidate();

    void Resize(uint32_t width, uint32_t height);
    int  ReadPixel(uint32_t attachment_index, int x, int y);

    void ClearColorAttachment(uint32_t attachment_index, int value);
    void ClearColorAttachment(uint32_t attachment_index, const Vector4& color);

    void Bind() const;
    void UnBind() const;

    FrameBufferSpecification&       GetSpecification();
    const FrameBufferSpecification& GetSpecification() const;
    uint32_t                        GetColorAttachmentRendererId(int index = 0) const;
    uint32_t                        GetColorAttachmentRendererId(int index = 0);
    uint32_t                        GetDepthAttachmentRendererId();

private:
    static bool   IsDepthFormat(FrameBufferTextureFormat format);
    static void   CreateTextures(bool multi_sampled, uint32_t count, uint32_t* out_texture_ids);
    static GLenum TextureTarget(bool multi_sampled);
    static void   BindTexture(bool multi_sampled, uint32_t texture_id);
    static void   AttachColorTexture(uint32_t id,
                                     int      samples,
                                     GLenum   internal_format,
                                     uint32_t width,
                                     uint32_t height,
                                     GLenum   format,
                                     int      index);
    static void
    AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachment_type, uint32_t width, uint32_t height);
    static GLenum ConvertTextureFormatToGLFormat(FrameBufferTextureFormat format);

private:
    uint32_t                                     m_renderer_id{0};
    uint32_t                                     m_depth_attachment_id{0};
    std::vector<uint32_t>                        m_color_attachment_ids;
    FrameBufferSpecification                     m_frame_buffer_spec;
    std::vector<FrameBufferTextureSpecification> m_color_attachment_specs;
    FrameBufferTextureSpecification              m_depth_attachment_spec;
};
} // namespace sputnik::graphics::glcore
