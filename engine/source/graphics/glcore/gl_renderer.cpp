#include "pch.h"
#include "gl_renderer.h"
#include "editor/editor_camera.h"
#include "graphics/glcore/gl_shader.h"
#include "graphics/glcore/gl_buffer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace sputnik::graphics::gl
{

void oglMessageCallback(GLenum        source,
                        GLenum        type,
                        GLuint        id,
                        GLenum        severity,
                        GLsizei       length,
                        GLchar const* message,
                        void const*   user_param)
{
    auto const src_str = [source]()
    {
        switch(source)
        {
        case GL_DEBUG_SOURCE_API:
            return "API";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            return "WINDOW SYSTEM";
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            return "SHADER COMPILER";
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            return "THIRD PARTY";
        case GL_DEBUG_SOURCE_APPLICATION:
            return "APPLICATION";
        case GL_DEBUG_SOURCE_OTHER:
            return "OTHER";
        default:
            return "UNKNOWN";
        }
    }();

    auto const type_str = [type]()
    {
        switch(type)
        {
        case GL_DEBUG_TYPE_ERROR:
            return "ERROR";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            return "DEPRECATED_BEHAVIOR";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            return "UNDEFINED_BEHAVIOR";
        case GL_DEBUG_TYPE_PORTABILITY:
            return "PORTABILITY";
        case GL_DEBUG_TYPE_PERFORMANCE:
            return "PERFORMANCE";
        case GL_DEBUG_TYPE_MARKER:
            return "MARKER";
        case GL_DEBUG_TYPE_OTHER:
            return "OTHER";
        default:
            return "UNKNOWN";
        }
    }();

    auto const severity_str = [severity]()
    {
        switch(severity)
        {
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            return "NOTIFICATION";
        case GL_DEBUG_SEVERITY_LOW:
            return "LOW";
        case GL_DEBUG_SEVERITY_MEDIUM:
            return "MEDIUM";
        case GL_DEBUG_SEVERITY_HIGH:
            return "HIGH";
        default:
            return "UNKNOWN";
        }
    }();
    // std::cout << src_str << ", " << type_str << ", " << severity_str << ", " << id << ": " << message << '\n';

    SPUTNIK_ASSERT_MESSAGE(false,
                           "OpenGL Error: Source: {}, Type: {}, Severity: {}, Id: {}, Message: {}",
                           src_str,
                           type_str,
                           severity_str,
                           id,
                           message);
}

OglRenderer::OglRenderer(GLFWwindow* const window)
{
    glfwMakeContextCurrent(window);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        ENGINE_CRITICAL("Failed to initialize Glad!");
    }

    // #ifdef _DEBUG
    // Enable OpenGL debug messages
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(oglMessageCallback, nullptr);

    // Disable deprecated OpenGL functionality errors
    glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, GL_DONT_CARE, 0, nullptr, GL_FALSE);

    // Todo: Figure out the performance issue when running the rope bridge demo
    // Disable performance warnings
    glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_PERFORMANCE, GL_DONT_CARE, 0, nullptr, GL_FALSE);

    // Disable SEVERITY: NOTIFICATION messages : NVIDIA is throwing a lot of these
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

    // #endif // DEBUG

    m_per_frame_gpu_buffer = std::make_shared<OglBuffer>(sizeof(PerFrameData));
    // m_per_frame_gpu_buffer->bind(BufferBindTarget::UniformBuffer, 0); // bind to binding point 0
    m_per_frame_gpu_buffer->bind(BufferBindTarget::UniformBuffer, kPerFrameDataBindingPoint); // bind to binding point 0

    m_light_gpu_buffer = std::make_shared<OglBuffer>(sizeof(Light));
    // m_light_gpu_buffer->bind(BufferBindTarget::UniformBuffer, 1); // bind to binding point 1
    m_light_gpu_buffer->bind(BufferBindTarget::UniformBuffer, kLightDataBindingPoint); // bind to binding point 1

    m_vao = std::make_unique<OglVertexArray>();

    m_sky_program = std::make_shared<OglShaderProgram>();
    m_sky_program->addShaderStage("../../data/shaders/sky-rendering/sky.vert");
    m_sky_program->addShaderStage("../../data/shaders/sky-rendering/sky.frag");
    m_sky_program->configure();

    m_grid_program = std::make_shared<OglShaderProgram>();
    m_grid_program->addShaderStage("../../data/shaders/glsl/grid.vert");
    m_grid_program->addShaderStage("../../data/shaders/glsl/grid.frag");
    m_grid_program->configure();

    m_blinn_phong_program = std::make_shared<OglShaderProgram>();
    m_blinn_phong_program->addShaderStage("../../data/shaders/glsl/blinn_phong.vert");
    m_blinn_phong_program->addShaderStage("../../data/shaders/glsl/blinn_phong.frag");
    m_blinn_phong_program->configure();

    m_blinn_phong_skinned_program = std::make_shared<OglShaderProgram>();
    m_blinn_phong_skinned_program->addShaderStage("../../data/shaders/glsl/skinned.vert");
    m_blinn_phong_skinned_program->addShaderStage("../../data/shaders/glsl/blinn_phong.frag");
    m_blinn_phong_skinned_program->configure();

    m_blinn_phong_pvp_program = std::make_shared<OglShaderProgram>();
    m_blinn_phong_pvp_program->addShaderStage("../../data/shaders/glsl/blinn_phong_pvp.vert");
    m_blinn_phong_pvp_program->addShaderStage("../../data/shaders/glsl/blinn_phong.frag");
    m_blinn_phong_pvp_program->configure();

    m_blinn_phong_instanced_program = std::make_shared<OglShaderProgram>();
    m_blinn_phong_instanced_program->addShaderStage("../../data/shaders/glsl/blinn_phong_instanced.vert");
    m_blinn_phong_instanced_program->addShaderStage("../../data/shaders/glsl/blinn_phong_instanced.frag");
    m_blinn_phong_instanced_program->configure();

    m_debug_draw_program = std::make_shared<OglShaderProgram>();
    m_debug_draw_program->addShaderStage("../../data/shaders/glsl/debug_draw.vert");
    m_debug_draw_program->addShaderStage("../../data/shaders/glsl/debug_draw.frag");
    m_debug_draw_program->configure();

    m_light_direction = vec3(0.0f, sin(m_sun_angle), cos(m_sun_angle)).normalized();

    u32 white       = 0xffffffff;
    u32 red         = 0xff0000ff;
    u32 green       = 0xff00ff00;
    u32 blue        = 0xffff0000;
    m_white_texture = std::make_shared<OglTexture2D>(1, 1, &white, TextureFormat::RGBA8);
    m_red_texture   = std::make_shared<OglTexture2D>(1, 1, &red, TextureFormat::RGBA8);
    m_green_texture = std::make_shared<OglTexture2D>(1, 1, &green, TextureFormat::RGBA8);
    m_blue_texture  = std::make_shared<OglTexture2D>(1, 1, &blue, TextureFormat::RGBA8);
}

OglRenderer* OglRenderer::getInstance(GLFWwindow* const window)
{
    static OglRenderer instance(window);
    return &instance;
}

OglRenderer::~OglRenderer() {}

OglRenderer::OglRenderer(OglRenderer&& other) noexcept
{
    *this = std::move(other);
}

OglRenderer& OglRenderer::operator=(OglRenderer&& other) noexcept
{
    return *this;
}

void OglRenderer::resizeViewport(const u32& width, const u32& height)
{
    glViewport(0, 0, width, height);
}

void OglRenderer::resizeViewport(const u32& x, const u32& y, const u32& width, const u32& height)
{
    glViewport(x, y, width, height);
}

void OglRenderer::render(const mat4& projection, const mat4& view, const vec3& camera_position, const Light& light)
{
    // Todo:: rendering code
    // render sky
    // render grid
    // render scene geometry

    // update per frame gpu buffer
    m_per_frame_data.projection      = projection;
    m_per_frame_data.view            = view;
    m_per_frame_data.camera_position = camera_position;
    m_per_frame_gpu_buffer->setData(&m_per_frame_data, sizeof(PerFrameData));

    // update light gpu buffer
    m_light_gpu_buffer->setData((void*)&light, sizeof(Light));

    renderAtmosphericScattering(); // sky
    renderEditorGrid();            // grid

    // Todo:: render scene geometry with indirect draw calls
}

void OglRenderer::initializeRenderingState() {}

void OglRenderer::lateUpdate(const core::TimeStep& timestep, GLFWwindow* const window)
{
    glfwSwapBuffers(window);
}

void OglRenderer::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void OglRenderer::setClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void OglRenderer::renderAtmosphericScattering()
{
    m_vao->bind();

    m_preetham_sky_model.SetDirection(m_light_direction);
    m_preetham_sky_model.Update();

    mat4 camera_projection{};
    mat4 camera_view{};
    vec3 camera_position{};

    // if(m_camera_type == CameraType::Camera)
    //{
    //     camera_projection = renderer->m_camera->GetCameraPerspective();
    //     camera_view       = renderer->m_camera->GetCameraView();
    //     camera_position   = renderer->m_camera->GetCameraPosition();
    // }
    // else
    //{
    //     camera_projection = renderer->m_editor_camera->GetCameraPerspective();
    //     camera_view       = renderer->m_editor_camera->GetCameraView();
    //     camera_position   = renderer->m_editor_camera->GetCameraPosition();
    // }

    auto m_editor_camera = EditorCamera::GetInstance();
    camera_projection    = m_editor_camera->GetCameraPerspective();
    camera_view          = m_editor_camera->GetCameraView();
    camera_position      = m_editor_camera->GetCameraPosition();

    // removing translation from the view matrix
    mat4 cubemap_view_mat4{camera_view};
    cubemap_view_mat4.m[3]  = 0;
    cubemap_view_mat4.m[7]  = 0;
    cubemap_view_mat4.m[11] = 0;
    cubemap_view_mat4.m[15] = 1;
    cubemap_view_mat4.m[12] = 0;
    cubemap_view_mat4.m[13] = 0;
    cubemap_view_mat4.m[14] = 0;

    mat4 cubemap_view_projection_mat4     = camera_projection * cubemap_view_mat4;
    mat4 inv_cubemap_view_projection_mat4 = cubemap_view_projection_mat4.inverted();

    // setup pipeline state, there should be a PipelineState API with set() and reset(), bind/unbind methods
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    m_sky_program->bind();

    m_sky_program->setMat4("inv_view_projection", inv_cubemap_view_projection_mat4);

    m_preetham_sky_model.SetRenderUniforms(m_sky_program);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    m_sky_program->unbind();

    // reset the pipeline state
    glDepthFunc(GL_LESS); // This is the default depth function
    glDisable(GL_DEPTH_TEST);

    m_vao->unbind();
}

void OglRenderer::renderEditorGrid()
{
    m_vao->bind();
    m_grid_program->bind();
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    //  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // black lines
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA); // This gives best results - white lines
    // (GLenum mode, GLint first, GLsizei count, GLsizei instancecount, GLuint baseinstance)
    glDrawArraysInstancedBaseInstance(GL_TRIANGLES, 0, 6, 1, 0);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    m_grid_program->unbind();
    m_vao->unbind();
}

void OglRenderer::drawTriangles(const u64& vertex_count, const Material& material, const mat4& model)
{
    auto active_program = m_blinn_phong_program;
    if(material.shader_name == "blinn_phong_pvp")
    {
        m_vao->bind();
        active_program = m_blinn_phong_pvp_program;
    }

    glEnable(GL_DEPTH_TEST);
    {
        mat4 normal_matrix = model;
        normal_matrix      = normal_matrix.inverted().transpose(); // (Transpose of inverse of the model matrix)

        active_program->bind();
        active_program->setMat4("model", model);
        active_program->setMat4("normal_matrix", normal_matrix);
        // m_blinn_phong_program->setFloat3("material.ambient", material.ambient);
        active_program->setFloat3("material.diffuse", material.diffuse);
        active_program->setFloat3("material.specular", material.specular);
        active_program->setFloat("material.shininess", material.shininess);
        if(material.diff_texture)
        {
            active_program->setInt("material.diffuse_texture", 0);
            material.diff_texture->bind(0);
        }
        else
        {
            active_program->setInt("material.diffuse_texture", 0);
            m_white_texture->bind(0);
        }
        if(material.spec_texture)
        {
            active_program->setInt("material.specular_texture", 1);
            material.spec_texture->bind(1);
        }
        else
        {
            active_program->setInt("material.specular_texture", 1);
            m_white_texture->bind(1);
        }

        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertex_count);
        if(material.shader_name == "blinn_phong_pvp")
        {
            m_vao->unbind();
        }

        active_program->unbind();
    }
    glDisable(GL_DEPTH_TEST);
}

void OglRenderer::drawTrianglesIndexed(const u64& index_count, const Material& material, const mat4& model)
{
    auto active_program = m_blinn_phong_program;
    if(material.shader_name == "blinn_phong_pvp")
    {
        m_vao->bind();
        active_program = m_blinn_phong_pvp_program;
    }
    else if(material.shader_name == "m_blinn_phong_skinned")
    {
        active_program = m_blinn_phong_skinned_program;
    }

    glEnable(GL_DEPTH_TEST);
    {
        mat4 normal_matrix = model;
        normal_matrix      = normal_matrix.inverted().transpose(); // (Transpose of inverse of the model matrix)

        active_program->bind();
        active_program->setMat4("model", model);
        active_program->setMat4("normal_matrix", normal_matrix);
        // m_blinn_phong_program->setFloat3("material.ambient", material.ambient);
        active_program->setFloat3("material.diffuse", material.diffuse);
        active_program->setFloat3("material.specular", material.specular);
        active_program->setFloat("material.shininess", material.shininess);
        if(material.diff_texture)
        {
            active_program->setInt("material.diffuse_texture", 0);
            material.diff_texture->bind(0);
        }
        else
        {
            active_program->setInt("material.diffuse_texture", 0);
            m_white_texture->bind(0);
        }
        if(material.spec_texture)
        {
            active_program->setInt("material.specular_texture", 1);
            material.spec_texture->bind(1);
        }
        {
            active_program->setInt("material.specular_texture", 1);
            m_white_texture->bind(1);
        }

        glDrawElements(GL_TRIANGLES, (GLsizei)index_count, GL_UNSIGNED_INT, 0);
        if(material.shader_name == "blinn_phong_pvp")
        {
            m_vao->unbind();
        }

        active_program->unbind();
    }
    glDisable(GL_DEPTH_TEST);
}

void OglRenderer::drawTrianglesIndexed(const u64&                  index_count,
                                       const Material&             material,
                                       const mat4&                 model,
                                       const std::vector<Matrix4>& skin_transformations)
{
    auto active_program = m_blinn_phong_program;
    if(material.shader_name == "blinn_phong_pvp")
    {
        m_vao->bind();
        active_program = m_blinn_phong_pvp_program;
    }
    else if(material.shader_name == "m_blinn_phong_skinned")
    {
        active_program = m_blinn_phong_skinned_program;
    }

    glEnable(GL_DEPTH_TEST);
    {
        mat4 normal_matrix = model;
        normal_matrix      = normal_matrix.inverted().transpose(); // (Transpose of inverse of the model matrix)

        active_program->bind();
        active_program->setMat4("model", model);
        active_program->setMat4("normal_matrix", normal_matrix);
        active_program->setMat4s("skin_transforms", skin_transformations);
        // m_blinn_phong_program->setFloat3("material.ambient", material.ambient);
        active_program->setFloat3("material.diffuse", material.diffuse);
        active_program->setFloat3("material.specular", material.specular);
        active_program->setFloat("material.shininess", material.shininess);
        if(material.diff_texture)
        {
            active_program->setInt("material.diffuse_texture", 0);
            material.diff_texture->bind(0);
        }
        else
        {
            active_program->setInt("material.diffuse_texture", 0);
            m_white_texture->bind(0);
        }
        if(material.spec_texture)
        {
            active_program->setInt("material.specular_texture", 1);
            material.spec_texture->bind(1);
        }
        {
            active_program->setInt("material.specular_texture", 1);
            m_white_texture->bind(1);
        }

        glDrawElements(GL_TRIANGLES, (GLsizei)index_count, GL_UNSIGNED_INT, 0);
        if(material.shader_name == "blinn_phong_pvp")
        {
            m_vao->unbind();
        }

        active_program->unbind();
    }
    glDisable(GL_DEPTH_TEST);
}

void OglRenderer::drawTrianglesInstanced(const u64& vertex_count, const u64& instance_count, const Material& material)
{
    auto active_program = m_blinn_phong_instanced_program;
    // if(material.shader_name == "blinn_phong_pvp")
    //{
    //     m_vao->bind();
    //     active_program = m_blinn_phong_pvp_program;
    // }
    // else if(material.shader_name == "blinn_phong_instanced")
    //{
    //     active_program = m_blinn_phong_instanced_program;
    // }

    glEnable(GL_DEPTH_TEST);
    {
        // mat4 normal_matrix = model;
        // normal_matrix      = normal_matrix.inverted().transpose(); // (Transpose of inverse of the model matrix)

        active_program->bind();
        // active_program->setMat4("model", model);
        // active_program->setMat4("normal_matrix", normal_matrix);
        //  m_blinn_phong_program->setFloat3("material.ambient", material.ambient);
        active_program->setFloat3("material.diffuse", material.diffuse);
        active_program->setFloat3("material.specular", material.specular);
        active_program->setFloat("material.shininess", material.shininess);
        if(material.diff_texture)
        {
            active_program->setInt("material.diffuse_texture", 0);
            material.diff_texture->bind(0);
        }
        else
        {
            active_program->setInt("material.diffuse_texture", 0);
            m_white_texture->bind(0);
        }
        if(material.spec_texture)
        {
            active_program->setInt("material.specular_texture", 1);
            material.spec_texture->bind(1);
        }
        {
            active_program->setInt("material.specular_texture", 1);
            m_white_texture->bind(1);
        }

        glDrawArraysInstanced(GL_TRIANGLES, 0, (GLsizei)vertex_count, (GLsizei)instance_count);

        active_program->unbind();
    }
    glDisable(GL_DEPTH_TEST);
}

void OglRenderer::drawTrianglesIndexedInstanced(const u64&      index_count,
                                                const u64&      instance_count,
                                                const Material& material)
{
    auto active_program = m_blinn_phong_instanced_program;

    glEnable(GL_DEPTH_TEST);
    {
        // mat4 normal_matrix = model;
        // normal_matrix      = normal_matrix.inverted().transpose(); // (Transpose of inverse of the model matrix)

        active_program->bind();
        // active_program->setMat4("model", model);
        // active_program->setMat4("normal_matrix", normal_matrix);
        //  m_blinn_phong_program->setFloat3("material.ambient", material.ambient);
        active_program->setFloat3("material.diffuse", material.diffuse);
        active_program->setFloat3("material.specular", material.specular);
        active_program->setFloat("material.shininess", material.shininess);
        if(material.diff_texture)
        {
            active_program->setInt("material.diffuse_texture", 0);
            material.diff_texture->bind(0);
        }
        else
        {
            active_program->setInt("material.diffuse_texture", 0);
            m_white_texture->bind(0);
        }
        if(material.spec_texture)
        {
            active_program->setInt("material.specular_texture", 1);
            material.spec_texture->bind(1);
        }
        {
            active_program->setInt("material.specular_texture", 1);
            m_white_texture->bind(1);
        }

        glDrawElementsInstanced(GL_TRIANGLES, (GLsizei)index_count, GL_UNSIGNED_INT, 0, (GLsizei)instance_count);

        active_program->unbind();
    }
    glDisable(GL_DEPTH_TEST);
}

void OglRenderer::drawDebugLines(const std::vector<vec4>& vertices, const vec3& color, const float& line_width)
{
    glEnable(GL_DEPTH_TEST);
    glLineWidth(line_width);
    glEnable(GL_LINE_SMOOTH);
    m_vao->bind();

    m_debug_draw_program->bind();
    m_debug_draw_program->setFloat3("color", color);
    m_debug_draw_program->setMat4("model", mat4());

    OglBuffer vbo((void*)vertices.data(), vertices.size() * sizeof(vec4));
    vbo.bind(BufferBindTarget::ShaderStorageBuffer, 0);

    glDrawArrays(GL_LINES, 0, (GLsizei)vertices.size());

    m_debug_draw_program->unbind();
    m_vao->unbind();
    glDisable(GL_LINE_SMOOTH);
    glLineWidth(1.0f);
    glDisable(GL_DEPTH_TEST);
}

void OglRenderer::drawDebugLines(const std::vector<vec4>& vertices,
                                 const vec3&              color,
                                 const mat4&              model,
                                 const float&             line_width)
{
    glEnable(GL_DEPTH_TEST);
    glLineWidth(line_width);
    glEnable(GL_LINE_SMOOTH);
    m_vao->bind();

    m_debug_draw_program->bind();
    m_debug_draw_program->setFloat3("color", color);
    m_debug_draw_program->setMat4("model", model);

    OglBuffer vbo((void*)vertices.data(), vertices.size() * sizeof(vec4));
    vbo.bind(BufferBindTarget::ShaderStorageBuffer, 0);

    glDrawArrays(GL_LINES, 0, (GLsizei)vertices.size());

    m_debug_draw_program->unbind();
    m_vao->unbind();
    glDisable(GL_LINE_SMOOTH);
    glLineWidth(1.0f);
    glDisable(GL_DEPTH_TEST);
}

void OglRenderer::drawDebugLines(const std::vector<vec4>& vertices,
                                 const vec3&              color,
                                 const glm::mat4&         model,
                                 const float&             line_width)
{
    glEnable(GL_DEPTH_TEST);
    glLineWidth(line_width);
    glEnable(GL_LINE_SMOOTH);
    m_vao->bind();

    m_debug_draw_program->bind();
    m_debug_draw_program->setFloat3("color", color);
    m_debug_draw_program->setMat4("model", model);

    OglBuffer vbo((void*)vertices.data(), vertices.size() * sizeof(vec4));
    vbo.bind(BufferBindTarget::ShaderStorageBuffer, 0);

    glDrawArrays(GL_LINES, 0, (GLsizei)vertices.size());

    m_debug_draw_program->unbind();
    m_vao->unbind();
    glDisable(GL_LINE_SMOOTH);
    glLineWidth(1.0f);
    glDisable(GL_DEPTH_TEST);
}

void OglRenderer::drawDebugPoints(const std::vector<vec4>& vertices, const vec3& color, const float& point_size)
{
    glEnable(GL_DEPTH_TEST);
    glPointSize(point_size);
    m_vao->bind();

    m_debug_draw_program->bind();
    m_debug_draw_program->setFloat3("color", color);
    m_debug_draw_program->setMat4("model", mat4());

    // https://www.khronos.org/opengl/wiki/Shader_Storage_Buffer_Object
    OglBuffer vbo((void*)vertices.data(), vertices.size() * sizeof(vec4));
    vbo.bind(BufferBindTarget::ShaderStorageBuffer, 0);

    glDrawArrays(GL_POINTS, 0, (GLsizei)vertices.size());

    m_debug_draw_program->unbind();
    m_vao->unbind();

    glDisable(GL_DEPTH_TEST);
}

void OglRenderer::drawDebugPoints(const std::vector<vec4>& vertices,
                                  const vec3&              color,
                                  const mat4&              model,
                                  const float&             point_size)
{
    glEnable(GL_DEPTH_TEST);
    glPointSize(point_size);
    m_vao->bind();

    m_debug_draw_program->bind();
    m_debug_draw_program->setFloat3("color", color);
    m_debug_draw_program->setMat4("model", model);

    // https://www.khronos.org/opengl/wiki/Shader_Storage_Buffer_Object
    OglBuffer vbo((void*)vertices.data(), vertices.size() * sizeof(vec4));
    vbo.bind(BufferBindTarget::ShaderStorageBuffer, 0);

    glDrawArrays(GL_POINTS, 0, (GLsizei)vertices.size());

    m_debug_draw_program->unbind();
    m_vao->unbind();

    glDisable(GL_DEPTH_TEST);
}

void OglRenderer::drawDebugPoints(const std::vector<vec4>& vertices,
                                  const vec3&              color,
                                  const glm::mat4&         model,
                                  const float&             point_size)
{
    glEnable(GL_DEPTH_TEST);
    glPointSize(point_size);
    m_vao->bind();

    m_debug_draw_program->bind();
    m_debug_draw_program->setFloat3("color", color);
    m_debug_draw_program->setMat4("model", model);

    // https://www.khronos.org/opengl/wiki/Shader_Storage_Buffer_Object
    OglBuffer vbo((void*)vertices.data(), vertices.size() * sizeof(vec4));
    vbo.bind(BufferBindTarget::ShaderStorageBuffer, 0);

    glDrawArrays(GL_POINTS, 0, (GLsizei)vertices.size());

    m_debug_draw_program->unbind();
    m_vao->unbind();

    glDisable(GL_DEPTH_TEST);
}

void OglRenderer::drawArrays(const u64& vertex_count, DrawMode mode)
{
    glDrawArrays(drawModeToGLEnum(mode), 0, (GLsizei)vertex_count);
}

void OglRenderer::drawArraysInstanced(const u64& vertex_count, const u64& instance_count, DrawMode mode)
{
    glDrawArraysInstanced(drawModeToGLEnum(mode), 0, (GLsizei)vertex_count, (GLsizei)instance_count);
}

void OglRenderer::drawElements(const u64& index_count, DrawMode mode)
{
    glDrawElements(drawModeToGLEnum(mode), (GLsizei)index_count, GL_UNSIGNED_INT, 0);
    // glDrawElements(DrawModeToGLEnum(mode), index_count, GL_UNSIGNED_INT, indices); // does not work
    //(GLenum mode, GLsizei count, GLenum type, const void* indices);
}

void OglRenderer::drawElementsInstanced(const u64& index_count, const u64& instance_count, DrawMode mode)
{
    glDrawElementsInstanced(drawModeToGLEnum(mode), (GLsizei)index_count, GL_UNSIGNED_INT, 0, (GLsizei)instance_count);
}

GLenum OglRenderer::drawModeToGLEnum(DrawMode mode)
{
    switch(mode)
    {
    case DrawMode::POINTS:
        return GL_POINTS;

    case DrawMode::LINE_STRIP:
        return GL_LINE_STRIP;

    case DrawMode::LINE_LOOP:
        return GL_LINE_LOOP;

    case DrawMode::LINES:
        return GL_LINES;

    case DrawMode::TRIANGLES:
        return GL_TRIANGLES;

    case DrawMode::TRIANGLE_STRIP:
        return GL_TRIANGLE_STRIP;

    case DrawMode::TRIANGLE_FAN:
        return GL_TRIANGLE_FAN;
    }

    // assert() // invalid draw mode
    return static_cast<unsigned int>(DrawMode::INVALID);
}

} // namespace sputnik::graphics::gl