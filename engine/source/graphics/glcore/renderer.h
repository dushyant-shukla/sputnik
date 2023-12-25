#pragma once

#include "core/core.h"

#include <glad/glad.h>

namespace sputnik::graphics::gl
{
class OglVertexArray;
class OglBuffer;
} // namespace sputnik::graphics::gl

namespace sputnik::graphics::glcore
{

using namespace sputnik::graphics::gl;

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

    static void DrawArrays(const u64& vertex_count, DrawMode mode);
    static void DrawArraysInstanced(const u64& vertex_count, const u64& instance_count, DrawMode mode);
    static void DrawElements(const u64& index_count, DrawMode mode);
    static void DrawElementsInstanced(const u64& index_count, const u64& instance_count, DrawMode mode);

    //static void Draw(std::shared_ptr<OglVertexArray>& vertex_array, const u64& vertex_count);
    //static void Draw(OglBuffer& index_buffer, DrawMode mode);
    //static void Draw(unsigned int vertex_count, DrawMode mode);
    //static void DrawInstanced(OglBuffer& index_buffer, DrawMode mode, unsigned int instance_count);
    //static void DrawInstanced(unsigned int vertex_count, DrawMode mode, unsigned int instance_count);

protected:
    // protected methods

private:
    // private methods

    static GLenum DrawModeToGLEnum(DrawMode mode);

private:
    // private data
};

} // namespace sputnik::graphics::glcore