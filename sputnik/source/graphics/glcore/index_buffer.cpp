#include "pch.h"
#include "index_buffer.h"

#include <glad/glad.h>

namespace sputnik::glcore
{

IndexBuffer::IndexBuffer() : m_count(0)
{
    glGenBuffers(1, &m_id);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_id);
}

void IndexBuffer::Set(unsigned int* input_array, unsigned int length)
{
    m_count           = length;
    unsigned int size = sizeof(unsigned int);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * m_count, input_array, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::Set(std::vector<unsigned int>& input)
{
    Set(&input[0], (unsigned int)input.size());
}

const unsigned int& IndexBuffer::GetCount()
{
    return m_count;
}

const unsigned int& IndexBuffer::GetId()
{
    return m_id;
}

} // namespace sputnik::glcore