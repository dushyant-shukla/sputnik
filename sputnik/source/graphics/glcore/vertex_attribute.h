#pragma once

#include "pch.h"
#include "core/core.h"

namespace sputnik::graphics::glcore
{

/**
 * The VectorAttribute class uses templates. This ensures that if an attribute is meant to be a vec3, we don't
 * accidentally load vec2 into it.
 */
template <typename T>
class VertexAttribute
{

public:
    VertexAttribute();
    ~VertexAttribute();
    NON_COPYABLE(VertexAttribute); // copy constructor and assignment operators are deleted because this class is not
                                   // intended to be copied since it holds a handle to a GPU resource

    void SetVertexAttributePointer(unsigned int slot);

    /**
     * This method fills the associated OpenGL buffer with vertex attribute data
     *
     * \param input_array An array containing the attribute data
     * \param array_length Length of the array
     */
    void Set(T* input_array, unsigned int array_length);
    void Set(std::vector<T>& input);

    void Bind(unsigned int slot);
    void Unbind(unsigned int slot);

    unsigned int GetCount();
    unsigned int GetId();

protected:
    unsigned int m_id;    // Holds a handle to an OpenGL buffer
    unsigned int m_count; // Represents the total number of attributes. For a vertex with position(x, y, z), normal(z,
                          // y, z), and uv(s, t), m_count = 3 + 3 + 2 = 8
};

} // namespace sputnik::graphics::glcore