#pragma once

#include <main/entry_point.h>
#include <main/application.h>
#include <core/layers/layer.h>
#include <core/time_step.h>
#include <graphics/glcore/shader.h>
#include <graphics/glcore/texture.h>

#include <vector2.h>
#include <vector3.h>
#include <vector.hpp>

#include <vector>
#include <memory>

namespace sputnik::demos
{

using namespace ramanujan;
using namespace ramanujan::experimental;
using namespace sputnik::graphics::glcore;

class ComputerShaderClothDemoLayer : public core::Layer
{

public:
    ComputerShaderClothDemoLayer(const std::string& name);
    virtual ~ComputerShaderClothDemoLayer();

    virtual void OnAttach();
    virtual void OnDetach();
    virtual void OnUpdate(const core::TimeStep& time_step);
    virtual void OnEvent();
    virtual void OnUpdateUI(const core::TimeStep& time_step);

private:
    // Shader storage buffer objects
    // We'll lay out the particle positions/velocities in row-major order starting at the lower left and proceeding to
    // the upper right of the lattice.
    unsigned int m_position_ssbos[2];
    unsigned int m_velocity_ssbos[2];
    unsigned int m_read_buffer_index = 0;
    unsigned int m_particle_grid_vbo;
    unsigned int m_texture_coord_vbo;
    unsigned int m_element_buffer;

    vec4 m_particle_color{0.0f, 1.0f, 0.0f, 1.0f};

    uvec2 m_cloth_dimensions{3, 4};
    uvec2 m_num_particles{40, 40};
    // uvec2                     m_num_particles{4, 4};
    std::vector<vec4>         m_initial_positions;
    std::vector<vec2>         m_texture_coords;
    std::vector<unsigned int> m_indices;
    unsigned int              m_buffer_size;

    std::shared_ptr<Texture> m_color_texture;
    std::shared_ptr<Texture> m_normal_texture;
    std::shared_ptr<Texture> m_opacity_texture;

    std::shared_ptr<Shader> m_compute_integration_shader;
    std::shared_ptr<Shader> m_compute_normal_shader;
    std::shared_ptr<Shader> m_cloth_shader;
    std::shared_ptr<Shader> m_particles_shader;
};

class ComputerShaderClothDemo : public sputnik::main::Application
{

public:
    ComputerShaderClothDemo(const std::string& name) : sputnik::main::Application(name)
    {
        PushLayer(std::make_shared<ComputerShaderClothDemoLayer>(name));
    }

    ~ComputerShaderClothDemo() {}
};

} // namespace sputnik::demos

sputnik::main::Application* sputnik::main::CreateApplication()
{
    return new sputnik::demos::ComputerShaderClothDemo("Computer Shader Cloth Demo");
}
