#include "pch.h"
#include "renderer.h"

namespace sputnik::graphics::glcore
{
void Renderer::Draw(IndexBuffer& index_buffer, DrawMode mode)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer.GetId());
    glDrawElements(DrawModeToGLEnum(mode), index_buffer.GetCount(), GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Renderer::Draw(unsigned int vertex_count, DrawMode mode)
{
    glDrawArrays(DrawModeToGLEnum(mode), 0, vertex_count);
}

void Renderer::DrawInstanced(IndexBuffer& index_buffer, DrawMode mode, unsigned int instance_count)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer.GetId());
    glDrawElementsInstanced(DrawModeToGLEnum(mode), index_buffer.GetCount(), GL_UNSIGNED_INT, 0, instance_count);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Renderer::DrawInstanced(unsigned int vertex_count, DrawMode mode, unsigned int instance_count)
{
    glDrawArraysInstanced(DrawModeToGLEnum(mode), 0, vertex_count, instance_count);
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