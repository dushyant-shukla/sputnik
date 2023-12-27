#include "pch.h"
#include "gl_renderer.h"
#include "editor/editor_camera.h"
#include "graphics/glcore/gl_shader.h"

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
        std::cout << "Failed to initialize Glad!";
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

    m_sky_shader = std::make_shared<OglShaderProgram>();
    m_sky_shader->addShaderStage("../../data/shaders/sky-rendering/sky.vert");
    m_sky_shader->addShaderStage("../../data/shaders/sky-rendering/sky.frag");
    m_sky_shader->configure();

    m_light_direction = vec3(0.0f, sin(m_sun_angle), cos(m_sun_angle)).normalized();

    glGenVertexArrays(1, &m_vao);
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

void OglRenderer::resizeViewport(const u32& width, const u32& height) {}

void OglRenderer::resizeViewport(const u32& x, const u32& y, const u32& width, const u32& height) {}

// TODO:: a bunch of draw call apis (indirect/canonical)
void OglRenderer::draw() {}

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
    glBindVertexArray(m_vao); // Todo:: Temporary solution

    m_preetham_sky_model.SetDirection(m_light_direction);
    m_preetham_sky_model.Update();

    mat4 camera_projection{};
    mat4 camera_view{};
    vec3 camera_position{};

    //if(m_camera_type == CameraType::Camera)
    //{
    //    camera_projection = renderer->m_camera->GetCameraPerspective();
    //    camera_view       = renderer->m_camera->GetCameraView();
    //    camera_position   = renderer->m_camera->GetCameraPosition();
    //}
    //else
    //{
    //    camera_projection = renderer->m_editor_camera->GetCameraPerspective();
    //    camera_view       = renderer->m_editor_camera->GetCameraView();
    //    camera_position   = renderer->m_editor_camera->GetCameraPosition();
    //}

    auto m_editor_camera   = EditorCamera::GetInstance();
    camera_projection = m_editor_camera->GetCameraPerspective();
    camera_view       = m_editor_camera->GetCameraView();
    camera_position   = m_editor_camera->GetCameraPosition();

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

    m_sky_shader->bind();

    // Uniform<mat4>::Set(renderer->m_sky_shader->GetUniform("inv_view_projection"), inv_cubemap_view_projection_mat4);

    m_sky_shader->setMat4("inv_view_projection", inv_cubemap_view_projection_mat4);

    m_preetham_sky_model.SetRenderUniforms(m_sky_shader);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    m_sky_shader->unbind();

    // reset the pipeline state
    glDepthFunc(GL_LESS); // This is the default depth function
    glDisable(GL_DEPTH_TEST);

    glBindVertexArray(0); // Todo:: Temporary solution
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