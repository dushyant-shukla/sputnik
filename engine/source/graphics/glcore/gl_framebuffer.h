#pragma once

#include "core/core.h"
#include "gl_texture.h"

#include <vector.hpp>

namespace sputnik::graphics::gl
{

using namespace ramanujan::experimental;

enum class FramebufferTextureFormat : u32
{
    None = 0,
    RGBA8,
    RedInteger,
    Depth24Stencil8,
    Depth = Depth24Stencil8 // default
};

struct FramebufferAttachmentSpecification
{
    FramebufferTextureFormat texture_format = FramebufferTextureFormat::None;
};

struct FramebufferSpecification
{
    u32  width             = 0;
    u32  height            = 0;
    u32  samples           = 1;
    bool swap_chain_target = false;

    std::vector<FramebufferAttachmentSpecification> attachments;
};

class OglFramebuffer
{
public:
    OglFramebuffer(const FramebufferSpecification& spec);
    ~OglFramebuffer();

    OglFramebuffer(OglFramebuffer&& other) noexcept;
    OglFramebuffer& operator=(OglFramebuffer&& other) noexcept;

    void reset();
    void resize(u32 width, u32 height);
    void clear(const vec4& rgba = {0.0f});
    void clear(const vec4& rgba, const float& depth_value, const int& stencil_value);
    void clear(const u32& color_attachment_index, const void* value);

    FramebufferSpecification&       getSpecification();
    const FramebufferSpecification& getSpecification() const;
    u32                             getColorAttachmentId(u32 index = 0) const;
    u32                             getColorAttachmentId(u32 index = 0);
    u32                             getDepthAttachmentId() const;

    void bind();
    void unbind();

private:
    void attachColorAttachment(const FramebufferAttachmentSpecification& attachment_spec,
                               const TextureFormat&                      texture_format);

private:
    OglFramebuffer(const OglFramebuffer&)            = delete;
    OglFramebuffer& operator=(const OglFramebuffer&) = delete;

private:
    u32 m_id;
    // u32              m_depth_attachment_id;
    // std::vector<u32> m_color_attachment_ids;

    std::unique_ptr<OglTexture2D>              m_depth_attachment;
    std::vector<std::unique_ptr<OglTexture2D>> m_color_attachments;

    FramebufferSpecification m_specification;
};

} // namespace sputnik::graphics::gl
