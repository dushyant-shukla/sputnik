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

#include <algorithm>
#include <execution>
#include <ranges>

namespace sputnik::demos
{

MassAggregateRopeDemoLayer::MassAggregateRopeDemoLayer(const std::string& name) : core::Layer{name}
{
    m_cube_specification.mass = 1.0f;
    // m_cube_specification.scale      = {1.0f, 0.0f, 0.0f};
    // m_cube_specification.resolution = {2, 1, 1};
    m_cube_specification.scale           = {3.0f, 0.0f, 0.0f};
    m_cube_specification.resolution      = {60, 1, 1};
    m_cube_specification.center_position = {0.0f, 1.5f, 0.0f};
    m_cube_specification.damping         = 0.00005f;
    m_cube_specification.spring_flexion  = {.stiffness_coefficient = 2500.0f, .damping_coefficient = 35.0f};
    m_cube_specification.spring_torsion  = {.stiffness_coefficient = 2500.0f, .damping_coefficient = 35.0f};

    // structural springs are usually stiffer than shear,flexion springs
    m_cube_specification.spring_structural = {.stiffness_coefficient = 3500.0f, .damping_coefficient = 35.0f};

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

    std::vector<u32>       indices;
    std::ranges::iota_view vertices(0, 10);
    int                    strips = 5;
    for(int i = 0; i < vertices.size() - strips; i += strips)
    {
        for(int j = 0; j < strips; ++j)
        {
            indices.push_back(i);
            indices.push_back(i + strips);
            indices.push_back(i + 1);

            indices.push_back(i + strips);
            indices.push_back(i + 1);
            indices.push_back(i + strips + 1);
        }
    }
    ENGINE_INFO("indices size: {}", indices.size());
    for(int i = 0; i < indices.size(); ++i)
    {
        std::cout << indices[i] << ", ";
    }

    m_pvp_vertex_buffer = std::make_shared<OglBuffer>();
    m_rope_diff_texture = std::make_shared<OglTexture2D>("../../data/assets/textures/rope/rope_diff.png", true);
}

MassAggregateRopeDemoLayer::~MassAggregateRopeDemoLayer() {}

void MassAggregateRopeDemoLayer::OnAttach()
{

    m_render_system = core::systems::RenderSystem::getInstance();
    auto& light     = m_render_system->getLight();
    light.position  = vec3(5.0f, 5.0f, 5.0f);
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

    if(m_render_vertices || m_render_mesh)
    {
        int num_vertices = m_cube_specification.resolution.x - 1;
        // int num_vertices = 5;
        int strips = 10;
        // for(int i = 0; i < num_vertices; ++i)
        for(int i = 1; i < num_vertices; ++i)
        {
            vec3 position_a = m_mass_spring_curve->getPosition(i);
            vec3 position_c = m_mass_spring_curve->getPosition(i - 1);
            vec3 position_b = m_mass_spring_curve->getPosition(i + 1);
            vec3 tangent    = (position_b - position_c).normalized();
            vec3 up         = tangent;

            u8 axis;
            if(std::abs(tangent.x) > std::abs(tangent.y))
            {
                if(std::abs(tangent.x) > std::abs(tangent.z))
                {
                    axis = 0;
                }
                else
                {
                    axis = 2;
                }
            }
            else
            {
                if(std::abs(tangent.y) > std::abs(tangent.z))
                {
                    axis = 1;
                }
                else
                {
                    axis = 2;
                }
            }

            vec3 forward, right;
            switch(axis)
            {
            case 0:
            case 1:
            {
                forward = vec3(0.0f, 0.0f, 1.0f);
                right   = up.cross(forward).normalized();
                forward = right.cross(up).normalized();
                break;
            }
            case 2:
            {
                right   = vec3(0.0f, 0.0f, 1.0f);
                forward = right.cross(up).normalized();
                right   = up.cross(forward).normalized();
                break;
            }
            default:
                break;
            }

            for(int j = 0; j < strips; ++j)
            {
                // float t     = (float)j / (float)strips;
                // float theta = t * 2.0f * 3.14f;
                // int   s_normalized = i * strips + j / float(num_vertices * strips);
                // float rad          = 0.1f * (1.0f - s_normalized);
                // vec3  pc(std::cos(theta) * rad, 0, std::sin(theta) * rad);

                // float x = pc.x * right.x + pc.y * up.x + pc.z * forward.x;
                // float y = pc.x * right.y + pc.y * up.y + pc.z * forward.y;
                // float z = pc.x * right.z + pc.y * up.z + pc.z * forward.z;
                // vec3  pos(position_a.x + x, position_a.y + y, position_a.z + z);
                // m_positions.push_back(pos);

                // This also generates loop of points along the rope
                {
                    // float angle = j * 2.0f * 3.14f / strips;
                    // vec3  pos   = (position_a + 0.025f * right * std::cos(angle) + 0.025f * forward *
                    // std::sin(angle)); m_positions.push_back(pos);
                }
                // float      radius     = 0.025f;
                float      radius     = 0.040f;
                float      t          = (float)j / (float)strips;
                float      s          = i * strips + j / float(num_vertices * strips);
                float      angle      = t * 2.0f * 3.14f;
                vec3       pos_offset = radius * right * std::cos(angle) + radius * forward * std::sin(angle);
                vec3       pos        = (position_a + pos_offset);
                VertexData vertex{};
                vertex.position = pos;
                vertex.uv       = {s, t};
                // vertex.normal   = pos.normalized();
                vertex.normal = pos_offset.normalized();
                m_vertices.push_back(vertex);
            }
        }

        if(m_render_vertices)
        {
            std::vector<vec4> point_positions;
            for(int i = 0; i < num_vertices; ++i)
            {
                for(int j = 0; j < strips; ++j)
                {
                    int idx = i * strips + j;
                    point_positions.push_back({m_vertices[idx].position, 1.0f});
                }
            }
            m_render_system->drawDebugPoints(point_positions, material_blue_shine.diffuse, 5.0f);
            point_positions.clear();
        }
        else if(m_render_mesh)
        {
            // std::vector<vec4> tube_vertices;
            std::vector<VertexData> tube_vertices;
            // for(int i = 0; i < m_positions.size() - strips; i += strips)
            // for(int i = 0; i < m_positions.size() - strips - 1; ++i)
            for(int i = 0; i < m_vertices.size() - strips - 1; ++i)
            {
                // tube_vertices.emplace_back(m_vertices[i].position, 1.0f);
                // tube_vertices.emplace_back(m_vertices[i + strips].position, 1.0f);
                // tube_vertices.emplace_back(m_vertices[i + 1].position, 1.0f);

                // tube_vertices.emplace_back(m_vertices[i + strips].position, 1.0f);
                // tube_vertices.emplace_back(m_vertices[i + 1].position, 1.0f);
                // tube_vertices.emplace_back(m_vertices[i + strips + 1].position, 1.0f);

                tube_vertices.emplace_back(m_vertices[i]);
                tube_vertices.emplace_back(m_vertices[i + strips]);
                tube_vertices.emplace_back(m_vertices[i + 1]);

                tube_vertices.emplace_back(m_vertices[i + strips]);
                tube_vertices.emplace_back(m_vertices[i + 1]);
                tube_vertices.emplace_back(m_vertices[i + strips + 1]);
            }
            // m_render_system->drawDebugPoints(tube_vertices, material_blue_shine.diffuse, 5.0f);

            m_pvp_vertex_buffer->setData(tube_vertices.data(), tube_vertices.size() * sizeof(VertexData));
            m_pvp_vertex_buffer->bind(BufferBindTarget::ShaderStorageBuffer, 0);

            mat4     model{};
            Material material    = material_ruby;
            material.shader_name = "blinn_phong_pvp";
            // material.diff_texture = m_rope_diff_texture;

            m_render_system->drawTriangles(tube_vertices.size(), material, model);

            tube_vertices.clear();
        }

        m_positions.clear();
        m_vertices.clear();
    }

    // update the mass aggregate volume
    m_mass_spring_curve->update(time_step.GetSeconds());
}

void MassAggregateRopeDemoLayer::OnEvent() {}

void MassAggregateRopeDemoLayer::OnUpdateUI(const core::TimeStep& time_step)
{
    if(ImGui::Begin("Mass Spring Rope Demo"))
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
        sputnik::editor::Editor::drawWidgetCheckbox("Render Vertices", m_render_vertices, 90.0f, "#vertices");
        sputnik::editor::Editor::drawWidgetCheckbox("Render Mesh", m_render_mesh, 90.0f, "#mesh");
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
