#pragma once

#include "core/core.h"

namespace sputnik::graphics::glcore
{

class Texture
{

public:
    NON_COPYABLE(Texture);

    Texture();
    Texture(const char* path);
    ~Texture();

    void Set(unsigned int uniform_index, unsigned int texture_index);
    void Unset(unsigned int texture_index);

    const unsigned int& GetId();

protected:
    void Load(const char* path);

protected:
    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_channels;
    unsigned int m_id;
};

} // namespace sputnik::graphics::glcore