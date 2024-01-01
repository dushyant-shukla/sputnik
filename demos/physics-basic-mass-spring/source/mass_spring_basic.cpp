#include "mass_spring_basic.h"

// #include <graphics/api/renderer.h>
// #include <graphics/glcore/uniform.h>
#include <graphics/api/color_material.h>
#include <core/systems/render_system.h>

#include <core/logging/logging_core.h>

#include <imgui.h>
#include <ImGuizmo.h>

namespace sputnik::demos
{

MassSpringBasicDemoLayer::MassSpringBasicDemoLayer(const std::string& name)
    : core::Layer{name}
    , m_spring_force_generator_a{nullptr}
    , m_spring_force_generator_b{nullptr}
    , m_anchored_bungee_force_generator{nullptr}
{
    m_physics_system = core::systems::PhysicsSystem::getInstance();
    m_physics_system->initParticleWorld(kParticleCount * 10);
}

MassSpringBasicDemoLayer::~MassSpringBasicDemoLayer() {}

void MassSpringBasicDemoLayer::OnAttach()
{
    m_box    = Model::LoadModel("../../data/assets/box/Box.gltf");
    m_sphere = Model::LoadModel("../../data/assets/sphere.gltf");

    // anchor particle
    m_particles.push_back(std::make_shared<Particle>());
    m_particles.back()->setPosition({0.0f, 1.0f, 0.0f});
    m_particles.back()->setMass(10.0f); // 10 kg
    m_particles.back()->setDamping(0.5f);
    m_physics_system->addParticle(m_particles.back());
    m_particle_str += "Anchor";
    m_particle_str += '\0';

    // particle connected to the anchor with a spring
    m_particles.push_back(std::make_shared<Particle>());
    m_particles.back()->setPosition({0.0f, 1.0f, 0.0f});
    m_particles.back()->setMass(10.0f); // 10 kg
    m_particles.back()->setDamping(0.5f);
    m_particles.back()->setAcceleration({0.0f, -9.81f, 0.0f});
    m_physics_system->addParticle(m_particles.back());
    m_particle_str += "Anchored particle";
    m_particle_str += '\0';

    // particle a
    m_particles.push_back(std::make_shared<Particle>());
    m_particles.back()->setPosition({1.0f, 1.0f, 0.0f});
    m_particles.back()->setMass(10.0f); // 10 kg
    m_particles.back()->setDamping(0.5f);
    m_physics_system->addParticle(m_particles.back());
    m_particle_str += "Spring particle a";
    m_particle_str += '\0';

    // particle b
    m_particles.push_back(std::make_shared<Particle>());
    m_particles.back()->setPosition({2.0f, 1.0f, 0.0f});
    m_particles.back()->setMass(10.0f); // 10 kg
    m_particles.back()->setDamping(0.5f);
    m_physics_system->addParticle(m_particles.back());
    m_particle_str += "Spring particle b";
    m_particle_str += '\0';

    // particle c
    m_particles.push_back(std::make_shared<Particle>());
    m_particles.back()->setPosition({3.0f, 1.0f, 0.0f});
    m_particles.back()->setMass(10.0f); // 10 kg
    m_particles.back()->setDamping(0.5f);
    m_physics_system->addParticle(m_particles.back());
    m_particle_str += "Spring particle c";
    m_particle_str += '\0';

    // particle d
    m_particles.push_back(std::make_shared<Particle>());
    m_particles.back()->setPosition({3.0f, 0.0f, 0.0f});
    m_particles.back()->setMass(10.0f); // 10 kg
    m_particles.back()->setDamping(0.5f);
    m_particles.back()->setAcceleration({0.0f, -9.81f, 0.0f});
    m_physics_system->addParticle(m_particles.back());
    m_particle_str += "Spring particle d";
    m_particle_str += '\0';
    m_particle_str += '\0';

    m_spring_force_generator_a = std::make_shared<ParticleSpringForceGenerator>(m_particles[3], 25.0f, 5.0f);
    m_physics_system->registerParticleForceGenerator(m_particles[2], m_spring_force_generator_a);

    m_spring_force_generator_b = std::make_shared<ParticleSpringForceGenerator>(m_particles[2], 25.0f, 5.0f);
    m_physics_system->registerParticleForceGenerator(m_particles[3], m_spring_force_generator_b);

    m_anchored_spring_force_generator =
        std::make_shared<ParticleAnchoredSpringForceGenerator>(m_particles[4]->getPosition(), 50.0f, 2.0f);
    m_physics_system->registerParticleForceGenerator(m_particles[5], m_anchored_spring_force_generator);

    m_anchored_bungee_force_generator =
        std::make_shared<ParticleAnchoredBungeeForceGenerator>(m_particles[0]->getPosition(), 50.0f, 1.0f);
    m_physics_system->registerParticleForceGenerator(m_particles[1], m_anchored_bungee_force_generator);

    auto  render_system = core::systems::RenderSystem::getInstance();
    auto& light         = render_system->getLight();
    light.ambient       = vec3(1.0f, 1.0f, 1.0f);
    light.diffuse       = vec3(1.0f, 1.0f, 1.0f);
    light.specular      = vec3(1.0f, 1.0f, 1.0f);
}

void MassSpringBasicDemoLayer::OnDetach() {}

void MassSpringBasicDemoLayer::OnUpdate(const core::TimeStep& time_step)
{
    auto render_system = core::systems::RenderSystem::getInstance();

    // render light source
    {
        mat4 model_mat4;
        model_mat4 = model_mat4.translate(render_system->getLight().position);
        model_mat4 = model_mat4.scale({0.20f, 0.20f, 0.20f});
        m_sphere->draw(material_pearl, model_mat4);
    }

    m_anchored_bungee_force_generator->setAnchor(m_particles[0]->getPosition());
    m_anchored_spring_force_generator->setAnchor(m_particles[4]->getPosition());

    for(unsigned i = 0; i < m_particles.size(); ++i)
    {
        const auto& particle = m_particles[i];
        mat4        model{};
        model = model.translate(particle->getPosition());
        model = model.scale({0.15f});

        switch(i)
        {
        case 0:
        case 1:
        {
            m_sphere->draw(material_pearl, model);
            break;
        }
        case 2:
        case 3:
        {
            m_sphere->draw(material_ruby, model);
            break;
        }
        case 4:
        case 5:
        {
            m_sphere->draw(material_gold, model);
            break;
        }
        default:
            break;
        }
    }
}

void MassSpringBasicDemoLayer::OnEvent() {}

void MassSpringBasicDemoLayer::OnUpdateUI(const core::TimeStep& time_step)
{
    auto render_system = core::systems::RenderSystem::getInstance();
    mat4 projection    = render_system->getCameraProjection();
    mat4 view          = render_system->getCameraView();

    if(ImGui::Begin("Particles"))
    {
        ImGui::Combo("Particle", &m_particle_idx, m_particle_str.c_str());
        std::shared_ptr<Particle> particle = m_particles[m_particle_idx];
        ImGuizmo::SetGizmoSizeClipSpace(0.075f);
        mat4 model = {};
        model      = model.translate(particle->getPosition());
        ImGuizmo::Manipulate(&view.m[0],
                             &projection.m[0],
                             ImGuizmo::OPERATION::TRANSLATE,
                             ImGuizmo::MODE::LOCAL,
                             &model.m[0],
                             nullptr,
                             nullptr);
        if(ImGuizmo::IsUsing())
        {
            particle->setPosition({model.m[12], model.m[13], model.m[14]});

            // We need to block camera update when we are using ImGuizmo
            render_system->blockCameraUpdate();
        }
        else
        {
            render_system->blockCameraUpdate(false);
        }
    }
    ImGui::End();
}

} // namespace sputnik::demos
