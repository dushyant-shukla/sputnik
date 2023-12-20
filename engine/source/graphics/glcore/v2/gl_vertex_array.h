#pragma once

#include "core/core.h"

namespace sputnik::graphics::glcore::v2
{

class GlVertexArray
{

public:
    // public interface

    GlVertexArray();
    ~GlVertexArray();

    void bind();
    void unbind();

    // add vertex buffer (there can be multiple buffers attached to a vertex array: interleaved vs separate vertex
    // buffers)

    // add index buffer

    // vertex array layout

protected:
    // protected methods

    u32 m_id;

private:
    // private methods

private:
    // private data
};

} // namespace sputnik::graphics::glcore::v2