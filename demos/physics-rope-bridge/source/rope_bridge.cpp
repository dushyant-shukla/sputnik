#include "rope_bridge.h"

// #include <graphics/api/renderer.h>
// #include <graphics/glcore/uniform.h>
#include <graphics/api/color_material.h>
#include <core/systems/render_system.h>

#include <core/logging/logging_core.h>
#include <physics/physics_core.h>

#include <imgui.h>
#include <ImGuizmo.h>

#include <core/core.h>

namespace sputnik::demos
{

RopeBridgeDemoLayer::RopeBridgeDemoLayer(const std::string& name) : core::Layer{name}, m_mass_position{0.0f, 0.0f, 0.5f}
{
    m_physics_system = core::systems::PhysicsSystem::getInstance();
    m_physics_system->initParticleWorld(kParticleCount * 10);

    m_particles.resize(kParticleCount);

    for(unsigned i = 0; i < kParticleCount; ++i)
    {
        m_particles[i] = std::make_shared<Particle>();
        m_physics_system->addParticle(m_particles[i]);
    }

    m_falling_particle = std::make_shared<Particle>();
    m_falling_particle->setPosition({0.0f, 4.0f, 0.0f});
    m_falling_particle->setVelocity({0.0f, 0.0f, 0.0f});
    m_falling_particle->setMass(kBaseMass);
    m_falling_particle->setDamping(0.9f);
    m_falling_particle->setAcceleration(kGravity);

    m_physics_system->addParticle(m_falling_particle);

    m_ground_contact_generator = std::make_shared<GroundContactGenerator>();
    m_ground_contact_generator->init(m_physics_system->getParticles());

    m_physics_system->addContactGenerator(m_ground_contact_generator);
}

RopeBridgeDemoLayer::~RopeBridgeDemoLayer() {}

void RopeBridgeDemoLayer::OnAttach()
{

    m_sphere = Model::LoadModel("../../data/assets/sphere.gltf");

    auto  render_system = core::systems::RenderSystem::getInstance();
    auto& light         = render_system->getLight();
    light.position      = vec3(10.0f, 7.0f, 0.0f);
    light.ambient       = vec3(1.0f, 1.0f, 1.0f);
    light.diffuse       = vec3(1.0f, 1.0f, 1.0f);
    light.specular      = vec3(1.0f, 1.0f, 1.0f);

    for(unsigned i = 0; i < 12; ++i)
    {
        unsigned x = (i % 12) / 2;
        m_particles[i]->setPosition({real(i / 2) * 2.0f - 5.0f, 4.0f, real(i % 2) * 2.0f - 1.0f});
        m_particles[i]->setVelocity({0.0f, 0.0f, 0.0f});
        m_particles[i]->setMass(kBaseMass);
        m_particles[i]->setDamping(0.9f);
        m_particles[i]->setAcceleration(kGravity);
        m_particles[i]->clearAccumulator();
    }

    // add constrains
    {
        for(unsigned i = 0; i < kCableCount; ++i)
        {
            std::shared_ptr<ParticleCable> cable = std::make_shared<ParticleCable>();
            cable->m_particles[0]                = m_particles[i];
            cable->m_particles[1]                = m_particles[i + 2];
            cable->m_max_length                  = 1.9f;
            cable->m_restitution                 = 0.3f;
            m_cables.push_back(cable);
            m_physics_system->addContactGenerator(cable);
        }

        for(unsigned i = 0; i < kAnchoredCableCount; ++i)
        {
            std::shared_ptr<AnchoredParticleCable> cable = std::make_shared<AnchoredParticleCable>();
            cable->m_particle                            = m_particles[i];
            cable->m_anchor                              = {real(i / 2) * 2.2f - 5.5f, 6.0f, real(i % 2) * 1.6f - 0.8f};
            if(i < 6)
            {
                cable->m_max_length = real(i / 2) * 0.5f + 3.0f;
            }
            else
            {
                cable->m_max_length = 5.5f - real(i / 2) * 0.5f;
            }
            cable->m_restitution = 0.5f;
            m_anchored_cables.push_back(cable);
            m_physics_system->addContactGenerator(cable);
        }

        for(unsigned i = 0; i < kRodCount; ++i)
        {
            std::shared_ptr<ParticleRod> rod = std::make_shared<ParticleRod>();
            rod->m_particles[0]              = m_particles[i * 2];
            rod->m_particles[1]              = m_particles[i * 2 + 1];
            rod->m_length                    = 2.0f;
            m_rods.push_back(rod);
            m_physics_system->addContactGenerator(rod);
        }
    }

    UpdateAdditionalMass();
}

void RopeBridgeDemoLayer::OnDetach() {}

void RopeBridgeDemoLayer::OnUpdate(const core::TimeStep& time_step)
{

    if(time_step.GetSeconds() <= Constants::EPSILON)
    {
        return;
    }

    auto render_system = core::systems::RenderSystem::getInstance();

    float       speed         = 0.35f;
    const auto& input_manager = engine::api::InputManager::GetInstance();
    if(input_manager->IsKeyPressed(KEY_LEFT))
    {
        m_mass_position.z += 0.1f * speed;
        if(m_mass_position.z > 1.0f)
            m_mass_position.z = 1.0f;
    }
    if(input_manager->IsKeyPressed(KEY_RIGHT))
    {
        m_mass_position.z -= 0.1f * speed;
        if(m_mass_position.z < 0.0f)
            m_mass_position.z = 0.0f;
    }
    if(input_manager->IsKeyPressed(KEY_UP))
    {
        m_mass_position.x -= 0.1f * speed;
        if(m_mass_position.x < 0.0f)
            m_mass_position.x = 0.0f;
    }
    if(input_manager->IsKeyPressed(KEY_DOWN))
    {
        m_mass_position.x += 0.1f * speed;
        if(m_mass_position.x > 5.0f)
            m_mass_position.x = 5.0f;
    }

    // render light source
    {
        mat4 model;
        model = model.translate(render_system->getLight().position);
        model = model.scale({0.20f, 0.20f, 0.20f});
        m_sphere->draw(material_white, model);
    }

    std::vector<vec4> vertices;
    for(unsigned i = 0; i < kRodCount; ++i)
    {
        const auto& rod      = m_rods[i];
        vec3        position = rod->m_particles[0]->getPosition();
        vertices.emplace_back(position.x, position.y, position.z, 1.0f);
        position = rod->m_particles[1]->getPosition();
        vertices.emplace_back(position.x, position.y, position.z, 1.0f);
    }
    render_system->drawDebugLines(vertices, {0.0f, 0.0f, 1.0f}, 10.0f);
    vertices.clear();

    for(unsigned i = 0; i < kCableCount; ++i)
    {
        const auto& cable    = m_cables[i];
        vec3        position = cable->m_particles[0]->getPosition();
        vertices.emplace_back(position.x, position.y, position.z, 1.0f);
        position = cable->m_particles[1]->getPosition();
        vertices.emplace_back(position.x, position.y, position.z, 1.0f);
    }
    render_system->drawDebugLines(vertices, {0.0f, 1.0f, 0.0f}, 10.0f);
    vertices.clear();

    for(unsigned i = 0; i < kAnchoredCableCount; ++i)
    {
        const auto& cable    = m_anchored_cables[i];
        vec3        position = cable->m_particle->getPosition();
        vertices.emplace_back(position.x, position.y, position.z, 1.0f);
        vertices.emplace_back(cable->m_anchor.x, cable->m_anchor.y, cable->m_anchor.z, 1.0f);

        mat4 model{};
        model = model.translate({cable->m_anchor.x, cable->m_anchor.y, cable->m_anchor.z});
        model = model.scale({0.15f});
        m_sphere->draw(material_ruby, model);
    }
    render_system->drawDebugLines(vertices, {0.7f, 0.7f, 0.7f}, 7.5f);
    vertices.clear();

    // render particles
    for(unsigned i = 0; i < m_particles.size(); ++i)
    {
        const auto& particle = m_particles[i];
        mat4        model{};
        model = model.translate(particle->getPosition());
        model = model.scale({0.15f});
        m_sphere->draw(material_ruby, model);
    }

    // render moving mass
    {
        mat4 model{};
        model = model.translate(m_mass_display_position);
        model = model.scale({0.25f});
        m_sphere->draw(material_gold, model);
    }

    // render falling particle(s)
    {
        mat4 model = {};
        model      = model.translate(m_falling_particle->getPosition());
        model      = model.scale({0.25f});
        m_sphere->draw(material_pearl, model);
    }

    UpdateAdditionalMass();
}

void RopeBridgeDemoLayer::OnEvent() {}

void RopeBridgeDemoLayer::OnUpdateUI(const core::TimeStep& time_step)
{
    auto render_system = core::systems::RenderSystem::getInstance();
    mat4 projection    = render_system->getCameraProjection();
    mat4 view          = render_system->getCameraView();

    // if(ImGui::Begin("Particles"))
    {
        ImGuizmo::SetGizmoSizeClipSpace(0.075f);
        mat4 model = {};
        model      = model.translate(m_falling_particle->getPosition());
        ImGuizmo::Manipulate(&view.m[0],
                             &projection.m[0],
                             ImGuizmo::OPERATION::TRANSLATE,
                             ImGuizmo::MODE::LOCAL,
                             &model.m[0],
                             nullptr,
                             nullptr);
        if(ImGuizmo::IsUsing())
        {
            m_falling_particle->setPosition({model.m[12], model.m[13], model.m[14]});

            // We need to block camera update when we are using ImGuizmo
            render_system->blockCameraUpdate();
        }
        else
        {
            render_system->blockCameraUpdate(false);
        }
    }
    // ImGui::End();
}

void RopeBridgeDemoLayer::UpdateAdditionalMass()
{
    for(unsigned i = 0; i < kParticleCount; ++i)
    {
        m_particles[i]->setMass(kBaseMass);
    }

    // Find the coordinates of the mass as an index and proportion
    int  x  = int(m_mass_position.x);
    real xp = real_fmod(m_mass_position.x, real(1.0f));
    if(x < 0)
    {
        x  = 0;
        xp = 0;
    }
    if(x >= 5)
    {
        x  = 5;
        xp = 0;
    }

    int  z  = int(m_mass_position.z);
    real zp = real_fmod(m_mass_position.z, real(1.0f));
    if(z < 0)
    {
        z  = 0;
        zp = 0;
    }
    if(z >= 1)
    {
        z  = 1;
        zp = 0;
    }

    // Calculate where to draw the mass
    m_mass_display_position.clear();

    m_mass_display_position.y += 0.2f;

    // Add the proportion to the correct masses
    m_particles[x * 2 + z]->setMass(kBaseMass + kExtraMass * (1 - xp) * (1 - zp));
    m_mass_display_position += m_particles[x * 2 + z]->getPosition() * (1 - xp) * (1 - zp);

    if(xp > 0)
    {
        m_particles[x * 2 + z + 2]->setMass(kBaseMass + kExtraMass * xp * (1 - zp));
        m_mass_display_position += m_particles[x * 2 + z + 2]->getPosition() * xp * (1 - zp);

        if(zp > 0)
        {
            m_particles[x * 2 + z + 3]->setMass(kBaseMass + kExtraMass * xp * zp);
            m_mass_display_position += m_particles[x * 2 + z + 3]->getPosition() * xp * zp;
        }
    }
    if(zp > 0)
    {
        m_particles[x * 2 + z + 1]->setMass(kBaseMass + kExtraMass * (1 - xp) * zp);
        m_mass_display_position += m_particles[x * 2 + z + 1]->getPosition() * (1 - xp) * zp;
    }
}

} // namespace sputnik::demos
