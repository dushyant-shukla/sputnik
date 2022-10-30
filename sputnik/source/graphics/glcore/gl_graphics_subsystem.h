#pragma once

#include "graphics/api/graphics_subsystem.h"

namespace sputnik
{
class Window;
} // namespace sputnik

namespace sputnik::glcore
{

class GlContext;

class GlGraphicsSubsystem : public api::GraphicsSubsystem
{

public:
    GlGraphicsSubsystem();
    virtual ~GlGraphicsSubsystem();

    virtual void Update(const core::TimeStep& time_step);
    virtual void Clear() const;
    virtual void SetClearColor(float r, float g, float b, float a);
    virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

private:
    std::unique_ptr<Window>    m_window;
    std::unique_ptr<GlContext> m_context;
    uint32_t                   m_vao; // Todo: Temporary global vao
};

} // namespace sputnik::glcore