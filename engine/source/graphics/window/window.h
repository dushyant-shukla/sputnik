#pragma once

#include "core/core.h"
#include "window_specification.h"
#include "core/time_step.h"

#include <memory>

struct GLFWwindow;

namespace sputnik::graphics::window
{

class Window
{

public:
    NON_COPYABLE(Window);

    Window(const WindowSpecification& specification = WindowSpecification());
    virtual ~Window();

    virtual unsigned int        GetWidth() const;
    virtual unsigned int        GetHeight() const;
    virtual std::pair<u32, u32> GetWindowWidthAndHeight() const;
    virtual void                SetVsync(bool enable_vsync);
    virtual bool                IsVsyncEnabled();
    virtual GLFWwindow*         GetNativeWindow();
    virtual void                OnUpdate(const sputnik::core::TimeStep& time_step);
    virtual void                setViewportToCurrentWindowSize();

protected:
    virtual void Shutdown();

private:
protected:
    WindowSpecification m_window_specification;
    GLFWwindow*         m_window_handle;
};

} // namespace sputnik::graphics::window