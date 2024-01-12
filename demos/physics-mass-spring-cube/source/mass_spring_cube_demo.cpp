#include "mass_spring_cube_demo.h"

#include <graphics/api/color_material.h>
#include <core/systems/render_system.h>

#include <core/logging/logging_core.h>
#include <physics/physics_core.h>
#include <editor/editor.hpp>
#include <editor/editor_viewport.hpp>

#include <imgui.h>
#include <ImGuizmo.h>

namespace sputnik::demos
{

MassSpringCubeDemoLayer::MassSpringCubeDemoLayer(const std::string& name)
    : core::Layer{name}
    , m_physics_system{nullptr}
    , m_render_system{nullptr}
{
}

MassSpringCubeDemoLayer::~MassSpringCubeDemoLayer() {}

void MassSpringCubeDemoLayer::OnAttach()
{
    m_sphere = Model::LoadModel("../../data/assets/sphere.gltf");

    m_physics_system = core::systems::PhysicsSystem::getInstance();
    m_render_system  = core::systems::RenderSystem::getInstance();

    auto& light    = m_render_system->getLight();
    light.position = vec3(10.0f, 7.0f, 0.0f);
    light.ambient  = vec3(1.0f, 1.0f, 1.0f);
    light.diffuse  = vec3(1.0f, 1.0f, 1.0f);
    light.specular = vec3(1.0f, 1.0f, 1.0f);

    const vec3 center_position = vec3(0.0f, 2.5f, 0.0f);
    const vec3 half_cube_size  = kCubeGridSize * 0.5f;
    const vec3 scale           = kCubeGridSize / kCubeGridResolution;

    u32 surface_particles_count = (kCubeGridResolution.x * kCubeGridResolution.y * 2) +
                                  (kCubeGridResolution.x * kCubeGridResolution.z * 2) +
                                  (kCubeGridResolution.y * kCubeGridResolution.z * 2);

    u32 edge_particles_count = (kCubeGridResolution.x * 4) + (kCubeGridResolution.y * 4) + (kCubeGridResolution.z * 4);

    u32 corner_particles_count = 8;

    u32 total_particles_count = surface_particles_count + edge_particles_count + corner_particles_count;

    u32 calculated_surface_points_count = 0;

    for(size_t i = 0; i < kCubeGridResolution.x; ++i)
    {
        for(size_t j = 0; j < kCubeGridResolution.y; ++j)
        {
            for(size_t k = 0; k < kCubeGridResolution.z; ++k)
            {
                vec3 position = center_position + vec3(real(i), real(j), real(k)) * scale - half_cube_size;
                std::shared_ptr<Particle> particle = std::make_shared<Particle>();
                // if(position.x == 0 && position.z == 0)
                {
                    // ENGINE_INFO("Central particle at: {}", position);
                    m_central_particle = particle;
                    ENGINE_INFO("Central particle at i: {}, j: {}, k: {}, position: {}", i, j, k, position);
                }
                particle->setPosition(position);
                particle->setMass(kParticleMass);
                particle->setAcceleration({0.0f, -1.0f, 0.0f});
                particle->setDamping(kParticleDamping);
                m_particles.push_back(particle);

                if(i == 0 || i == (kCubeGridResolution.x - 1) || j == 0 || j == (kCubeGridResolution.x - 1) || k == 0 ||
                   k == (kCubeGridResolution.x - 1))
                {
                    ++calculated_surface_points_count;
                }
            }
        }
    }

    ENGINE_INFO("Total particles: {}, calculated surface particles: {}",
                total_particles_count,
                calculated_surface_points_count);
    ENGINE_INFO("Surface particles: {}, calculated surface particles: {}",
                surface_particles_count,
                calculated_surface_points_count);

    m_particle_positions.resize(m_particles.size());
    m_particle_shapes.resize(m_particles.size());
    std::for_each(m_particle_shapes.begin(),
                  m_particle_shapes.end(),
                  [this](auto& shape)
                  {
                      shape.center = {0.0f};
                      shape.radius = {kParticleRadius};
                  });

    m_physics_system->initParticleWorld((u32)(m_particles.size() * 10));
    for(size_t i = 0; i < m_particles.size(); i++)
    {
        m_physics_system->addParticle(m_particles[i]);

        for(size_t j = 0; j < m_particles.size(); ++j)
        {
            if(i == j)
            {
                continue;
            }

            std::shared_ptr<Particle> particle_a = m_particles[i];
            std::shared_ptr<Particle> particle_b = m_particles[j];

            real const kSpringRestLength =
                real_abs((particle_a->getPosition() - particle_b->getPosition()).magnitude());

            std::shared_ptr<ParticleSpringForceGenerator> spring_force_generator =
                std::make_shared<ParticleSpringForceGenerator>(particle_b, kSpringStiffness, kSpringRestLength);
            m_physics_system->registerParticleForceGenerator(particle_a, spring_force_generator);
        }
    }

    std::shared_ptr<GroundContactGenerator> ground_contact_generator = std::make_shared<GroundContactGenerator>();
    ground_contact_generator->init(m_physics_system->getParticles());
    m_physics_system->addContactGenerator(ground_contact_generator);

    m_sphere_geometry.center = vec3{5.0f, 5.0f, 0.0f};
    m_sphere_geometry.radius = 0.5f;
}

void MassSpringCubeDemoLayer::OnDetach() {}

void MassSpringCubeDemoLayer::OnUpdate(const core::TimeStep& time_step)
{
    if(time_step.GetSeconds() <= Constants::EPSILON)
    {
        return;
    }

    for(size_t i = 0; i < m_particles.size(); ++i)
    {
        m_particle_positions[i]     = {m_particles[i]->getPosition(), 1.0f};
        m_particle_shapes[i].center = m_particles[i]->getPosition();
        // m_particle_shapes[i].radius = kParticleRadius;

        mat4 model{};
        model = model.translate(m_particles[i]->getPosition());
        model = model.scale(vec3(kParticleRadius));
        m_sphere->draw(material_blue_shine, model);
    }

    {
        mat4 model{};
        model = model.translate({5.0f, 5.0f, 0.0f});
        model = model.scale(vec3(0.5f));
        m_sphere->draw(material_ruby, model);
    }

    // m_render_system->drawDebugPoints(m_particle_positions, material_blue_shine.diffuse, 10.0f);

    auto input = InputManager::GetInstance();
    if(input->IsMouseButtonTriggered(MOUSE_BUTTON_LEFT))
    {
        Ray ray1 = getPickRay(input->GetCursorNDCPosition(),
                              m_render_system->getCameraProjection(),
                              m_render_system->getCameraView());
        ENGINE_INFO("Calculated ray#1: origin: ({}, {}, {}) - direction ({}, {}, {})",
                    ray1.origin.x,
                    ray1.origin.y,
                    ray1.origin.z,
                    ray1.direction.x,
                    ray1.direction.y,
                    ray1.direction.z);

        Ray ray2 = getPickRay_1(input->GetCursorNDCPosition(),
                                m_render_system->getCameraProjection(),
                                m_render_system->getCameraView());
        ENGINE_INFO("Calculated ray#2: origin: ({}, {}, {}) - direction ({}, {}, {})",
                    ray2.origin.x,
                    ray2.origin.y,
                    ray2.origin.z,
                    ray2.direction.x,
                    ray2.direction.y,
                    ray2.direction.z);

        Ray  ray3;
        auto editor = editor::Editor::getInstance();
        if(editor->m_is_viewport_active)
        {
            auto [w, h]             = editor->m_viewport->m_viewport_size;
            auto [vx, vy]           = editor->m_viewport->m_viewport_bounds[0];
            vec2 adjusted_mouse_pos = {input->GetCursorPosition().x - vx, input->GetCursorPosition().y - vy};
            ray3                    = GetPickRay_2(adjusted_mouse_pos,
                                vec2(0.0f),
                                                   {w, h},
                                m_render_system->getCameraView(),
                                m_render_system->getCameraProjection());
            ENGINE_INFO("Calculated ray#3: origin: ({}, {}, {}) - direction ({}, {}, {})",
                        ray3.origin.x,
                        ray3.origin.y,
                        ray3.origin.z,
                        ray3.direction.x,
                        ray3.direction.y,
                        ray3.direction.z);
        }
        else
        {
            ray3 = GetPickRay_2({input->GetCursorPosition().x, input->GetCursorPosition().y},
                                vec2(0.0f),
                                {input->GetCursorMaxPosition().x, input->GetCursorMaxPosition().y},
                                m_render_system->getCameraView(),
                                m_render_system->getCameraProjection());
            ENGINE_INFO("Calculated ray#3: origin: ({}, {}, {}) - direction ({}, {}, {})",
                        ray3.origin.x,
                        ray3.origin.y,
                        ray3.origin.z,
                        ray3.direction.x,
                        ray3.direction.y,
                        ray3.direction.z);
        }

        // auto result = raycast(ray3, m_sphere_geometry);
        // if(result)
        //{
        //     ENGINE_INFO("Raycast result: hit: {}, t: {}, normal: ({}, {}, {}), point: ({}, {}, {})",
        //                 result->hit,
        //                 result->t,
        //                 result->normal.x,
        //                 result->normal.y,
        //                 result->normal.z,
        //                 result->point.x,
        //                 result->point.y,
        //                 result->point.z);
        // }
        // else
        //{
        //     ENGINE_INFO("Raycast result: no hit");
        // }

        for(u32 i = 0; i < m_particle_shapes.size(); ++i)
        {
            if(raycast(ray3, m_particle_shapes[i]))
            {
                hit_particle_idx = i;
            }
        }
        if(hit_particle_idx > -1)
        {
            gizmo_particle_idx = hit_particle_idx;
            ENGINE_INFO("Hit particle: {}", hit_particle_idx);
        }
        else
        {
            ENGINE_INFO("No hit particle");
        }
        hit_particle_idx = -1;
    }
}

void MassSpringCubeDemoLayer::OnEvent() {}

void MassSpringCubeDemoLayer::OnUpdateUI(const core::TimeStep& time_step)
{
    // if(m_central_particle)
    //{
    //     mat4 projection = m_render_system->getCameraProjection();
    //     mat4 view       = m_render_system->getCameraView();
    //     {
    //         ImGuizmo::SetGizmoSizeClipSpace(0.075f);
    //         mat4 model = {};
    //         model      = model.translate(m_central_particle->getPosition());
    //         ImGuizmo::Manipulate(&view.m[0],
    //                              &projection.m[0],
    //                              ImGuizmo::OPERATION::TRANSLATE,
    //                              ImGuizmo::MODE::LOCAL,
    //                              &model.m[0],
    //                              nullptr,
    //                              nullptr);
    //         if(ImGuizmo::IsUsing())
    //         {
    //             m_central_particle->setPosition({model.m[12], model.m[13], model.m[14]});

    //            // We need to block camera update when we are using ImGuizmo
    //            m_render_system->blockCameraUpdate();
    //        }
    //        else
    //        {
    //            m_render_system->blockCameraUpdate(false);
    //        }
    //    }
    //}
    if(gizmo_particle_idx > -1)
    {
        mat4 projection = m_render_system->getCameraProjection();
        mat4 view       = m_render_system->getCameraView();
        {
            ImGuizmo::SetGizmoSizeClipSpace(0.075f);
            mat4 model = {};
            model      = model.translate(m_particles[gizmo_particle_idx]->getPosition());
            ImGuizmo::Manipulate(&view.m[0],
                                 &projection.m[0],
                                 ImGuizmo::OPERATION::TRANSLATE,
                                 ImGuizmo::MODE::WORLD,
                                 &model.m[0],
                                 nullptr,
                                 nullptr);
            if(ImGuizmo::IsUsing())
            {
                m_particles[gizmo_particle_idx]->setPosition({model.m[12], model.m[13], model.m[14]});

                // We need to block camera update when we are using ImGuizmo
                m_render_system->blockCameraUpdate();
            }
            else
            {
                m_render_system->blockCameraUpdate(false);
            }
        }
    }
}

} // namespace sputnik::demos
