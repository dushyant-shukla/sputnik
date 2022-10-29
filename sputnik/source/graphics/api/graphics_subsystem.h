#pragma once

#include "core/core.h"

#include <stdint.h>
#include <memory>

namespace sputnik::api
{

class GraphicsContext;

class GraphicsSubsystem
{

public:
    virtual void Update()                                                             = 0;
    virtual void Clear() const                                                        = 0;
    virtual void SetClearColor(float r, float g, float b, float a)                    = 0;
    virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

protected:
    std::shared_ptr<GraphicsContext> m_context;
};

} // namespace sputnik::api