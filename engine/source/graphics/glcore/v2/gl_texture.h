#pragma once

#include "core/core.h"

namespace sputnik::graphics::gl
{

class OglTexture
{
protected:
    NON_COPYABLE(OglTexture)

    OglTexture();
    virtual ~OglTexture();

    void bind();
    void unbind();

protected:
    u32 m_id;
};

class OglTexture2D : public OglTexture
{
public:
    OglTexture2D();
    ~OglTexture2D();

    void setData(void* data, u32 width, u32 height, u32 channels);
    void setData(void* data, u32 width, u32 height, u32 channels, u32 format, u32 type);

    void setFiltering(u32 min_filter, u32 mag_filter);
    void setWrapping(u32 s_wrap, u32 t_wrap);

    void bind(u32 slot);
    void unbind(u32 slot);
};

} // namespace sputnik::graphics::gl
