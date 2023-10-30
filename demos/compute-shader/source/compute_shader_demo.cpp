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
    // Bind the compute shader
    m_compute_shader->Bind();

    // Dispatch compute shader
    glDispatchCompute(TEXTURE_WIDTH, TEXTURE_HEIGHT, 1);

    // make sure writing to image has finished before read
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    // Unbind the compute shader
    m_compute_shader->Unbind();

    //// Bind the fullscreen quad shader
    // m_fs_quad_shader->Bind();

    //// Bind the texture
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, m_texture_id);
    // glUniform1i(m_fs_quad_shader->GetUniform("s_texture"), 0); // 0 is texture unit

    //// Draw the fullscreen quad
    //// glDrawArrays(GL_TRIANGLES, 0, 6);
    // glDrawArrays(GL_TRIANGLES, 0, 3);

    //// Unbind the fullscreen quad shader
    // m_fs_quad_shader->Unbind();
}

void ComputeShaderDemo::OnEvent() {}

void ComputeShaderDemo::OnUpdateUI(const core::TimeStep& time_step)
{
    if(ImGui::Begin("Compute Texture"))
    {
        // uint64_t textureID = m_framebuffer->GetColorAttachmentRendererId(1);
        ImGui::Image(reinterpret_cast<void*>(m_texture_id),
                     ImVec2{static_cast<float>(TEXTURE_WIDTH), static_cast<float>(TEXTURE_HEIGHT)},
                     ImVec2{0, 1},
                     ImVec2{1, 0});
    }
    ImGui::End();
}

} // namespace sputnik::demos