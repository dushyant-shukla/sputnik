#include "pch.h"
#include "texture.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace sputnik::glcore
{

Texture::Texture() : m_width(0), m_height(0), m_channels(0)
{
    glGenTextures(1, &m_id);
}

Texture::Texture(const char* path)
{
    glGenTextures(1, &m_id);
    Load(path);
}

Texture::~Texture()
{
    glGenTextures(1, &m_id);
}

void Texture::Set(unsigned int uniform_index, unsigned int texture_index)
{
    glActiveTexture(GL_TEXTURE0 + texture_index);
    glBindTexture(GL_TEXTURE_2D, m_id);
    glUniform1i(uniform_index, texture_index);
}

void Texture::Unset(unsigned int texture_index)
{
    glActiveTexture(GL_TEXTURE0 + texture_index);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
}

const unsigned int& Texture::GetId()
{
    return m_id;
}

void Texture::Load(const char* path)
{
    glBindTexture(GL_TEXTURE_2D, m_id);

    int            width, height, channels;
    unsigned char* data = stbi_load(path, &width, &height, &channels, 4);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    m_width    = width;
    m_height   = height;
    m_channels = channels;
}

} // namespace sputnik::glcore