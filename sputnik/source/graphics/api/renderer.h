#pragma once

#include "core/core.h"
#include "graphics/glcore/index_buffer.h"

#include <glad/glad.h>

namespace sputnik::glcore
{

enum class DrawMode
{
    INVALID = 0,
    POINTS,
    LINE_STRIP,
    LINE_LOOP,
    LINES,
    TRIANGLES,
    TRIANGLE_STRIP,
    TRIANGLE_FAN
};

class Renderer
{

public:
    // public interface

    NON_INSTATIABLE(Renderer);

    static void Draw(IndexBuffer& index_buffer, DrawMode mode);
    static void Draw(unsigned int vertex_count, DrawMode mode);
    static void DrawInstanced(IndexBuffer& index_buffer, DrawMode mode, unsigned int instance_count);
    static void DrawInstanced(unsigned int vertex_count, DrawMode mode, unsigned int instance_count);

protected:
    // protected methods

private:
    // private methods

    static GLenum DrawModeToGLEnum(DrawMode mode);

private:
    // private data
};

} // namespace sputnik::glcore