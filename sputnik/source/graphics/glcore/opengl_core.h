#pragma once

#include "core/core.h"

namespace sputnik::graphics::glcore
{

void GLClearError();
bool GLCheckError(const char* function, const char* file, int line);

#ifdef SPUTNIK_DEBUG

#define GL_CALL(x)                   \
    sputnik::glcore::GLClearError(); \
    x;                               \
    SPUTNIK_ASSERT(sputnik::glcore::GLCheckError(#x, __FILE__, __LINE__))

#else

#define GL_CALL(x) x

#endif // SPUTNIK_DEBUG

// template<typename T>
// T GlCall()

//////////////////////////////////////// OPENGL DECORATORS ////////////////////////////////////////

// Have wrappers for OpenGL calls, that can do error checks in debug mode.

} // namespace sputnik::graphics::glcore