#pragma once

#include "graphics/core/graphics_subsystem.h"

#include <vector2.h>

namespace sputnik::graphics::window
{
class Window;
} // namespace sputnik::graphics::window

namespace sputnik::graphics::glcore
{

class GlContext;

class GlGraphicsSubsystem : public sputnik::graphics::core::GraphicsSubsystem
{

public:
    GlGraphicsSubsystem();
    virtual ~GlGraphicsSubsystem();

    virtual void                          Update(const sputnik::core::TimeStep& time_step) override;
    virtual void                          Clear() const override;
    virtual void                          SetClearColor(float r, float g, float b, float a) override;
    virtual void                          SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
    virtual std::pair<uint32_t, uint32_t> GetViewPortDimensions() const override;
    virtual GLFWwindow*                   GetNativeWindow() override;

private:
    std::unique_ptr<window::Window> m_window;
    std::unique_ptr<GlContext>      m_context;
    uint32_t                        m_vao; // Todo: Temporary global vao
};

} // namespace sputnik::graphics::glcore