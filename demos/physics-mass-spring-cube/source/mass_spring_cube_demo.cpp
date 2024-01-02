#include "mass_spring_cube_demo.h"

#include <graphics/api/color_material.h>
#include <core/systems/render_system.h>

#include <core/logging/logging_core.h>
#include <physics/physics_core.h>

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

    auto  render_system = core::systems::RenderSystem::getInstance();
    auto& light         = render_system->getLight();
    light.position      = vec3(10.0f, 7.0f, 0.0f);
    light.ambient       = vec3(1.0f, 1.0f, 1.0f);
    light.diffuse       = vec3(1.0f, 1.0f, 1.0f);
    light.specular      = vec3(1.0f, 1.0f, 1.0f);

    m_physics_system = core::systems::PhysicsSystem::getInstance();
    m_render_system  = core::systems::RenderSystem::getInstance();

    const vec3 center_position = vec3(0.0f, 2.5f, 0.0f);
    const vec3 half_cube_size  = kCubeGridSize * 0.5f;
    const vec3 scale           = kCubeGridSize / kCubeGridResolution;

    for(size_t i = 0; i < kCubeGridResolution.x; ++i)
    {
        for(size_t j = 0; j < kCubeGridResolution.x; ++j)
        {
            for(size_t k = 0; k < kCubeGridResolution.x; ++k)
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
            }
        }
    }

    m_particle_positions.resize(m_particles.size());

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
        m_particle_positions[i] = {m_particles[i]->getPosition(), 1.0f};

         mat4 model{};
         model = model.translate(m_particles[i]->getPosition());
         model = model.scale(vec3(kParticleRadius));
         m_sphere->draw(material_blue_shine, model);
    }

    //m_render_system->drawDebugPoints(m_particle_positions, material_blue_shine.diffuse, 10.0f);
}

void MassSpringCubeDemoLayer::OnEvent() {}

void MassSpringCubeDemoLayer::OnUpdateUI(const core::TimeStep& time_step)
{
    if(m_central_particle)
    {
        mat4 projection = m_render_system->getCameraProjection();
        mat4 view       = m_render_system->getCameraView();
        {
            ImGuizmo::SetGizmoSizeClipSpace(0.075f);
            mat4 model = {};
            model      = model.translate(m_central_particle->getPosition());
            ImGuizmo::Manipulate(&view.m[0],
                                 &projection.m[0],
                                 ImGuizmo::OPERATION::TRANSLATE,
                                 ImGuizmo::MODE::LOCAL,
                                 &model.m[0],
                                 nullptr,
                                 nullptr);
            if(ImGuizmo::IsUsing())
            {
                m_central_particle->setPosition({model.m[12], model.m[13], model.m[14]});

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
