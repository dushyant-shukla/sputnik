#pragma once

#include "core/core.h"

namespace sputnik::graphics::gl
{

enum class TextureFormat : u32
{
    Invalid = 0,
    R8,
    R16,
    R32,
    R32I,
    RG8,
    RG16,
    RG32,
    RGB8,
    RGB16,
    RGB32,
    RGBA8,
    RGBA16,
    RGBA32,
    Depth16,
    Depth24,
    Depth32,
    Depth32F,
    Depth24Stencil8,
    Depth32FStencil8
};

enum class TextureWrap : u32
{
    Invalid = 0,
    Repeat,
    MirroredRepeat,
    ClampToEdge,
    ClampToBorder
};

enum class TextureFilter : u32
{
    Invalid = 0,
    Nearest,
    Linear,
    NearestMipmapNearest,
    LinearMipmapNearest,
    NearestMipmapLinear,
    LinearMipmapLinear
};

enum class TextureSwizzle : i32
{
    Invalid = 0,
    Red,
    Green,
    Blue,
    Alpha,
    Zero,
    One
};

std::ostream& operator<<(std::ostream& os, const TextureFormat& format);
std::ostream& operator<<(std::ostream& os, const TextureSwizzle& format);

struct TextureSpecification
{
    u32            width;
    u32            height;
    void*          data{nullptr};
    cstring        texture_filepath{nullptr}; // ideally this is temporary and texture loading is handled elsewhere
    TextureFormat  texture_format  = TextureFormat::RGBA8;
    TextureWrap    r_wrap          = TextureWrap::Repeat;
    TextureWrap    s_wrap          = TextureWrap::Repeat;
    TextureWrap    t_wrap          = TextureWrap::Repeat;
    TextureFilter  min_filter      = TextureFilter::Linear;
    TextureFilter  mag_filter      = TextureFilter::Nearest;
    TextureSwizzle r_swizzle       = TextureSwizzle::Red;
    TextureSwizzle g_swizzle       = TextureSwizzle::Green;
    TextureSwizzle b_swizzle       = TextureSwizzle::Blue;
    TextureSwizzle a_swizzle       = TextureSwizzle::Alpha;
    bool           flip_vertically = false;
};

class OglTexture2D
{
public:
    OglTexture2D(cstring              texture_filepath,
                 bool                 flip_vertically = false,
                 const TextureFormat& texture_format  = TextureFormat::Invalid,
                 const TextureWrap&   r_wrap          = TextureWrap::Repeat,
                 const TextureWrap&   s_wrap          = TextureWrap::Repeat,
                 const TextureWrap&   t_wrap          = TextureWrap::Repeat,
                 const TextureFilter& min_filter      = TextureFilter::Linear,
                 const TextureFilter& mag_filter      = TextureFilter::Nearest);

    OglTexture2D(const u32&           width,
                 const u32&           height,
                 const TextureFormat& texture_format = TextureFormat::RGB8,
                 const TextureWrap&   r_wrap         = TextureWrap::Repeat,
                 const TextureWrap&   s_wrap         = TextureWrap::Repeat,
                 const TextureWrap&   t_wrap         = TextureWrap::Repeat,
                 const TextureFilter& min_filter     = TextureFilter::Linear,
                 const TextureFilter& mag_filter     = TextureFilter::Nearest);

    OglTexture2D(const u32&           width,
                 const u32&           height,
                 void*                data,
                 const TextureFormat& texture_format = TextureFormat::RGB8,
                 const TextureWrap&   r_wrap         = TextureWrap::Repeat,
                 const TextureWrap&   s_wrap         = TextureWrap::Repeat,
                 const TextureWrap&   t_wrap         = TextureWrap::Repeat,
                 const TextureFilter& min_filter     = TextureFilter::Linear,
                 const TextureFilter& mag_filter     = TextureFilter::Nearest);

    OglTexture2D(const TextureSpecification& spec);

    ~OglTexture2D();

    OglTexture2D(OglTexture2D&& other) noexcept;
    OglTexture2D& operator=(OglTexture2D&& other) noexcept;

    void setData(void* data, const u32& size);
    void setFiltering(const TextureFilter& min_filter, const TextureFilter& mag_filter);
    void setWrapping(const TextureWrap& s_wrap, const TextureWrap& t_wrap);

    void bind(const u32& slot = 0);
    void unbind(const u32& slot = 0);

    const u32& getId() const;

private:
    OglTexture2D(const OglTexture2D&)            = delete;
    OglTexture2D& operator=(const OglTexture2D&) = delete;

    void init(void*                data,
              const TextureWrap&   r_wrap,
              const TextureWrap&   s_wrap,
              const TextureWrap&   t_wrap,
              const TextureFilter& min_filter,
              const TextureFilter& mag_filter);

    void init(const TextureSpecification& spec);

private:
    u32           m_id;
    u32           m_width;
    u32           m_height;
    TextureFormat m_format;
};

} // namespace sputnik::graphics::gl
