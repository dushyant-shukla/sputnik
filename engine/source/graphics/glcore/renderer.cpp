#include "pch.h"
#include "renderer.h"
#include "graphics/glcore/gl_buffer.h"
#include "graphics/glcore/gl_vertex_array.h"

namespace sputnik::graphics::glcore
{

void Renderer::DrawArrays(const u64& vertex_count, DrawMode mode)
{
    glDrawArrays(DrawModeToGLEnum(mode), 0, vertex_count);
}

void Renderer::DrawArraysInstanced(const u64& vertex_count, const u64& instance_count, DrawMode mode)
{
    glDrawArraysInstanced(DrawModeToGLEnum(mode), 0, vertex_count, instance_count);
}

void Renderer::DrawElements(const u64& index_count, DrawMode mode)
{
    glDrawElements(DrawModeToGLEnum(mode), index_count, GL_UNSIGNED_INT, 0);
    // glDrawElements(DrawModeToGLEnum(mode), index_count, GL_UNSIGNED_INT, indices); // does not work
    //(GLenum mode, GLsizei count, GLenum type, const void* indices);
}

void Renderer::DrawElementsInstanced(const u64& index_count, const u64& instance_count, DrawMode mode)
{
    glDrawElementsInstanced(DrawModeToGLEnum(mode), index_count, GL_UNSIGNED_INT, 0, instance_count);
}

GLenum Renderer::DrawModeToGLEnum(DrawMode mode)
{
    switch(mode)
    {
    case DrawMode::POINTS:
        return GL_POINTS;

    case DrawMode::LINE_STRIP:
        return GL_LINE_STRIP;

    case DrawMode::LINE_LOOP:
        return GL_LINE_LOOP;

    case DrawMode::LINES:
        return GL_LINES;

    case DrawMode::TRIANGLES:
        return GL_TRIANGLES;

    case DrawMode::TRIANGLE_STRIP:
        return GL_TRIANGLE_STRIP;

    case DrawMode::TRIANGLE_FAN:
        return GL_TRIANGLE_FAN;
    }

    // assert() // invalid draw mode
    return static_cast<unsigned int>(DrawMode::INVALID);
}

} // namespace sputnik::graphics::glcore