#include "pch.h"
#include "uniform.h"

#include <vector2.h>
#include <vector3.h>
#include <vector4.h>
#include <matrix4.h>
#include <quaternion.h>

#include <glad/glad.h>

namespace sputnik::graphics::glcore
{

template Uniform<int>;
template Uniform<ramanujan::IVector2>;
template Uniform<ramanujan::IVector4>;
template Uniform<float>;
template Uniform<ramanujan::Vector2>;
template Uniform<ramanujan::Vector3>;
template Uniform<ramanujan::Vector4>;
template Uniform<ramanujan::Quaternion>;
template Uniform<ramanujan::Matrix4>;

#define UNIFORM_IMPL(gl_function, t_type, d_type)                                    \
    template <>                                                                      \
    void Uniform<t_type>::Set(unsigned int slot, t_type* values, unsigned int count) \
    {                                                                                \
        gl_function(slot, (GLsizei)count, (d_type*)&values[0]);                      \
    }

UNIFORM_IMPL(glUniform1iv, int, int)
UNIFORM_IMPL(glUniform4iv, ramanujan::IVector4, int)
UNIFORM_IMPL(glUniform2iv, ramanujan::IVector2, int)
UNIFORM_IMPL(glUniform1fv, float, float)
UNIFORM_IMPL(glUniform2fv, ramanujan::Vector2, float)
UNIFORM_IMPL(glUniform3fv, ramanujan::Vector3, float)
UNIFORM_IMPL(glUniform4fv, ramanujan::Vector4, float)
UNIFORM_IMPL(glUniform4fv, ramanujan::Quaternion, float)

template <typename T>
void Uniform<T>::Set(unsigned int slot, const T& value)
{
    Set(slot, (T*)&value, 1);
}

template <typename T>
void Uniform<T>::Set(unsigned int slot, std::vector<T>& values)
{
    Set(slot, &values[0], (unsigned int)values.size());
}

template <>
void Uniform<ramanujan::Matrix4>::Set(unsigned int slot, ramanujan::Matrix4* values, unsigned int count)
{
    glUniformMatrix4fv(slot, (GLsizei)count, false, (float*)&values[0]);
}

} // namespace sputnik::graphics::glcore