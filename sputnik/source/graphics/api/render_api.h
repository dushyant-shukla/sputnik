#pragma once

#include "core/core.h"

#include <glad/glad.h>

namespace sputnik::api
{

class RenderAPI
{

public:
    NON_INSTATIABLE(RenderAPI);

    static unsigned int CreateProgram();
    static void         UseProgram();
    static void         DeleteProgram();
    static unsigned int CreateShader();
    static void         GetShaderiv(GLuint shader, GLenum pname, GLint* params);
    static void         GetProgramiv(GLuint program, GLenum pname, GLint* params);
};

} // namespace sputnik::api