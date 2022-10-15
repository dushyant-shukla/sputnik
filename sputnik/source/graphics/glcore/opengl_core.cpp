#include "pch.h"
#include "opengl_core.h"

#include <glad/glad.h>

namespace sputnik::glcore
{

void GLClearError()
{
    while(glGetError() != GL_NO_ERROR)
        ;
}

bool GLCheckError(const char* function, const char* file, int line)
{
    while(GLenum error = glGetError())
    {
        std::cerr << "[OpenGL Error] (" << error << ") : " << function << " " << file << " " << line << " "
                  << std::endl;
        return false;
    }
    return true;
}

} // namespace sputnik::glcore
