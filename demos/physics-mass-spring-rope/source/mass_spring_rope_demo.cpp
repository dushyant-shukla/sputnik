#include "mass_spring_rope_demo.h"

#include <graphics/api/color_material.h>
#include <core/systems/render_system.h>

#include <core/logging/logging_core.h>
#include <physics/physics_core.h>
#include <editor/editor.hpp>

#include <imgui.h>
#include <ImGuizmo.h>

#include <core/core.h>

namespace sputnik::demos
{

using namespace sputnik::editor;

MassSpringRopeDemoLayer::MassSpringRopeDemoLayer(const std::string& name)
    : core::Layer{name}
    , m_physics_system{nullptr}
    , m_render_system{nullptr}
{
}

MassSpringRopeDemoLayer::~MassSpringRopeDemoLayer() {}

void MassSpringRopeDemoLayer::OnAttach()
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

    const vec3 center_position = vec3(0.0f, 5.0f, 0.0f);
    const vec3 half_rope_size  = kRopeGridSize * 0.5f;
    const vec3 scale           = kRopeGridSize / kRopeGridResolution;

    const u32 total_particles = (u32)(kRopeGridResolution.x * kRopeGridResolution.y);
    m_physics_system->initParticleWorld(
        (total_particles * 10)); // 10 contacts per particle (should be part of the engine)
    m_particle_shapes.resize(total_particles);

    // kRopeGridResolution.y = 1.0f;

    for(size_t i = 0; i < kRopeGridResolution.x; ++i)
    {
        for(size_t j = 0; j < kRopeGridResolution.y; ++j)
        {
            for(size_t k = 0; k < kRopeGridResolution.z; ++k)
            {
                //vec3 position = center_position + vec3(real(i), real(j), real(k)) * scale - half_rope_size;
                vec3 position = center_position + vec3(real(i), real(j), real(k)) * kCableLength - half_rope_size;
                std::shared_ptr<Particle> particle = std::make_shared<Particle>();
                particle->setPosition(position);
                particle->setMass(kParticleMass);
                // particle->setAcceleration({0.0f, -1.0f, 0.0f});
                particle->setAcceleration(kGravity);
                particle->setDamping(kParticleDamping);
                m_particles.push_back(particle);
                m_physics_system->addParticle(particle);
            }
        }
    }

    std::for_each(m_particle_shapes.begin(),
                  m_particle_shapes.end(),
                  [this](auto& shape)
                  {
                      shape.center = {0.0f};
                      shape.radius = {kParticleRadius};
                  });

    for(size_t i = 0; i < m_particles.size(); i++)
    {
        if(i == 1) // second particle in the rope
        {
            m_particles[0]->setAcceleration({0.0f});
            std::shared_ptr<AnchoredParticleCable> cable = std::make_shared<AnchoredParticleCable>();
            cable->m_particle                            = m_particles[i];
            cable->m_anchor                              = m_particles[0]->getPosition(); // first particle is an anchor
            // cable->m_max_length                          = kParticleRadius * 12.5f;
            cable->m_max_length  = kCableLength;
            cable->m_restitution = kRestitution;
            m_anchored_cables.push_back(cable);
            m_physics_system->addContactGenerator(cable);
        }
        else if(i == kRopeGridResolution.x - 2) // penultimate particle in the rope
        {
            m_particles[i + 1]->setAcceleration({0.0f});
            std::shared_ptr<AnchoredParticleCable> cable = std::make_shared<AnchoredParticleCable>();
            cable->m_particle                            = m_particles[i];
            cable->m_anchor = m_particles[i + 1]->getPosition(); // last particle is an anchor
            // cable->m_max_length  = kParticleRadius * 12.5f;
            cable->m_max_length  = kCableLength;
            cable->m_restitution = kRestitution;
            m_anchored_cables.push_back(cable);
            m_physics_system->addContactGenerator(cable);
        }
        else if(i > 1 && i < kRopeGridResolution.x - 2) // all other particles in the rope
        {
            // auto spring_a = std::make_shared<ParticleSpringForceGenerator>(m_particles[i - 1],
            //                                                                kSpringStiffness,
            //                                                                kParticleRadius * 0.01f);
            // m_physics_system->registerParticleForceGenerator(m_particles[i], spring_a);

            // auto spring_b = std::make_shared<ParticleSpringForceGenerator>(m_particles[i + 1],
            //                                                                kSpringStiffness,
            //                                                                kParticleRadius * 0.01f);
            // m_physics_system->registerParticleForceGenerator(m_particles[i], spring_b);

            std::shared_ptr<ParticleCable> cable_a = std::make_shared<ParticleCable>();
            cable_a->m_particles[0]                = m_particles[i - 1];
            cable_a->m_particles[1]                = m_particles[i];
            // cable_a->m_max_length                  = kParticleRadius * 12.5f;
            cable_a->m_max_length  = kCableLength;
            cable_a->m_restitution = kRestitution;
            m_cables.push_back(cable_a);
            m_physics_system->addContactGenerator(cable_a);

            std::shared_ptr<ParticleCable> cable_b = std::make_shared<ParticleCable>();
            cable_b->m_particles[0]                = m_particles[i];
            cable_b->m_particles[1]                = m_particles[i + 1];
            // cable_b->m_max_length                  = kParticleRadius * 12.5f;
            cable_b->m_max_length  = kCableLength;
            cable_b->m_restitution = kRestitution;
            m_cables.push_back(cable_b);
            m_physics_system->addContactGenerator(cable_b);
        }
    }

    // for(size_t x_i = 0; x_i < kRopeGridResolution.y; ++x_i)
    //{
    //     for(size_t y_i = 0; y_i < kRopeGridResolution.x; ++y_i)
    //     // for(size_t j = 0; j < 1; ++j)
    //     {

    //        vec3 position = center_position + vec3(real(y_i), real(x_i), real(0.0f)) * scale - half_rope_size;
    //        std::shared_ptr<Particle> particle = std::make_shared<Particle>();
    //        particle->setPosition(position);
    //        particle->setMass(kParticleMass);
    //        // particle->setAcceleration({0.0f, -1.0f, 0.0f});
    //        particle->setDamping(kParticleDamping);
    //        m_particles.push_back(particle);

    //        // row major order: r * width + c
    //        ENGINE_INFO("Particle: {0}, position: {1}", x_i * (u32)kRopeGridResolution.x + y_i, position);
    //        // ENGINE_INFO("Particle: {0}, position: {1}", i * 1 + j, position);
    //        m_particle_shapes[x_i * (u32)kRopeGridResolution.x + y_i].center = position;

    //        m_physics_system->addParticle(particle);

    //        // for every j-th particle, add a spring to the (j+1)-th particle
    //        // for the last particle in every row, add a spring to the first particle in the current row
    //        // every particle in the current row is connected to the corresponding particle in the previous row
    //        if(y_i > 0)
    //        {
    //            std::shared_ptr<Particle> particle_a = m_particles[x_i * (u32)kRopeGridResolution.x + y_i];
    //            std::shared_ptr<Particle> particle_b = m_particles[x_i * (u32)kRopeGridResolution.x + y_i - 1];

    //            real const kSpringRestLength =
    //                real_abs((particle_a->getPosition() - particle_b->getPosition()).magnitude());

    //            std::shared_ptr<ParticleSpringForceGenerator> spring_force_generator =
    //                std::make_shared<ParticleSpringForceGenerator>(particle_b, kSpringStiffness, kSpringRestLength);
    //            m_physics_system->registerParticleForceGenerator(particle_a, spring_force_generator);

    //            // std::shared_ptr<ParticleCable> cable = std::make_shared<ParticleCable>();
    //            // cable->m_particles[0]                = m_particles[x_i * (u32)kRopeGridResolution.x + y_i];
    //            // cable->m_particles[1]                = m_particles[x_i * (u32)kRopeGridResolution.x + y_i - 1];
    //            // cable->m_max_length                  = 0.75f;
    //            // cable->m_restitution                 = 0.5f;
    //            ////m_cables.push_back(cable);
    //            // ENGINE_INFO("Cable: {0}, {1}", x_i * (u32)kRopeGridResolution.x + y_i, x_i *
    //            // (u32)kRopeGridResolution.x + y_i - 1); m_physics_system->addContactGenerator(cable);
    //        }
    //        if(y_i == kRopeGridResolution.x - 1)
    //        {
    //            std::shared_ptr<Particle> particle_a = m_particles[x_i * (u32)kRopeGridResolution.x + y_i];
    //            std::shared_ptr<Particle> particle_b = m_particles[x_i * (u32)kRopeGridResolution.x];

    //            // real const kSpringRestLength =
    //            //     real_abs((particle_a->getPosition() - particle_b->getPosition()).magnitude());

    //            real const kSpringRestLength = 0.0f;

    //            std::shared_ptr<ParticleSpringForceGenerator> spring_force_generator =
    //                std::make_shared<ParticleSpringForceGenerator>(particle_b, kSpringStiffness, kSpringRestLength);
    //            m_physics_system->registerParticleForceGenerator(particle_a, spring_force_generator);

    //            // std::shared_ptr<ParticleCable> cable = std::make_shared<ParticleCable>();
    //            // cable->m_particles[0]                = m_particles[x_i * (u32)kRopeGridResolution.x + y_i];
    //            // cable->m_particles[1]                = m_particles[x_i * (u32)kRopeGridResolution.x];
    //            // cable->m_max_length                  = 0.75f;
    //            // cable->m_restitution                 = 0.5f;
    //            //// m_cables.push_back(cable);
    //            // ENGINE_INFO("Cable: {0}, {1}",
    //            //             x_i * (u32)kRopeGridResolution.x + y_i,
    //            //             x_i * (u32)kRopeGridResolution.x);
    //            // m_physics_system->addContactGenerator(cable);
    //        }
    //        //{
    //        //    std::shared_ptr<Particle> particle_a = m_particles[i * (u32)kRopeGridResolution.x + j];
    //        //    std::shared_ptr<Particle> particle_b = m_particles[i * (u32)kRopeGridResolution.x];

    //        //    /*real const kSpringRestLength =
    //        //        real_abs((particle_a->getPosition() - particle_b->getPosition()).magnitude());*/
    //        //    real const kSpringRestLength = 0.0f;

    //        //    std::shared_ptr<ParticleSpringForceGenerator> spring_force_generator =
    //        //        std::make_shared<ParticleSpringForceGenerator>(particle_b, kSpringStiffness, kSpringRestLength);
    //        //    m_physics_system->registerParticleForceGenerator(particle_a, spring_force_generator);
    //        //}
    //    }
    //}

    // std::for_each(m_particle_shapes.begin(),
    //               m_particle_shapes.end(),
    //               [this](auto& shape)
    //               {
    //                   shape.center = {0.0f};
    //                   shape.radius = {kParticleRadius};
    //               });

    // for(size_t i = 0; i < m_particles.size(); i++)
    //{
    //     m_physics_system->addParticle(m_particles[i]);

    //    for(size_t j = 0; j < m_particles.size(); ++j)
    //    {
    //        if(i == j)
    //        {
    //            continue;
    //        }

    //        std::shared_ptr<Particle> particle_a = m_particles[i];
    //        std::shared_ptr<Particle> particle_b = m_particles[j];

    //        real const kSpringRestLength =
    //            real_abs((particle_a->getPosition() - particle_b->getPosition()).magnitude());

    //        std::shared_ptr<ParticleSpringForceGenerator> spring_force_generator =
    //            std::make_shared<ParticleSpringForceGenerator>(particle_b, kSpringStiffness, kSpringRestLength);
    //        m_physics_system->registerParticleForceGenerator(particle_a, spring_force_generator);
    //    }
    //}

    std::shared_ptr<GroundContactGenerator> ground_contact_generator = std::make_shared<GroundContactGenerator>();
    ground_contact_generator->init(m_physics_system->getParticles());
    m_physics_system->addContactGenerator(ground_contact_generator);
}

void MassSpringRopeDemoLayer::OnDetach() {}

void MassSpringRopeDemoLayer::OnUpdate(const core::TimeStep& time_step)
{

    if(time_step.GetSeconds() <= Constants::EPSILON)
    {
        return;
    }

    if(m_render_particles)
    {
        for(size_t i = 0; i < m_particles.size(); ++i)
        {
            // m_particle_positions[i]     = {m_particles[i]->getPosition(), 1.0f};
            m_particle_shapes[i].center = m_particles[i]->getPosition();
            // m_particle_shapes[i].radius = kParticleRadius;

            mat4 model{};
            model = model.translate(m_particles[i]->getPosition());
            model = model.scale(vec3(kParticleRadius));
            m_sphere->draw(material_blue_shine, model);
        }
    }
    else
    {
        std::vector<vec4> vertices;
        vertices.push_back(m_anchored_cables[0]->m_anchor);
        vertices.push_back(m_anchored_cables[0]->m_particle->getPosition());
        for(unsigned i = 0; i < m_cables.size(); ++i)
        {
            const auto& rod      = m_cables[i];
            vec3        position = rod->m_particles[0]->getPosition();
            vertices.emplace_back(position.x, position.y, position.z, 1.0f);
            position = rod->m_particles[1]->getPosition();
            vertices.emplace_back(position.x, position.y, position.z, 1.0f);
        }
        vertices.push_back(m_anchored_cables[1]->m_anchor);
        vertices.push_back(m_anchored_cables[1]->m_particle->getPosition());
        m_render_system->drawDebugLines(vertices, {0.0f, 0.0f, 1.0f}, 10.0f);
        vertices.clear();
    }
}

void MassSpringRopeDemoLayer::OnEvent() {}

void MassSpringRopeDemoLayer::OnUpdateUI(const core::TimeStep& time_step)
{
    if(ImGui::Begin("Mass Aggregate: Rope Demo"))
    {
        if(Editor::drawWidgetCheckbox("Draw particles", m_render_particles, 90.0f))
        {
        }
    }
    ImGui::End();
}

} // namespace sputnik::demos
