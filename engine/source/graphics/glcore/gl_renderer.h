#pragma once

#include "core/core.h"
#include "graphics/api/PreethamSkyModel.h"

#include <vector.hpp>
#include <matrix.hpp>

struct GLFWwindow;

namespace sputnik::graphics::gl
{

using namespace sputnik::core;

enum class DrawMode
{
    INVALID = 0,
    POINTS,
    LINE_STRIP,
    LINE_LOOP,
    LINES,
    TRIANGLES,
    TRIANGLE_STRIP,
    TRIANGLE_FAN
};

using namespace ramanujan::experimental;
using namespace sputnik::graphics::api;

class OglRenderer
{

public:
    static OglRenderer* getInstance(GLFWwindow* const window);
    ~OglRenderer();

    OglRenderer(OglRenderer&& other) noexcept;
    OglRenderer& operator=(OglRenderer&& other) noexcept;

    void resizeViewport(const u32& width, const u32& height);
    void resizeViewport(const u32& x, const u32& y, const u32& width, const u32& height);

    void lateUpdate(const TimeStep& timestep, GLFWwindow* const window);

    // TODO:: a bunch of draw call apis (indirect/canonical)
    void draw();

    void clear();
    void setClearColor(float r, float g, float b, float a);

    void renderAtmosphericScattering();

    static void drawArrays(const u64& vertex_count, DrawMode mode);
    static void drawArraysInstanced(const u64& vertex_count, const u64& instance_count, DrawMode mode);
    static void drawElements(const u64& index_count, DrawMode mode);
    static void drawElementsInstanced(const u64& index_count, const u64& instance_count, DrawMode mode);

protected:
private:
    OglRenderer(GLFWwindow* const window);

    OglRenderer(const OglRenderer&)            = delete;
    OglRenderer& operator=(const OglRenderer&) = delete;

    void initializeRenderingState();

    static u32 drawModeToGLEnum(DrawMode mode);

    // Todo:: Will be implemented once the event system is in place.
    // Or maybe render system handles this?
    // void registerEventCallbacks();

private:
    // private data

    // GPU state/buffers

    u32 m_vao;

    // Temporary
    float                             m_exposure        = 1.0f;
    float                             m_sun_angle       = -1.45f; // radians (~ (-83) degress)
    vec3                              m_light_direction = vec3(0.0f, 0.0f, 1.0f);
    PreethamSkyModel                  m_preetham_sky_model;
    std::shared_ptr<OglShaderProgram> m_sky_shader;
};

} // namespace sputnik::graphics::gl