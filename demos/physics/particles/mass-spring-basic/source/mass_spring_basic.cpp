#include "mass_spring_basic.h"

namespace sputnik::demos
{

MassSpringBasicDemoLayer::MassSpringBasicDemoLayer(const std::string& name) : core::Layer{name} {}

MassSpringBasicDemoLayer::~MassSpringBasicDemoLayer() {}

void MassSpringBasicDemoLayer::OnAttach()
{

    m_static_shader =
        std::make_shared<Shader>("../../../../data/shaders/simple.vert", "../../../../data/shaders/simple.frag");

    m_sky_shader = std::make_shared<Shader>("../../../../data/shaders/sky-rendering/sky.vert",
                                            "../../../../data/shaders/sky-rendering/sky.frag");
    m_simple_lighting_shader =
        std::make_shared<Shader>("../../../../data/shaders/simple-lighting/simple-lighting.vert",
                                 "../../../../data/shaders/simple-lighting/simple-lighting.frag");

    m_box    = Model::LoadModel("../../../../data/assets/box/Box.gltf");
    m_sphere = Model::LoadModel("../../../../data/assets/sphere.gltf");

    a = std::make_shared<Particle>();
    a->setPosition(0.0f);
    a->setMass(10.0f);

    anchor = std::make_shared<Particle>();
    anchor->setPosition({0.0f, -5.0f, 0.0f});
    anchor->setMass(10.0f);

    std::shared_ptr<ParticleAnchoredSpringForceGenerator> spring_force_generator =
        std::make_shared<ParticleAnchoredSpringForceGenerator>(anchor->getPosition(), 500.0f, 5.0f);
    m_particle_force_registry.add(a, spring_force_generator);
}

void MassSpringBasicDemoLayer::OnDetach() {}

void MassSpringBasicDemoLayer::OnUpdate(const core::TimeStep& time_step)
{
    // m_particle_force_registry.updateForces(time_step);

    // draw particles
    {
        m_simple_lighting_shader->Bind();

        mat4 model{};

        m_simple_lighting_shader->Unbind();
    }
}

void MassSpringBasicDemoLayer::OnEvent() {}

void MassSpringBasicDemoLayer::OnUpdateUI(const core::TimeStep& time_step) {}

} // namespace sputnik::demos
