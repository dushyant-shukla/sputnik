#include "pch.h"
#include "render_api.h"
#include "graphics/glcore/opengl_core.h"

namespace sputnik::api
{

unsigned int RenderAPI::CreateShader()
{
    // return GL_CALL(glCreateShader());
    return 0;
}

void RenderAPI::GetShaderiv(GLuint shader, GLenum pname, GLint* params)
{
    GL_CALL(glGetShaderiv(shader, pname, params));
}

void RenderAPI::GetProgramiv(GLuint program, GLenum pname, GLint* params)
{
    GL_CALL(glGetProgramiv(program, pname, params));
}

} // namespace sputnik::api