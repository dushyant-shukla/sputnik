#pragma once

#include "core/core.h"
#include "core/time_step.h"
#include "graphics/core/graphics_subsystem_type.h"

struct GLFWwindow;

namespace sputnik::graphics::api
{

class Renderer
{

public:
    NON_INSTATIABLE(Renderer);

    static void Init(const sputnik::graphics::core::GraphicsSubsystemType& subsystem_type);
    static void Update(const sputnik::core::TimeStep& time_step);
    static void Clear();
    static void SetClearColor(float r, float g, float b, float a);
    static void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
    static void Shutdown();
    static void OnWindowResize(uint32_t width, uint32_t height);
    static const graphics::core::GraphicsSubsystemType& GetCurrentGraphicsSubsystemType();
    static std::pair<uint32_t, uint32_t>                GetViewportDimensions();
    static GLFWwindow*                                  GetNativeWindow();

private:
    static graphics::core::GraphicsSubsystemType s_graphics_subsystem;
};

} // namespace sputnik::graphics::api