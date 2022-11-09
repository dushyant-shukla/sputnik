#pragma once

#include "pch.h"

namespace sputnik::graphics::core
{

class IndexBuffer
{

public:
    IndexBuffer();
    virtual ~IndexBuffer();

    virtual void                        Bind() const                                     = 0;
    virtual void                        Unbind() const                                   = 0;
    virtual ui32                        GetCount() const                                 = 0;
    virtual void                        UpdateData(ui32 const* indices, ui32 size) const = 0;
    static std::shared_ptr<IndexBuffer> CreateIndexBuffer(ui32 const* indices, ui32 count);
    static std::shared_ptr<IndexBuffer> CreateIndexBuffer(ui32 count);
};

} // namespace sputnik::graphics::core