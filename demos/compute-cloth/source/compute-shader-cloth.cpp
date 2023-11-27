#include "compute-shader-cloth.h"

#include <editor/editor_camera.h>
#include <graphics/api/camera.h>
#include <graphics/glcore/uniform.h>

#include <constants.h>
#include <vector.hpp>
#include <matrix.hpp>

namespace sputnik::demos
{

ComputerShaderClothDemoLayer::ComputerShaderClothDemoLayer(const std::string& name) : core::Layer{name} {}

ComputerShaderClothDemoLayer::~ComputerShaderClothDemoLayer() {}

void ComputerShaderClothDemoLayer::OnAttach()
{
    glGenBuffers(2, m_position_ssbos);
    glGenBuffers(2, m_velocity_ssbos);

    m_compute_integration_shader =
        std::make_shared<sputnik::graphics::glcore::Shader>("../../data/shaders/compute/cloth/cloth_integration.comp");
    // m_compute_normal_shader =
    //     std::make_shared<sputnik::graphics::glcore::Shader>("../../data/shaders/compute/cloth/cloth_normal.comp");
    m_cloth_shader = std::make_shared<sputnik::graphics::glcore::Shader>("../../data/shaders/compute/cloth/cloth.vert",
                                                                         "../../data/shaders/compute/cloth/cloth.frag");

    m_particles_shader =
        std::make_shared<sputnik::graphics::glcore::Shader>("../../data/shaders/compute/cloth/cloth_particles.vert",
                                                            "../../data/shaders/compute/cloth/cloth_particles.frag");

    mat4 transform{};
    transform = transform.translate({0.0f, real(m_cloth_dimensions.y), 0.0f});
    transform = transform.rotate({1.0f, 0.0f, 0.0f}, Constants::DEG_TO_RAD * (-80.0f));
    transform = transform.translate({0.0f, real(-m_cloth_dimensions.y), 0.0f});

    float dx = real(m_cloth_dimensions.x) / (m_num_particles.x - 1);
    float dy = real(m_cloth_dimensions.y) / (m_num_particles.y - 1);
    float ds = 1.0f / (m_num_particles.x - 1);
    float dt = 1.0f / (m_num_particles.y - 1);
    for(unsigned int row = 0; row < m_num_particles.y; ++row)
    {
        for(unsigned int col = 0; col < m_num_particles.x; ++col)
        {
            vec3 pos = transform.transformVector(vec3{col * dx, row * dy, 0.0f});
            m_initial_positions.push_back({pos.x, pos.y, pos.z, 1.0f});
            m_texture_coords.push_back({col * ds, row * dt});

            if(row < m_num_particles.y - 1)
            {
                unsigned current_index = row * m_num_particles.x + col;

                if(col == 0)
                {
                    m_indices.push_back(current_index);                     // current index
                    m_indices.push_back(current_index + 1);                 // right
                    m_indices.push_back(current_index + m_num_particles.x); // up
                }
                else if(col == m_num_particles.x - 1)
                {
                    m_indices.push_back(current_index);                         // current index
                    m_indices.push_back(current_index + m_num_particles.x);     // up
                    m_indices.push_back(current_index + m_num_particles.x - 1); // up and left
                }
                else
                {

                    m_indices.push_back(current_index);                         // current index
                    m_indices.push_back(current_index + m_num_particles.x);     // up
                    m_indices.push_back(current_index + m_num_particles.x - 1); // up and left

                    m_indices.push_back(current_index);                     // current index
                    m_indices.push_back(current_index + 1);                 // right
                    m_indices.push_back(current_index + m_num_particles.x); // up
                }
            }
        }
    }

    m_buffer_size = m_num_particles.x * m_num_particles.y * 4 * sizeof(float);

    std::vector<vec4> initial_velocity(m_num_particles.x * m_num_particles.y);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_position_ssbos[0]); // bind the buffer to the binding point 0
    glBufferData(GL_SHADER_STORAGE_BUFFER, m_buffer_size, &m_initial_positions[0], GL_DYNAMIC_DRAW);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_position_ssbos[1]); // bind the buffer to the binding point 1
    glBufferData(GL_SHADER_STORAGE_BUFFER, m_buffer_size, NULL, GL_DYNAMIC_DRAW);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_velocity_ssbos[0]); // bind the buffer to the binding point 2
    glBufferData(GL_SHADER_STORAGE_BUFFER, m_buffer_size, &initial_velocity[0], GL_DYNAMIC_DRAW);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_velocity_ssbos[1]); // bind the buffer to the binding point 3
    glBufferData(GL_SHADER_STORAGE_BUFFER, m_buffer_size, NULL, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &m_particle_grid_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_particle_grid_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_initial_positions.size() * sizeof(vec4), &m_initial_positions[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &m_texture_coord_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_texture_coord_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_texture_coords.size() * sizeof(vec2), &m_texture_coords[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &m_element_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    m_color_texture   = std::make_shared<Texture>("../../data/assets/fabric_basecolor.jpg");
    m_opacity_texture = std::make_shared<Texture>("../../data/assets/fabric_opacity.jpg");
}

void ComputerShaderClothDemoLayer::OnDetach()
{
    glDeleteBuffers(2, m_position_ssbos);
    glDeleteBuffers(2, m_velocity_ssbos);
    glDeleteBuffers(1, &m_particle_grid_vbo);
    glDeleteBuffers(1, &m_texture_coord_vbo);
}

void ComputerShaderClothDemoLayer::OnUpdate(const core::TimeStep& time_step)
{
    // update the positions and velocities

    // const auto& camera = sputnik::graphics::api::Camera::GetInstance();
    const auto& camera     = sputnik::graphics::api::EditorCamera::GetInstance();
    const auto& projection = camera->GetCameraPerspective();
    const auto& view       = camera->GetCameraView();

    // draw the particle grid
    {
        m_particles_shader->Bind();

        mat4 model{};
        Uniform<mat4>::Set(m_particles_shader->GetUniform("model"), model);
        Uniform<mat4>::Set(m_particles_shader->GetUniform("view"), view);
        Uniform<mat4>::Set(m_particles_shader->GetUniform("projection"), projection);
        Uniform<vec4>::Set(m_particles_shader->GetUniform("color"), m_particle_color);
        glBindBuffer(GL_ARRAY_BUFFER, m_particle_grid_vbo);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glBufferData(GL_ARRAY_BUFFER, m_buffer_size, &m_initial_positions[0], GL_DYNAMIC_DRAW);
        glPointSize(2.5f);
        // glDrawArrays(GL_POINTS, 0, m_num_particles.x * m_num_particles.y); // render the particles

        m_particles_shader->Unbind();
    }

    // update the positions and velocities
    {
        m_compute_integration_shader->Bind(); // Bind the compute shader
        for(int i = 0; i < 5; i++)
        {
            glDispatchCompute(m_num_particles.x / 10, m_num_particles.y / 10, 1);
            glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

            // swap the read and write buffers
            m_read_buffer_index = 1 - m_read_buffer_index;
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_position_ssbos[m_read_buffer_index]);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_position_ssbos[1 - m_read_buffer_index]);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_velocity_ssbos[m_read_buffer_index]);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_velocity_ssbos[1 - m_read_buffer_index]);
        }

        m_compute_integration_shader->Unbind(); // Unbind the compute shader
    }

    // draw the cloth
    {
        // glDisable(GL_DEPTH_TEST);
        //   render any debug visuals
        m_cloth_shader->Bind();

        mat4 model{};
        Uniform<mat4>::Set(m_cloth_shader->GetUniform("model"), model);
        Uniform<mat4>::Set(m_cloth_shader->GetUniform("view"), view);
        Uniform<mat4>::Set(m_cloth_shader->GetUniform("projection"), projection);
        // Uniform<vec4>::Set(m_draw_shader->GetUniform("color"), m_particle_color);
        glBindBuffer(GL_ARRAY_BUFFER, m_position_ssbos[1 - m_read_buffer_index]);
        // glBindBuffer(GL_ARRAY_BUFFER, m_particle_grid_vbo);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glBindBuffer(GL_ARRAY_BUFFER, m_texture_coord_vbo);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        m_color_texture->Set(m_cloth_shader->GetUniform("color_texture"), 0);
        m_opacity_texture->Set(m_cloth_shader->GetUniform("opacity_texture"), 1);
        glPointSize(2.5f);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_element_buffer);
        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

        // glDrawArrays(GL_POINTS, 0, m_num_particles.x * m_num_particles.y); // render the particles

        m_cloth_shader->Unbind();
        // glEnable(GL_DEPTH_TEST);
    }

    // draw the particles
    {
        glDisable(GL_DEPTH_TEST);
        //  render any debug visuals
        m_particles_shader->Bind();

        mat4 model{};
        Uniform<mat4>::Set(m_particles_shader->GetUniform("model"), model);
        Uniform<mat4>::Set(m_particles_shader->GetUniform("view"), view);
        Uniform<mat4>::Set(m_particles_shader->GetUniform("projection"), projection);
        Uniform<vec4>::Set(m_particles_shader->GetUniform("color"), m_particle_color);
        glBindBuffer(GL_ARRAY_BUFFER, m_position_ssbos[1 - m_read_buffer_index]);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glPointSize(2.0f);

        glDrawArrays(GL_POINTS, 0, m_num_particles.x * m_num_particles.y); // render the particles

        m_particles_shader->Unbind();
        glEnable(GL_DEPTH_TEST);
    }
}

void ComputerShaderClothDemoLayer::OnEvent() {}

void ComputerShaderClothDemoLayer::OnUpdateUI(const core::TimeStep& time_step) {}

} // namespace sputnik::demos
