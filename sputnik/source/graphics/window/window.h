#pragma once

#include "core/core.h"
#include "window_specification.h"

#include <memory>

struct GLFWwindow;

namespace sputnik
{

namespace api
{
class GraphicsContext;
}

class Window
{

public:
    NON_COPYABLE(Window);

    Window(const WindowSpecification& specification = WindowSpecification());
    virtual ~Window();

    virtual unsigned int GetWidth() const;
    virtual unsigned int GetHeight() const;
    virtual void         SetVsync(bool enable_vsync);
    virtual bool         IsVsyncEnabled();
    virtual GLFWwindow*  GetNativeWindow();
    virtual void         OnUpdate();

protected:
    virtual void Shutdown();

private:

protected:
    WindowSpecification                   m_window_specification;
    GLFWwindow*                           m_window_handle;
};

} // namespace sputnik