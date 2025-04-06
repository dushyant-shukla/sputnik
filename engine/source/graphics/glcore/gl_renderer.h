#pragma once

#include "core/core.h"
#include "graphics/api/PreethamSkyModel.h"
#include "graphics/glcore/gl_vertex_array.h"
#include "graphics/glcore/gl_pipeline.h"
#include "graphics/api/light.h"
#include "graphics/api/color_material.h"
#include "graphics/glcore/gl_buffer.h"

#include <vector.hpp>
#include <matrix.hpp>
#include <glm/glm.hpp>

struct GLFWwindow;

namespace sputnik::graphics::gl
{

using namespace sputnik::core;
using namespace ramanujan::experimental;
using namespace sputnik::graphics::api;

struct PerFrameData
{
    alignas(16) mat4 projection;
    alignas(16) mat4 view;
    alignas(16) vec3 camera_position;
};

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

struct DrawElementsIndirectCommand
{
    u32 count;
    u32 instance_count;
    u32 first_index;
    u32 base_vertex;
    u32 base_instance;
};

struct DrawArraysIndirectCommand
{
    u32 count;
    u32 instance_count;
    u32 first_vertex;
    u32 base_instance;
};

class OglRenderer
{

public:
    static OglRenderer* getInstance(GLFWwindow* const window);
    ~OglRenderer();

    OglRenderer(OglRenderer&& other) noexcept;
    OglRenderer& operator=(OglRenderer&& other) noexcept;

    void clear();
    void setClearColor(float r, float g, float b, float a);
    void resizeViewport(const u32& width, const u32& height);
    void resizeViewport(const u32& x, const u32& y, const u32& width, const u32& height);

    void lateUpdate(const TimeStep& timestep, GLFWwindow* const window);

    // TODO:: a bunch of draw call apis (indirect/canonical)
    // would be replaced by a pointer to the active camera type
    void render(const mat4& projection, const mat4& view, const vec3& camera_position, const Light& light);

    // Todo:: Indirect draw calls

    void renderAtmosphericScattering();
    void renderEditorGrid();

    static void drawArrays(const u64& vertex_count, DrawMode mode);
    static void drawArraysInstanced(const u64& vertex_count, const u64& instance_count, DrawMode mode);
    static void drawElements(const u64& index_count, DrawMode mode);
    static void drawElementsInstanced(const u64& index_count, const u64& instance_count, DrawMode mode);

    // parameters: vao, shader program, material, mat4 model
    // Canonical draw calls
    void drawTriangles(const u64& vertex_count, const Material& material, const mat4& model);
    void drawTrianglesIndexed(const u64& index_count, const Material& material, const mat4& model);
    void drawTrianglesIndexed(const u64&                  index_count,
                              const Material&             material,
                              const mat4&                 model,
                              const std::vector<Matrix4>& skin_transformations);
    void drawTrianglesInstanced(const u64& vertex_count, const u64& instance_count, const Material& material);
    void drawTrianglesIndexedInstanced(const u64& index_count, const u64& instance_count, const Material& material);
    void drawDebugLines(const std::vector<vec4>& vertices, const vec3& color, const float& line_width = 2.5f);
    void drawDebugLines(const std::vector<vec4>& vertices,
                        const vec3&              color,
                        const mat4&              model      = {},
                        const float&             line_width = 2.5f);
    void drawDebugLines(const std::vector<vec4>& vertices,
                        const vec3&              color,
                        const glm::mat4&         model      = glm::mat4(1.0f),
                        const float&             line_width = 2.5f);
    void drawDebugPoints(const std::vector<vec4>& vertices, const vec3& color, const float& point_size = 2.5f);
    void drawDebugPoints(const std::vector<vec4>& vertices,
                         const vec3&              color,
                         const mat4&              model      = {},
                         const float&             point_size = 2.5f);
    void drawDebugPoints(const std::vector<vec4>& vertices,
                         const vec3&              color,
                         const glm::mat4&         model      = glm::mat4(1.0f),
                         const float&             point_size = 2.5f);

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

    // u32 m_vao;

    // Temporary
    float            m_exposure        = 1.0f;
    float            m_sun_angle       = -1.45f; // radians (~ (-83) degress)
    vec3             m_light_direction = vec3(0.0f, 0.0f, 1.0f);
    PreethamSkyModel m_preetham_sky_model;

    // A vertex array object for immediate drawing purposes (sky, grid, points, lines, maybe)
    std::unique_ptr<OglVertexArray> m_vao;

    // Shader programs
    std::shared_ptr<OglShaderProgram> m_sky_program;
    std::shared_ptr<OglShaderProgram> m_grid_program;
    std::shared_ptr<OglShaderProgram> m_blinn_phong_program;
    std::shared_ptr<OglShaderProgram> m_blinn_phong_skinned_program;
    std::shared_ptr<OglShaderProgram> m_blinn_phong_instanced_program;
    std::shared_ptr<OglShaderProgram> m_blinn_phong_pvp_program;
    std::shared_ptr<OglShaderProgram> m_debug_draw_program;

    // CPU data
    PerFrameData m_per_frame_data;
    Light        m_light_data;

    // GPU buffers
    const u8                   kPerFrameDataBindingPoint = 0;
    std::shared_ptr<OglBuffer> m_per_frame_gpu_buffer;
    const u8                   kLightDataBindingPoint = 1;
    std::shared_ptr<OglBuffer> m_light_gpu_buffer;

    struct VertexData
    {
        alignas(16) vec3 position;
        alignas(16) vec3 normal;
        alignas(8) vec2 uv;
    };
    std::vector<VertexData>    m_vertex_data;
    std::shared_ptr<OglBuffer> m_vertex_buffer; // SSBO for PVP

    // Skinned vertex data

    // default textures
    std::shared_ptr<OglTexture2D> m_white_texture;
    std::shared_ptr<OglTexture2D> m_red_texture;
    std::shared_ptr<OglTexture2D> m_green_texture;
    std::shared_ptr<OglTexture2D> m_blue_texture;
};

} // namespace sputnik::graphics::gl