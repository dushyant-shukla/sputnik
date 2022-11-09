#include "pch.h"
#include "vertex_attribute.h"

#include <glad/glad.h>
#include <vector2.h>
#include <vector3.h>
#include <vector4.h>

namespace sputnik::graphics::glcore
{

template VertexAttribute<int>;
template VertexAttribute<float>;
template VertexAttribute<ramanujan::Vector2>;
template VertexAttribute<ramanujan::Vector3>;
template VertexAttribute<ramanujan::Vector4>;
template VertexAttribute<ramanujan::IVector4>;

/**
 * The constructor allocates an OpenGL buffer.
 */
template <typename T>
VertexAttribute<T>::VertexAttribute() : m_count(0)
{
    glGenBuffers(1, &m_id);
}

/**
 * The destructor is responsible for freeing-up the allocated OpenGL buffer memory.
 */
template <typename T>
VertexAttribute<T>::~VertexAttribute()
{
    glDeleteBuffers(1, &m_id);
}

template <>
void VertexAttribute<int>::SetVertexAttributePointer(unsigned int slot)
{
    glVertexAttribIPointer(slot, 1, GL_INT, 0, (void*)0);
}

template <>
void VertexAttribute<ramanujan::IVector4>::SetVertexAttributePointer(unsigned int slot)
{
    // stride == 0 is a special case, it means that your attributes are tightly packed (separate buffers and not
    // interleaved). So e.g. if you're passing a dvec4 (vector of 4 doubles) per vertex, it doesn't matter if you use 0
    // or 32
    glVertexAttribIPointer(slot, 4, GL_INT, 0, (void*)0);
}

template <>
void VertexAttribute<float>::SetVertexAttributePointer(unsigned int slot)
{
    glVertexAttribPointer(slot, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

template <>
void VertexAttribute<ramanujan::Vector2>::SetVertexAttributePointer(unsigned int slot)
{
    glVertexAttribPointer(slot, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

template <>
void VertexAttribute<ramanujan::Vector3>::SetVertexAttributePointer(unsigned int slot)
{
    glVertexAttribPointer(slot, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

template <>
void VertexAttribute<ramanujan::Vector4>::SetVertexAttributePointer(unsigned int slot)
{
    glVertexAttribPointer(slot, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

template <typename T>
void VertexAttribute<T>::Set(T* input_array, unsigned int array_length)
{
    m_count                       = array_length;
    unsigned int size_per_element = sizeof(T);

    glBindBuffer(GL_ARRAY_BUFFER, m_id); // Bind to the allocated OpenGL buffer
    glBufferData(GL_ARRAY_BUFFER,
                 size_per_element * m_count,
                 input_array,
                 GL_STREAM_DRAW); // Update the allocated buffer with data
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template <typename T>
void VertexAttribute<T>::Set(std::vector<T>& input)
{
    Set(&input[0], (unsigned int)input.size());
}

template <typename T>
void VertexAttribute<T>::Bind(unsigned int slot)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id); // Todo: Understand this better + what is GL_VERTEX_ARRAY used for? Original
                                         // code unbinds the array buffer (Why?)
    glEnableVertexAttribArray(slot);
    SetVertexAttributePointer(slot);
}

template <typename T>
void VertexAttribute<T>::Unbind(unsigned int slot)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glDisableVertexAttribArray(slot);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Todo: Understand this better
}

template <typename T>
unsigned int VertexAttribute<T>::GetCount()
{
    return m_count;
}

template <typename T>
unsigned int VertexAttribute<T>::GetId()
{
    return m_id;
}

} // namespace sputnik::graphics::glcore