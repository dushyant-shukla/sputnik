#pragma once

#include "core/core.h"
#include "core/time_step.h"
#include "graphics_subsystem_type.h"

namespace sputnik::api
{

class Renderer
{

public:
    NON_INSTATIABLE(Renderer);

    static void                         Init(const GraphicsSubsystemType& subsystem_type);
    static void                         Update(const core::TimeStep& time_step);
    static void                         Clear();
    static void                         SetClearColor(float r, float g, float b, float a);
    static void                         SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
    static void                         Shutdown();
    static void                         OnWindowResize(uint32_t width, uint32_t height);
    static const GraphicsSubsystemType& GetCurrentGraphicsSubsystemType();

private:
    static GraphicsSubsystemType s_graphics_subsystem;
};

} // namespace sputnik::api