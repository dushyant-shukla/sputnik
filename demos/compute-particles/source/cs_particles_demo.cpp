#include "cs_particles_demo.h"

// #include <graphics/glcore/uniform.h>
// #include <graphics/glcore/renderer.h>
#include <graphics/glcore/gltf_loader.h>
#include <matrix4.h>
#include <vector3.h>
#include <camera_transforms.h>
#include <imgui.h>
#include <graphics/core/animation/rearrange_bones.h>
#include <editor/editor_camera.h>
#include <graphics/api/camera.h>

#include <matrix.hpp>

#include <glad/glad.h>

namespace sputnik::demos
{

ComputeShaderParticlesDemo::ComputeShaderParticlesDemo(const std::string& name) : core::Layer(name) {}

ComputeShaderParticlesDemo::~ComputeShaderParticlesDemo() {}

void ComputeShaderParticlesDemo::OnAttach()
{

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // default

    // sputnik::graphics::api::EditorCamera::GetInstance()->SetPosition({0.0f, -5.0f, 5.0f});

    // m_particle_compute_shader =
    //     std::make_shared<sputnik::graphics::glcore::Shader>("../../data/shaders/compute/particles/particles.comp");

    m_vao = std::make_unique<OglVertexArray>();

    m_particle_compute_shader = std::make_shared<OglShaderProgram>();
    m_particle_compute_shader->addShaderStage("../../data/shaders/compute/particles/particles.comp");
    m_particle_compute_shader->configure();

    m_particle_draw_shader = std::make_shared<OglShaderProgram>();
    m_particle_draw_shader->addShaderStage("../../data/shaders/compute/particles/particles.vert");
    m_particle_draw_shader->addShaderStage("../../data/shaders/compute/particles/particles.frag");
    m_particle_draw_shader->configure();

    // each particle has a position (x, y, z, w)
    unsigned int buffer_size = TOTAL_PARTICLE_COUNT * 4 * sizeof(float);

    // Generate the buffers
    // unsigned int ssbos[2];
    glGenBuffers(2, ssbos);

    std::vector<ramanujan::Vector4> initial_positions;
    float                           dx = 2.0f / (PARTICLE_COUNT_X - 1);
    float                           dy = 2.0f / (PARTICLE_COUNT_Y - 1);
    float                           dz = 2.0f / (PARTICLE_COUNT_Z - 1);

    // we want to center the particles at {0, 0, 0}}
    mat4 transform{};
    transform = transform.translate({-1.0f, -1.0f, -1.0f});

    for(unsigned int i = 0; i < PARTICLE_COUNT_X; ++i)
    {
        for(unsigned int j = 0; j < PARTICLE_COUNT_Y; ++j)
        {
            for(unsigned int k = 0; k < PARTICLE_COUNT_Z; ++k)
            {
                vec3 pos = transform.transformVector(vec3{i * dx, j * dy, k * dz});
                // vec3 pos{i * dx, j * dy, k * dz};
                initial_positions.push_back({pos.x, pos.y, pos.z, 1.0f});
                // initial_positions.push_back(pos.y);
                // initial_positions.push_back(pos.z);
                // initial_positions.push_back(1.0f);
            }
        }
    }

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbos[0]); // bind the buffer to the binding point 0
    glBufferData(GL_SHADER_STORAGE_BUFFER, buffer_size, &initial_positions[0], GL_DYNAMIC_DRAW);

    // std::vector<float> initial_velocities(TOTAL_PARTICLE_COUNT * 4, 0.0f);
    std::vector<ramanujan::Vector4> initial_velocities(TOTAL_PARTICLE_COUNT, 0.0f);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbos[1]); // bind the buffer to the binding point 1
    glBufferData(GL_SHADER_STORAGE_BUFFER, buffer_size, &initial_velocities[0], GL_DYNAMIC_DRAW);

    // glBindBuffer(GL_ARRAY_BUFFER, ssbos[0]);
    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glGenBuffers(1, &attractor_vbo);
}

// Todo: On Detach must be called at system cleanup before shutdown
void ComputeShaderParticlesDemo::OnDetach() {}

void ComputeShaderParticlesDemo::OnUpdate(const core::TimeStep& time_step)
{
    // updating particle positions
    {
        m_particle_compute_shader->bind();                       // Bind the compute shader
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbos[0]); // bind the ssbo for positions
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbos[1]); // bind the ssbo for velocities
        glDispatchCompute(TOTAL_PARTICLE_COUNT / 1000, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
        m_particle_compute_shader->unbind(); // Unbind the compute shader
    }

    // const auto& camera        = sputnik::graphics::api::Camera::GetInstance();
    const auto& camera     = sputnik::graphics::api::EditorCamera::GetInstance();
    const auto& projection = camera->GetCameraPerspective();
    const auto& view       = camera->GetCameraView();

    m_particle_draw_shader->bind();
    m_vao->bind();
    mat4 model{};
    // Uniform<mat4>::Set(m_particle_draw_shader->GetUniform("model"), model);
    // Uniform<mat4>::Set(m_particle_draw_shader->GetUniform("view"), view);
    // Uniform<mat4>::Set(m_particle_draw_shader->GetUniform("projection"), projection);

    m_particle_draw_shader->setMat4("model", model);
    m_particle_draw_shader->setMat4("view", view);
    m_particle_draw_shader->setMat4("projection", projection);

    // render particles
    {
        glBindBuffer(GL_ARRAY_BUFFER, ssbos[0]); // send the data from position ssbo to the vertex shader
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
        // Uniform<Vector4>::Set(m_particle_draw_shader->GetUniform("color"), m_particle_color);
        ramanujan::experimental::vec4 color = m_particle_color;
        m_particle_draw_shader->setFloat4("color", color);
        glPointSize(1.0f);
        glDrawArrays(GL_POINTS, 0, TOTAL_PARTICLE_COUNT); // render the particles
    }

    // render attractors
    {
        angle += speed * time_step.GetSeconds();
        if(angle > 360.0f)
        {
            angle -= 360.0f;
        }
        mat4 rotation{};
        rotation                = rotation.rotate({0.0f, 0.0f, 1.0f}, Constants::DEG_TO_RAD * angle);
        vec3 attactor1_position = rotation.transformVector(m_attactor1_position);
        vec3 attactor2_position = rotation.transformVector(m_attactor2_position);

        glBindBuffer(GL_ARRAY_BUFFER, attractor_vbo);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
        float data[] = {attactor1_position.x,
                        attactor1_position.y,
                        attactor1_position.z,
                        1.0f,
                        attactor2_position.x,
                        attactor2_position.y,
                        attactor2_position.z,
                        1.0f};
        glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);
        // Uniform<Vector4>::Set(m_particle_draw_shader->GetUniform("color"), m_attractor_color);
        ramanujan::experimental::vec4 color = m_attractor_color;
        m_particle_draw_shader->setFloat4("color", color);
        glPointSize(10.0f);
        glDrawArrays(GL_POINTS, 0, 2); // render the attractors
    }

    m_vao->unbind();
    m_particle_draw_shader->unbind();
}

void ComputeShaderParticlesDemo::OnEvent() {}

void ComputeShaderParticlesDemo::OnUpdateUI(const core::TimeStep& time_step)
{
    // if(ImGui::Begin("Compute Texture"))
    //{
    //     ImGui::Image(reinterpret_cast<void*>(m_texture_id),
    //                  ImVec2{static_cast<float>(TEXTURE_WIDTH), static_cast<float>(TEXTURE_HEIGHT)},
    //                  ImVec2{0, 1},
    //                  ImVec2{1, 0});
    // }
    // ImGui::End();

    if(ImGui::Begin("Compute Particles"))
    {
        ImGui::ColorEdit4("Particle color", &m_particle_color.x);
        ImGui::ColorEdit4("Attractor color", &m_attractor_color.x);
    }
    ImGui::End();
}

} // namespace sputnik::demos