#include "compute_shader_demo.h"

#include <graphics/glcore/uniform.h>
#include <graphics/glcore/renderer.h>
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
    m_compute_shader = std::make_shared<sputnik::graphics::glcore::Shader>("../../data/shaders/simple.comp");
    m_fs_quad_shader = std::make_shared<sputnik::graphics::glcore::Shader>("../../data/shaders/fullscreen_vs.glsl",
                                                                           "../../data/shaders/fullscreen_fs.glsl");

    // Create a texture
    glGenTextures(1, &m_texture_id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);

    // (GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format)
    glBindImageTexture(0, m_texture_id, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
}

// Todo: On Detach must be called at system cleanup before shutdown
void ComputeShaderDemo::OnDetach() {}

void ComputeShaderDemo::OnUpdate(const core::TimeStep& time_step)
{
    m_compute_shader->Bind();                            // Bind the compute shader
    glDispatchCompute(TEXTURE_WIDTH, TEXTURE_HEIGHT, 1); // Dispatch compute shader
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT); // make sure writing to image has finished before read
    m_compute_shader->Unbind();                          // Unbind the compute shader

    // Just testing fullscreen quad rendering
    m_fs_quad_shader->Bind();                                  // Bind the fullscreen quad shader
    glActiveTexture(GL_TEXTURE0);                              // Activate texture unit 0
    glBindTexture(GL_TEXTURE_2D, m_texture_id);                // Bind the texture
    glUniform1i(m_fs_quad_shader->GetUniform("s_texture"), 0); // Set the sampler uniform to texture unit 0
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);                     // Draw the fullscreen quad
    m_fs_quad_shader->Unbind();                                // Unbind the fullscreen quad shader
}

void ComputeShaderDemo::OnEvent() {}

void ComputeShaderDemo::OnUpdateUI(const core::TimeStep& time_step)
{
    if(ImGui::Begin("Compute Texture"))
    {
        ImGui::Image(reinterpret_cast<void*>(m_texture_id),
                     ImVec2{static_cast<float>(TEXTURE_WIDTH), static_cast<float>(TEXTURE_HEIGHT)},
                     ImVec2{0, 1},
                     ImVec2{1, 0});
    }
    ImGui::End();
}

} // namespace sputnik::demos