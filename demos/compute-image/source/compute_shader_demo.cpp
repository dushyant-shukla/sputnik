#include "compute_shader_demo.h"

#include <graphics/glcore/gltf_loader.h>
#include <matrix4.h>
#include <vector3.h>
#include <camera_transforms.h>
#include <imgui.h>
#include <graphics/core/animation/rearrange_bones.h>
#include <editor/editor_camera.h>

#include <glad/glad.h>

namespace sputnik::demos
{

ComputeShaderDemo::ComputeShaderDemo(const std::string& name) : core::Layer(name) {}

ComputeShaderDemo::~ComputeShaderDemo() {}

void ComputeShaderDemo::OnAttach()
{
    m_vao = std::make_unique<OglVertexArray>();

    m_compute_shader = std::make_shared<OglShaderProgram>();
    m_compute_shader->addShaderStage("../../data/shaders/compute/simple.comp");
    m_compute_shader->configure();

    m_fs_quad_shader = std::make_shared<OglShaderProgram>();
    m_fs_quad_shader->addShaderStage("../../data/shaders/glsl/fullscreen.vert");
    m_fs_quad_shader->addShaderStage("../../data/shaders/glsl/fullscreen.frag");
    m_fs_quad_shader->configure();

    // Create a texture
    TextureSpecification texture_spec;
    texture_spec.width          = TEXTURE_WIDTH;
    texture_spec.height         = TEXTURE_HEIGHT;
    texture_spec.texture_format = TextureFormat::RGBA32;
    texture_spec.r_wrap         = TextureWrap::ClampToEdge;
    texture_spec.s_wrap         = TextureWrap::ClampToEdge;
    texture_spec.t_wrap         = TextureWrap::ClampToEdge;
    texture_spec.min_filter     = TextureFilter::Linear;
    texture_spec.mag_filter     = TextureFilter::Linear;
    m_image_texture             = std::make_shared<OglTexture2D>(texture_spec);

    // (GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format)
    glBindImageTexture(0, m_image_texture->getId(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
}

// Todo: On Detach must be called at system cleanup before shutdown
void ComputeShaderDemo::OnDetach() {}

void ComputeShaderDemo::OnUpdate(const core::TimeStep& time_step)
{
    m_compute_shader->bind();                            // Bind the compute shader
    glDispatchCompute(TEXTURE_WIDTH, TEXTURE_HEIGHT, 1); // Dispatch compute shader
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT); // make sure writing to image has finished before read
    m_compute_shader->unbind();                          // Unbind the compute shader

    // Just testing fullscreen quad rendering ;-D
    m_vao->bind();                            // Bind the fullscreen quad vertex array object
    m_fs_quad_shader->bind();                 // Bind the fullscreen quad shader
    m_image_texture->bind(0);                 // Bind the texture to the shader
    m_fs_quad_shader->setInt("s_texture", 0); // Set the texture unit to 0
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);    // Draw the fullscreen quad
    m_fs_quad_shader->unbind();               // Unbind the fullscreen quad shader
    m_vao->unbind();                          // Unbind the fullscreen quad vertex array object
}

void ComputeShaderDemo::OnEvent() {}

void ComputeShaderDemo::OnUpdateUI(const core::TimeStep& time_step)
{
    if(ImGui::Begin("Compute Texture"))
    {
        ImGui::Image(reinterpret_cast<void*>(m_image_texture->getId()),
                     ImVec2{static_cast<float>(TEXTURE_WIDTH), static_cast<float>(TEXTURE_HEIGHT)},
                     ImVec2{0, 1},
                     ImVec2{1, 0});
    }
    ImGui::End();
}

} // namespace sputnik::demos