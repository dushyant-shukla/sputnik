#pragma once

#include "core/core.h"
#include "core/time_step.h"

#include <stdint.h>
#include <memory>

struct GLFWwindow;

namespace sputnik::graphics::core
{

class GraphicsSubsystem;
enum class GraphicsSubsystemType;

class RenderCommand
{

public:
    NON_INSTATIABLE(RenderCommand);

    static void                          Init(const GraphicsSubsystemType& subsystem_type);
    static void                          Update(const sputnik::core::TimeStep& time_step);
    static void                          Clear();
    static void                          SetClearColor(float r, float g, float b, float a);
    static void                          SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
    static std::pair<uint32_t, uint32_t> GetViewportDimensions();
    static GLFWwindow*                   GetNativeWindow();

private:
    static std::shared_ptr<sputnik::graphics::core::GraphicsSubsystem> s_graphics_subsystem;
};

} // namespace sputnik::graphics::core