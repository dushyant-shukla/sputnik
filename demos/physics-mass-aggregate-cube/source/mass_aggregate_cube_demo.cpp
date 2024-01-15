#include "mass_aggregate_cube_demo.h"

// #include <graphics/api/renderer.h>
// #include <graphics/glcore/uniform.h>
#include <graphics/glcore/gl_buffer.h>
#include <graphics/api/color_material.h>
#include <core/systems/render_system.h>
#include <editor/editor.hpp>

#include <core/logging/logging_core.h>
#include <physics/physics_core.h>

#include <imgui.h>
#include <ImGuizmo.h>

#include <core/core.h>

namespace sputnik::demos
{

MassAggregateCubeDemoLayer::MassAggregateCubeDemoLayer(const std::string& name) : core::Layer{name}
{
    m_cube_specification.mass       = 0.75f;
    m_cube_specification.scale      = {2.0f, 1.0f, 1.0f};
    m_cube_specification.resolution = {20, 10, 10};
    // m_cube_specification.scale             = {1.0f, 1.0f, 1.0f};
    // m_cube_specification.resolution        = {5, 5, 5};
    m_cube_specification.center_position = {0.0f, 7.0f, 0.0f};
    m_cube_specification.damping         = 0.005f;
    m_cube_specification.spring_shear    = {.stiffness_coefficient = 1000.0f, .damping_coefficient = 0.05f};
    m_cube_specification.spring_flexion  = {.stiffness_coefficient = 1000.0f, .damping_coefficient = 0.05f};

    // structural springs are usually stiffer than shear,flexion springs
    m_cube_specification.spring_structural = {.stiffness_coefficient = 2000.0f, .damping_coefficient = 0.05f};

    m_mass_spring_volume = std::make_shared<::physics::mad::MassAggregateVolume>(m_cube_specification);
    m_structural_spring  = m_mass_spring_volume->getStructuralSprings();
    m_shear_spring       = m_mass_spring_volume->getShearSprings();
    m_bend_spring        = m_mass_spring_volume->getBendSprings();

    u64 instanced_buffer_size = m_cube_specification.resolution.x * m_cube_specification.resolution.y *
                                m_cube_specification.resolution.z * 2 * sizeof(mat4);
    m_instanced_buffer = std::make_shared<OglBuffer>(instanced_buffer_size);

    // A temporary solution for instanced rendering the paritcles
    m_sphere          = Model::LoadModel("../../data/assets/hatch_sphere/scene.gltf");
    auto vertex_array = m_sphere->getVertexArray();
    vertex_array->addVertexBuffer(
        *m_instanced_buffer.get(),
        {.binding_index = 3, .stride = 2 * sizeof(mat4)},
        {{.name = "model[0]", .location = 3, .type = VertexAttributeType::Float4, .normalized = false, .divisor = 1},
         {.name = "model[1]", .location = 4, .type = VertexAttributeType::Float4, .normalized = false, .divisor = 1},
         {.name = "model[2]", .location = 5, .type = VertexAttributeType::Float4, .normalized = false, .divisor = 1},
         {.name = "model[3]", .location = 6, .type = VertexAttributeType::Float4, .normalized = false, .divisor = 1},
         {.name       = "normal_matrix[0]",
          .location   = 7,
          .type       = VertexAttributeType::Float4,
          .normalized = false,
          .divisor    = 1},
         {.name       = "normal_matrix[1]",
          .location   = 8,
          .type       = VertexAttributeType::Float4,
          .normalized = false,
          .divisor    = 1},
         {.name       = "normal_matrix[2]",
          .location   = 9,
          .type       = VertexAttributeType::Float4,
          .normalized = false,
          .divisor    = 1},
         {.name       = "normal_matrix[3]",
          .location   = 10,
          .type       = VertexAttributeType::Float4,
          .normalized = false,
          .divisor    = 1}});
}

MassAggregateCubeDemoLayer::~MassAggregateCubeDemoLayer() {}

void MassAggregateCubeDemoLayer::OnAttach()
{

    m_render_system = core::systems::RenderSystem::getInstance();
    auto& light     = m_render_system->getLight();
    light.position  = vec3(10.0f, 7.0f, 0.0f);
    light.ambient   = vec3(1.0f, 1.0f, 1.0f);
    light.diffuse   = vec3(1.0f, 1.0f, 1.0f);
    light.specular  = vec3(1.0f, 1.0f, 1.0f);

    // ENGINE_INFO("Reading back the index...");
    // for(unsigned slice_idx = 0; slice_idx < m_cube_specification.resolution.z; ++slice_idx)
    //{
    //     for(unsigned row_idx = 0; row_idx < m_cube_specification.resolution.y; ++row_idx)
    //     {
    //         for(unsigned col_idx = 0; col_idx < m_cube_specification.resolution.x; ++col_idx)
    //         {
    //             auto index = m_mass_spring_volume->getIndex(row_idx, col_idx, slice_idx);
    //             ENGINE_INFO("Row: {}, Column: {}, Slide: {}, Particle index: {}", row_idx, col_idx, slice_idx,
    //             index);
    //         }
    //     }
    // }

    // ENGINE_INFO("Reading back the local coordinates...");
    // size_t size = m_mass_spring_volume->getParticleCount();
    // for(size_t i = 0; i < size; ++i)
    //{
    //     uvec3 coords = m_mass_spring_volume->getLocalCoordinates(i);
    //     ENGINE_INFO("Row: {}, Column: {}, Slide: {}, Particle index: {}", coords.x, coords.y, coords.z, i);
    // }
}

void MassAggregateCubeDemoLayer::OnDetach() {}

void MassAggregateCubeDemoLayer::OnUpdate(const core::TimeStep& time_step)
{

    if(time_step.GetSeconds() <= Constants::EPSILON)
    {
        return;
    }

    // for(unsigned slice_idx = 0; slice_idx < m_cube_specification.resolution.z; ++slice_idx)
    //{
    //     for(unsigned row_idx = 0; row_idx < m_cube_specification.resolution.y; ++row_idx)
    //     {
    //         for(unsigned col_idx = 0; col_idx < m_cube_specification.resolution.x; ++col_idx)
    //         {
    //             vec3 position = m_mass_spring_volume->getPosition(row_idx, col_idx, slice_idx);
    //             mat4 model{};
    //             model = model.translate(position);
    //             model = model.scale(vec3(0.0004f));
    //             m_sphere->draw(material_blue_shine, model);
    //         }
    //     }
    // }

    if(m_render_particles)
    {
        std::vector<mat4> matrices;
        for(unsigned slice_idx = 0; slice_idx < m_cube_specification.resolution.z; ++slice_idx)
        {
            for(unsigned row_idx = 0; row_idx < m_cube_specification.resolution.y; ++row_idx)
            {
                for(unsigned col_idx = 0; col_idx < m_cube_specification.resolution.x; ++col_idx)
                {
                    vec3 position = m_mass_spring_volume->getPosition(row_idx, col_idx, slice_idx);
                    mat4 model{};
                    model = model.translate(position);
                    model = model.scale(vec3(0.0004f));

                    // (Transpose of inverse of the model matrix)
                    mat4 normal_matrix = model;
                    normal_matrix      = normal_matrix.inverted().transpose();

                    matrices.push_back(model);
                    matrices.push_back(normal_matrix);
                }
            }
        }
        m_instanced_buffer->setData(matrices.data(), matrices.size() * sizeof(mat4));
        material_blue_shine.shader_name = "blinn_phong_instanced";
        m_sphere->drawInstanced(material_emerald,
                                (u32)(m_cube_specification.resolution.x * m_cube_specification.resolution.y *
                                      m_cube_specification.resolution.z));
        matrices.clear();
    }

    std::vector<vec4> lines;
    if(m_render_structural_springs)
    {
        for(const auto& spring : m_structural_spring.getSprings())
        {
            uvec3 coords     = m_mass_spring_volume->getLocalCoordinates(spring.mass_a_idx);
            vec3  position_a = m_mass_spring_volume->getPosition(coords.x, coords.y, coords.z);
            coords           = m_mass_spring_volume->getLocalCoordinates(spring.mass_b_idx);
            vec3 position_b  = m_mass_spring_volume->getPosition(coords.x, coords.y, coords.z);
            lines.push_back({position_a, 1.0f});
            lines.push_back({position_b, 1.0f});
        }
        if(!lines.empty())
        {
            m_render_system->drawDebugLines(lines, {1.0f, 0.0f, 0.0f}, 5.0f);
        }
    }

    if(m_render_shear_springs)
    {
        lines.clear();
        for(const auto& pair : m_shear_spring.getSprings())
        {
            uvec3 coords     = m_mass_spring_volume->getLocalCoordinates(pair.mass_a_idx);
            vec3  position_a = m_mass_spring_volume->getPosition(coords.x, coords.y, coords.z);
            coords           = m_mass_spring_volume->getLocalCoordinates(pair.mass_b_idx);
            vec3 position_b  = m_mass_spring_volume->getPosition(coords.x, coords.y, coords.z);
            lines.push_back({position_a, 1.0f});
            lines.push_back({position_b, 1.0f});
        }
        if(!lines.empty())
        {
            m_render_system->drawDebugLines(lines, material_blue_shine.diffuse, 5.0f);
        }
    }

    if(m_render_bend_springs)
    {
        lines.clear();
        for(const auto& pair : m_bend_spring.getSprings())
        {
            uvec3 coords     = m_mass_spring_volume->getLocalCoordinates(pair.mass_a_idx);
            vec3  position_a = m_mass_spring_volume->getPosition(coords.x, coords.y, coords.z);
            coords           = m_mass_spring_volume->getLocalCoordinates(pair.mass_b_idx);
            vec3 position_b  = m_mass_spring_volume->getPosition(coords.x, coords.y, coords.z);
            lines.push_back({position_a, 1.0f});
            lines.push_back({position_b, 1.0f});
        }
        if(!lines.empty())
        {
            m_render_system->drawDebugLines(lines, material_gold.diffuse, 5.0f);
        }
    }

    // update the mass aggregate volume
    m_mass_spring_volume->update(time_step.GetSeconds());

    // m_mass_spring_volume->update(time_step.GetSeconds());
    // m_mass_spring_volume->update(0.01f);
    // m_mass_spring_volume->update(0.001f);
}

void MassAggregateCubeDemoLayer::OnEvent() {}

void MassAggregateCubeDemoLayer::OnUpdateUI(const core::TimeStep& time_step)
{
    if(ImGui::Begin("Mass Spring Cloth Demo"))
    {
        // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
        //             1000.0f / ImGui::GetIO().Framerate,
        //             ImGui::GetIO().Framerate);

        sputnik::editor::Editor::drawWidgetCheckbox("Render Particles", m_render_particles, 90.0f, "#particles");
        sputnik::editor::Editor::drawWidgetCheckbox("Render Structural Springs",
                                                    m_render_structural_springs,
                                                    90.0f,
                                                    "#structural");
        sputnik::editor::Editor::drawWidgetCheckbox("Render Shear Springs", m_render_shear_springs, 90.0f, "#shear");
        sputnik::editor::Editor::drawWidgetCheckbox("Render Bend Springs", m_render_bend_springs, 90.0f, "#bend");
    }
    ImGui::End();
}

} // namespace sputnik::demos
