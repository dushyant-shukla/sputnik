#include "pch.h"

#include "gl_texture.h"

#include <glad/glad.h>
#include <stb_image.h>

// Useful links:
// https://www.khronos.org/opengl/wiki/Image_Format
// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexStorage2D.xhtml
// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexSubImage2D.xhtml

namespace sputnik::graphics::gl
{

u32 getOglTextureFormat(const TextureFormat& format)
{
    // Reference: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexStorage2D.xhtml
    switch(format)
    {
    case TextureFormat::R8:
        return GL_R8;
    case TextureFormat::R16:
        return GL_R16;
    case TextureFormat::R32:
        return GL_R32F;
    case TextureFormat::R32I:
        return GL_R32I;
    case TextureFormat::RG8:
        return GL_RG8;
    case TextureFormat::RG16:
        return GL_RG16;
    case TextureFormat::RG32:
        return GL_RG32F;
    case TextureFormat::RGB8:
        return GL_RGB8;
    case TextureFormat::RGB16:
        return GL_RGB16;
    case TextureFormat::RGB32:
        return GL_RGB32F;
    case TextureFormat::RGBA8:
        return GL_RGBA8;
    case TextureFormat::RGBA16:
        return GL_RGBA16;
    case TextureFormat::RGBA32:
        return GL_RGBA32F;
    case TextureFormat::Depth16:
        return GL_DEPTH_COMPONENT16;
    case TextureFormat::Depth24:
        return GL_DEPTH_COMPONENT24;
    case TextureFormat::Depth32:
        return GL_DEPTH_COMPONENT32;
    case TextureFormat::Depth32F:
        return GL_DEPTH_COMPONENT32F;
    case TextureFormat::Depth24Stencil8:
        return GL_DEPTH24_STENCIL8;
    case TextureFormat::Depth32FStencil8:
        return GL_DEPTH32F_STENCIL8;
    default:
        break;
    }
    SPUTNIK_ASSERT_MESSAGE(false, "Invalid texture format: {}", static_cast<u32>(format));
    return 0;
}

u32 getOglTextureDataFormat(const TextureFormat& format)
{
    // Reference: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexStorage2D.xhtml
    switch(format)
    {
    case TextureFormat::R8:
        return GL_RED;
    case TextureFormat::R16:
        return GL_RED;
    case TextureFormat::R32:
        return GL_RED;
    case TextureFormat::R32I:
        return GL_RED;
        // return GL_RED_INTEGER;
    case TextureFormat::RG8:
        return GL_RG;
    case TextureFormat::RG16:
        return GL_RG;
    case TextureFormat::RG32:
        return GL_RG;
    case TextureFormat::RGB8:
        return GL_RGB;
    case TextureFormat::RGB16:
        return GL_RGB;
    case TextureFormat::RGB32:
        return GL_RGB;
    case TextureFormat::RGBA8:
        return GL_RGBA;
    case TextureFormat::RGBA16:
        return GL_RGBA;
    case TextureFormat::RGBA32:
        return GL_RGBA;
    case TextureFormat::Depth16:
        return GL_DEPTH_COMPONENT;
    case TextureFormat::Depth24:
        return GL_DEPTH_COMPONENT;
    case TextureFormat::Depth32:
        return GL_DEPTH_COMPONENT;
    case TextureFormat::Depth32F:
        return GL_DEPTH_COMPONENT;
    case TextureFormat::Depth24Stencil8:
        return GL_DEPTH_STENCIL;
    case TextureFormat::Depth32FStencil8:
        return GL_DEPTH_STENCIL;
    default:
        break;
    }
    SPUTNIK_ASSERT_MESSAGE(false, "Invalid texture format: {}", static_cast<u32>(format));
    return 0;
}

u32 getOglTextureWrap(const TextureWrap& wrap)
{
    switch(wrap)
    {
    case TextureWrap::Repeat:
        return GL_REPEAT;
    case TextureWrap::MirroredRepeat:
        return GL_MIRRORED_REPEAT;
    case TextureWrap::ClampToEdge:
        return GL_CLAMP_TO_EDGE;
    case TextureWrap::ClampToBorder:
        return GL_CLAMP_TO_BORDER;
    default:
        break;
    }
    SPUTNIK_ASSERT_MESSAGE(false, "Invalid texture wrap: {}", static_cast<u32>(wrap));
    return 0;
}

u32 getOglTextureFilter(const TextureFilter& filter)
{
    switch(filter)
    {
    case TextureFilter::Nearest:
        return GL_NEAREST;
    case TextureFilter::Linear:
        return GL_LINEAR;
    case TextureFilter::NearestMipmapNearest:
        return GL_NEAREST_MIPMAP_NEAREST;
    case TextureFilter::LinearMipmapNearest:
        return GL_LINEAR_MIPMAP_NEAREST;
    case TextureFilter::NearestMipmapLinear:
        return GL_NEAREST_MIPMAP_LINEAR;
    case TextureFilter::LinearMipmapLinear:
        return GL_LINEAR_MIPMAP_LINEAR;
    default:
        break;
    }
    SPUTNIK_ASSERT_MESSAGE(false, "Invalid texture filter type: {}", static_cast<u32>(filter));
    return 0;
}

u32 getBytesPerPixel(const TextureFormat& format)
{
    switch(format)
    {
    case TextureFormat::R8:
    case TextureFormat::R16:
    case TextureFormat::R32:
        return 1;
    case TextureFormat::RG8:
    case TextureFormat::RG16:
    case TextureFormat::RG32:
        return 2;
    case TextureFormat::RGB8:
    case TextureFormat::RGB16:
    case TextureFormat::RGB32:
        return 3;
    case TextureFormat::RGBA8:
    case TextureFormat::RGBA16:
    case TextureFormat::RGBA32:
        return 4;
    default:
        break;
    }
    SPUTNIK_ASSERT_MESSAGE(false, "Invalid texture format: {}", static_cast<u32>(format));
    return 0;
}

std::ostream& operator<<(std::ostream& os, const TextureFormat& format)
{
    switch(format)
    {
        SPUTNIK_CASE_TO_OSTREAM(TextureFormat::R8, os);
        SPUTNIK_CASE_TO_OSTREAM(TextureFormat::R16, os);
        SPUTNIK_CASE_TO_OSTREAM(TextureFormat::R32, os);
        SPUTNIK_CASE_TO_OSTREAM(TextureFormat::R32I, os);
        SPUTNIK_CASE_TO_OSTREAM(TextureFormat::RG8, os);
        SPUTNIK_CASE_TO_OSTREAM(TextureFormat::RG16, os);
        SPUTNIK_CASE_TO_OSTREAM(TextureFormat::RG32, os);
        SPUTNIK_CASE_TO_OSTREAM(TextureFormat::RGB8, os);
        SPUTNIK_CASE_TO_OSTREAM(TextureFormat::RGB16, os);
        SPUTNIK_CASE_TO_OSTREAM(TextureFormat::RGB32, os);
        SPUTNIK_CASE_TO_OSTREAM(TextureFormat::RGBA8, os);
        SPUTNIK_CASE_TO_OSTREAM(TextureFormat::RGBA16, os);
        SPUTNIK_CASE_TO_OSTREAM(TextureFormat::RGBA32, os);
        SPUTNIK_CASE_TO_OSTREAM(TextureFormat::Depth16, os);
        SPUTNIK_CASE_TO_OSTREAM(TextureFormat::Depth24, os);
        SPUTNIK_CASE_TO_OSTREAM(TextureFormat::Depth32, os);
        SPUTNIK_CASE_TO_OSTREAM(TextureFormat::Depth32F, os);
        SPUTNIK_CASE_TO_OSTREAM(TextureFormat::Depth24Stencil8, os);
        SPUTNIK_CASE_TO_OSTREAM(TextureFormat::Depth32FStencil8, os);
        SPUTNIK_CASE_DEFAULT_TO_OSTREAM(os);
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const TextureSwizzle& swizzle)
{
    switch(swizzle)
    {
        SPUTNIK_CASE_TO_OSTREAM(TextureSwizzle::Red, os);
        SPUTNIK_CASE_TO_OSTREAM(TextureSwizzle::Green, os);
        SPUTNIK_CASE_TO_OSTREAM(TextureSwizzle::Blue, os);
        SPUTNIK_CASE_TO_OSTREAM(TextureSwizzle::Alpha, os);
        SPUTNIK_CASE_TO_OSTREAM(TextureSwizzle::Zero, os);
        SPUTNIK_CASE_TO_OSTREAM(TextureSwizzle::One, os);
        SPUTNIK_CASE_DEFAULT_TO_OSTREAM(os);
    }
    return os;
}

i32 getOglTextureSwizzle(const TextureSwizzle& swizzle)
{
    switch(swizzle)
    {
    case TextureSwizzle::Red:
        return GL_RED;
    case TextureSwizzle::Green:
        return GL_GREEN;
    case TextureSwizzle::Blue:
        return GL_BLUE;
    case TextureSwizzle::Alpha:
        return GL_ALPHA;
    case TextureSwizzle::Zero:
        return GL_ZERO;
    case TextureSwizzle::One:
        return GL_ONE;
    default:
        break;
    }
    SPUTNIK_ASSERT_MESSAGE(false, "Invalid texture swizzle: {}", static_cast<u32>(swizzle));
    return 0;
}

OglTexture2D::OglTexture2D(cstring              texture_filepath,
                           bool                 flip_vertically,
                           const TextureFormat& texture_format,
                           const TextureWrap&   r_wrap,
                           const TextureWrap&   s_wrap,
                           const TextureWrap&   t_wrap,
                           const TextureFilter& min_filter,
                           const TextureFilter& mag_filter)
    : m_id{0}
    , m_width{0}
    , m_height{0}
    , m_format{TextureFormat::RGB8}
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(flip_vertically);
    unsigned char* data = stbi_load(texture_filepath, &width, &height, &channels, 0);
    SPUTNIK_ASSERT_MESSAGE(data, "Failed to load texture: {}", texture_filepath);
    switch(channels)
    {
    case 1:
        m_format = TextureFormat::R8;
        break;
    case 2:
        m_format = TextureFormat::RG8;
        break;
    case 3:
        m_format = TextureFormat::RGB8;
        break;
    case 4:
        m_format = TextureFormat::RGBA8;
        break;
    default:
        break;
    }

    m_width  = width;
    m_height = height;
    // m_format = texture_format;
    init(data, r_wrap, s_wrap, t_wrap, min_filter, mag_filter);
}

OglTexture2D::OglTexture2D(const u32&           width,
                           const u32&           height,
                           const TextureFormat& texture_format,
                           const TextureWrap&   r_wrap,
                           const TextureWrap&   s_wrap,
                           const TextureWrap&   t_wrap,
                           const TextureFilter& min_filter,
                           const TextureFilter& mag_filter)
    : m_id{0}
    , m_width{width}
    , m_height{height}
    , m_format{texture_format}
{
    init(nullptr, r_wrap, s_wrap, t_wrap, min_filter, mag_filter);
}

OglTexture2D::OglTexture2D(const u32&           width,
                           const u32&           height,
                           void*                data,
                           const TextureFormat& texture_format,
                           const TextureWrap&   r_wrap,
                           const TextureWrap&   s_wrap,
                           const TextureWrap&   t_wrap,
                           const TextureFilter& min_filter,
                           const TextureFilter& mag_filter)
    : m_id{0}
    , m_width{width}
    , m_height{height}
    , m_format{texture_format}
{
    init(data, r_wrap, s_wrap, t_wrap, min_filter, mag_filter);
}

OglTexture2D::OglTexture2D(const TextureSpecification& spec)
    : m_id{0}
    , m_width{spec.width}
    , m_height{spec.height}
    , m_format{spec.texture_format}
{
    init(spec);
}

OglTexture2D::~OglTexture2D()
{
    glDeleteTextures(1, &m_id);
}

OglTexture2D::OglTexture2D(OglTexture2D&& other) noexcept : m_id{0}, m_width{0}, m_height{0}
{
    *this = std::move(other);
}

OglTexture2D& OglTexture2D::operator=(OglTexture2D&& other) noexcept
{
    if(this != &other)
    {
        m_id       = other.m_id;
        m_width    = other.m_width;
        m_height   = other.m_height;
        m_format   = other.m_format;
        other.m_id = 0;
    }
    return *this;
}

void OglTexture2D::setData(void* data, const u32& size)
{
    u32 expected_size = m_width * m_height * getBytesPerPixel(m_format);
    SPUTNIK_ASSERT_MESSAGE(expected_size == size,
                           "Invalid texture size passed to set texture data. Expected size: {}, actual size: {}",
                           expected_size,
                           size);
    glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, getOglTextureDataFormat(m_format), GL_UNSIGNED_BYTE, data);
}

void OglTexture2D::setFiltering(const TextureFilter& min_filter, const TextureFilter& mag_filter)
{
    glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, getOglTextureFilter(min_filter));
    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, getOglTextureFilter(mag_filter));
}

void OglTexture2D::setWrapping(const TextureWrap& s_wrap, const TextureWrap& t_wrap)
{
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, getOglTextureWrap(s_wrap));
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, getOglTextureWrap(s_wrap));
}

void OglTexture2D::bind(const u32& slot)
{
    glBindTextureUnit(slot, m_id);
}

void OglTexture2D::unbind(const u32& slot)
{
    glBindTextureUnit(slot, 0);
}

const u32& OglTexture2D::getId() const
{
    return m_id;
}

void OglTexture2D::init(void*                data,
                        const TextureWrap&   r_wrap,
                        const TextureWrap&   s_wrap,
                        const TextureWrap&   t_wrap,
                        const TextureFilter& min_filter,
                        const TextureFilter& mag_filter)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
    glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, getOglTextureFilter(min_filter));
    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, getOglTextureFilter(mag_filter));
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, getOglTextureWrap(s_wrap));
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, getOglTextureWrap(s_wrap));
    glTextureStorage2D(m_id, 1, getOglTextureFormat(m_format), m_width, m_height);
    if(data)
    {
        glTextureSubImage2D(m_id,
                            0,
                            0,
                            0,
                            m_width,
                            m_height,
                            getOglTextureDataFormat(m_format),
                            GL_UNSIGNED_BYTE,
                            data);
    }
}

void OglTexture2D::init(const TextureSpecification& spec)
{
    const void* data = nullptr;
    if(spec.texture_filepath)
    {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(spec.flip_vertically);
        data = stbi_load(spec.texture_filepath, &width, &height, &channels, 0);
        SPUTNIK_ASSERT_MESSAGE(data, "Failed to load texture: {}", spec.texture_filepath);
        switch(channels)
        {
        case 1:
            m_format = TextureFormat::R8;
            break;
        case 2:
            m_format = TextureFormat::RG8;
            break;
        case 3:
            m_format = TextureFormat::RGB8;
            break;
        case 4:
            m_format = TextureFormat::RGBA8;
            break;
        default:
            break;
        }

        m_width  = width;
        m_height = height;
    }
    else
    {
        m_width  = spec.width;
        m_height = spec.height;
        m_format = spec.texture_format;
        data     = spec.data;
    }

    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
    glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, getOglTextureFilter(spec.min_filter));
    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, getOglTextureFilter(spec.mag_filter));
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, getOglTextureWrap(spec.s_wrap));
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, getOglTextureWrap(spec.t_wrap));
    GLint swizzle[4] = {
        getOglTextureSwizzle(spec.r_swizzle), // Shader Red   channel source = Texture Red
        getOglTextureSwizzle(spec.g_swizzle), // Shader Green channel source = Texture Green
        getOglTextureSwizzle(spec.b_swizzle), // Shader Blue  channel source = Texture Blue
        getOglTextureSwizzle(spec.a_swizzle), // Shader Alpha channel source = One
    };
    glTextureParameteriv(m_id, GL_TEXTURE_SWIZZLE_RGBA, swizzle);

    glTextureStorage2D(m_id, 1, getOglTextureFormat(m_format), m_width, m_height);
    if(data)
    {
        glTextureSubImage2D(m_id,
                            0,
                            0,
                            0,
                            m_width,
                            m_height,
                            getOglTextureDataFormat(m_format),
                            GL_UNSIGNED_BYTE,
                            data);
    }
}

} // namespace sputnik::graphics::gl
