#include "mass_aggregate_rope.h"

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

MassAggregateRopeDemoLayer::MassAggregateRopeDemoLayer(const std::string& name) : core::Layer{name}
{
    m_cube_specification.mass = 1.0f;
    // m_cube_specification.scale      = {1.0f, 0.0f, 0.0f};
    // m_cube_specification.resolution = {2, 1, 1};
    m_cube_specification.scale           = {3.0f, 0.0f, 0.0f};
    m_cube_specification.resolution      = {45, 1, 1};
    m_cube_specification.center_position = {0.0f, 1.5f, 0.0f};
    m_cube_specification.damping         = 0.0005f;
    m_cube_specification.spring_flexion  = {.stiffness_coefficient = 1500.0f, .damping_coefficient = 0.5f};

    // structural springs are usually stiffer than shear,flexion springs
    m_cube_specification.spring_structural = {.stiffness_coefficient = 2500.0f, .damping_coefficient = 0.5f};

    m_mass_spring_curve = std::make_shared<::physics::mad::MassAggregateCurve>(m_cube_specification);
    m_structural_spring = m_mass_spring_curve->getStructuralSprings();
    m_bend_spring       = m_mass_spring_curve->getFlexionSprings();

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

MassAggregateRopeDemoLayer::~MassAggregateRopeDemoLayer() {}

void MassAggregateRopeDemoLayer::OnAttach()
{

    m_render_system = core::systems::RenderSystem::getInstance();
    auto& light     = m_render_system->getLight();
    light.position  = vec3(10.0f, 7.0f, 0.0f);
    light.ambient   = vec3(1.0f, 1.0f, 1.0f);
    light.diffuse   = vec3(1.0f, 1.0f, 1.0f);
    light.specular  = vec3(1.0f, 1.0f, 1.0f);
}

void MassAggregateRopeDemoLayer::OnDetach() {}

void MassAggregateRopeDemoLayer::OnUpdate(const core::TimeStep& time_step)
{

    if(time_step.GetSeconds() <= Constants::EPSILON)
    {
        return;
    }

    if(m_render_particles)
    {
        std::vector<mat4> matrices;

        for(unsigned x = 0; x < m_cube_specification.resolution.x; ++x)
        {
            vec3 position = m_mass_spring_curve->getPosition(x);
            mat4 model{};
            model = model.translate(position);
            model = model.scale(vec3(0.0004f));

            // (Transpose of inverse of the model matrix)
            mat4 normal_matrix = model;
            normal_matrix      = normal_matrix.inverted().transpose();

            matrices.push_back(model);
            matrices.push_back(normal_matrix);
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
            vec3 position_a = m_mass_spring_curve->getPosition(spring.mass_a_idx);
            vec3 position_b = m_mass_spring_curve->getPosition(spring.mass_b_idx);
            lines.push_back({position_a, 1.0f});
            lines.push_back({position_b, 1.0f});
        }
        if(!lines.empty())
        {
            m_render_system->drawDebugLines(lines, {1.0f, 0.0f, 0.0f}, 5.0f);
        }
    }

    if(m_render_bend_springs)
    {
        lines.clear();
        for(const auto& spring : m_bend_spring.getSprings())
        {
            vec3 position_a = m_mass_spring_curve->getPosition(spring.mass_a_idx);
            vec3 position_b = m_mass_spring_curve->getPosition(spring.mass_b_idx);
            lines.push_back({position_a, 1.0f});
            lines.push_back({position_b, 1.0f});
        }
        if(!lines.empty())
        {
            m_render_system->drawDebugLines(lines, material_gold.diffuse, 5.0f);
        }
    }

    // update the mass aggregate volume
    m_mass_spring_curve->update(time_step.GetSeconds());
}

void MassAggregateRopeDemoLayer::OnEvent() {}

void MassAggregateRopeDemoLayer::OnUpdateUI(const core::TimeStep& time_step)
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
        sputnik::editor::Editor::drawWidgetCheckbox("Render Bend Springs", m_render_bend_springs, 90.0f, "#bend");
    }
    ImGui::End();

    mat4 projection = m_render_system->getCameraProjection();
    mat4 view       = m_render_system->getCameraView();
    ImGuizmo::SetGizmoSizeClipSpace(0.075f);
    mat4 model = {};
    model      = model.translate(m_mass_spring_curve->getPosition(m_cube_specification.resolution.x - 1));
    ImGuizmo::Manipulate(&view.m[0],
                         &projection.m[0],
                         ImGuizmo::OPERATION::TRANSLATE,
                         ImGuizmo::MODE::LOCAL,
                         &model.m[0],
                         nullptr,
                         nullptr);
    if(ImGuizmo::IsUsing())
    {
        m_mass_spring_curve->setPosition(m_cube_specification.resolution.x - 1,
                                         {model.m[12], model.m[13], model.m[14]});

        // We need to block camera update when we are using ImGuizmo
        m_render_system->blockCameraUpdate();
    }
    else
    {
        m_render_system->blockCameraUpdate(false);
    }
}

} // namespace sputnik::demos
