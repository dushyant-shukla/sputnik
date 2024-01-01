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
    m_particles.back()->setPosition({0.0f, 5.0f, 0.0f});
    m_particles.back()->setMass(10.0f); // 10 kg
    m_particles.back()->setDamping(0.5f);
    m_physics_system->addParticle(m_particles.back());
    m_particle_str += "Anchor";
    m_particle_str += '\0';

    // particle connected to the anchor with a spring
    m_particles.push_back(std::make_shared<Particle>());
    m_particles.back()->setPosition({0.0f, 5.0f, 0.0f});
    m_particles.back()->setMass(10.0f); // 10 kg
    m_particles.back()->setDamping(0.5f);
    m_particles.back()->setAcceleration({0.0f, -9.81f, 0.0f});
    m_physics_system->addParticle(m_particles.back());
    m_particle_str += "Anchored particle";
    m_particle_str += '\0';

    // particle a
    m_particles.push_back(std::make_shared<Particle>());
    m_particles.back()->setPosition({2.0f, 5.0f, 0.0f});
    m_particles.back()->setMass(10.0f); // 10 kg
    m_particles.back()->setDamping(0.5f);
    m_physics_system->addParticle(m_particles.back());
    m_particle_str += "Spring particle a";
    m_particle_str += '\0';

    // particle b
    m_particles.push_back(std::make_shared<Particle>());
    m_particles.back()->setPosition({4.0f, 5.0f, 0.0f});
    m_particles.back()->setMass(10.0f); // 10 kg
    m_particles.back()->setDamping(0.5f);
    m_physics_system->addParticle(m_particles.back());
    m_particle_str += "Spring particle b";
    m_particle_str += '\0';

    // particle c
    m_particles.push_back(std::make_shared<Particle>());
    m_particles.back()->setPosition({6.0f, 5.0f, 0.0f});
    m_particles.back()->setMass(10.0f); // 10 kg
    m_particles.back()->setDamping(0.5f);
    m_physics_system->addParticle(m_particles.back());
    m_particle_str += "Spring particle c";
    m_particle_str += '\0';

    // particle d
    m_particles.push_back(std::make_shared<Particle>());
    m_particles.back()->setPosition({6.0f, 0.0f, 0.0f});
    m_particles.back()->setMass(10.0f); // 10 kg
    m_particles.back()->setDamping(0.5f);
    m_particles.back()->setAcceleration({0.0f, -9.81f, 0.0f});
    m_physics_system->addParticle(m_particles.back());
    m_particle_str += "Spring particle d";
    m_particle_str += '\0';
    m_particle_str += '\0';

    m_spring_force_generator_a = std::make_shared<ParticleSpringForceGenerator>(m_particles[3], 100.0f, 2.0f);
    m_physics_system->registerParticleForceGenerator(m_particles[2], m_spring_force_generator_a);

    m_spring_force_generator_b = std::make_shared<ParticleSpringForceGenerator>(m_particles[2], 100.0f, 2.0f);
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

    ///////////////////// setting up the cube /////////////////////

    vec3 center{0.0f, 10.0f, 0.0f};
    vec3 half_extent{0.75f, 0.75f, 0.75f};
    vec3 front_top_left{center.x - half_extent.x, center.y + half_extent.y, center.z + half_extent.z};
    vec3 front_top_right{center.x + half_extent.x, center.y + half_extent.y, center.z + half_extent.z};
    vec3 front_bottom_left{center.x - half_extent.x, center.y - half_extent.y, center.z + half_extent.z};
    vec3 front_bottom_right{center.x + half_extent.x, center.y - half_extent.y, center.z + half_extent.z};
    vec3 back_top_left{center.x - half_extent.x, center.y + half_extent.y, center.z - half_extent.z};
    vec3 back_top_right{center.x + half_extent.x, center.y + half_extent.y, center.z - half_extent.z};
    vec3 back_bottom_left{center.x - half_extent.x, center.y - half_extent.y, center.z - half_extent.z};
    vec3 back_bottom_right{center.x + half_extent.x, center.y - half_extent.y, center.z - half_extent.z};

    std::vector<vec3> front_positions{front_top_left, front_top_right, front_bottom_left, front_bottom_right};

    std::vector<vec3> back_positions{back_top_left, back_top_right, back_bottom_left, back_bottom_right};

    for(size_t i = 0; i < front_positions.size(); ++i)
    {
        m_front_cube_particles.push_back(std::make_shared<Particle>());
        m_front_cube_particles.back()->setPosition(front_positions[i]);
        m_front_cube_particles.back()->setMass(2.0f); // 10 kg
        m_front_cube_particles.back()->setDamping(0.05f);
        // m_front_cube_particles.back()->setAcceleration({0.0f, -9.81f, 0.0f});
        m_front_cube_particles.back()->setAcceleration({0.0f, -5.0f, 0.0f});
        m_physics_system->addParticle(m_front_cube_particles.back());

        m_front_particle_str += "Front: " + std::to_string(i);
        m_front_particle_str += '\0';
    }
    m_front_particle_str += '\0';

    for(size_t i = 0; i < back_positions.size(); ++i)
    {
        m_back_cube_particles.push_back(std::make_shared<Particle>());
        m_back_cube_particles.back()->setPosition(back_positions[i]);
        m_back_cube_particles.back()->setMass(2.0f); // 10 kg
        m_back_cube_particles.back()->setDamping(0.05f);
        // m_back_cube_particles.back()->setAcceleration({0.0f, -9.81f, 0.0f});
        m_back_cube_particles.back()->setAcceleration({0.0f, -5.0f, 0.0f});
        m_physics_system->addParticle(m_back_cube_particles.back());

        m_back_particle_str += "Back: " + std::to_string(i);
        m_back_particle_str += '\0';
    }
    m_back_particle_str += '\0';

    m_central_particle = std::make_shared<Particle>();
    m_central_particle->setPosition(center);
    m_central_particle->setMass(2.0f); // 10 kg
    m_central_particle->setDamping(0.05f);
    m_central_particle->setAcceleration({0.0f, -5.0f, 0.0f});
    m_physics_system->addParticle(m_central_particle);

    m_connection_pairs.push_back(m_front_cube_particles[TOP_LEFT_IDX]);
    m_connection_pairs.push_back(m_front_cube_particles[TOP_RIGHT_IDX]);

    m_connection_pairs.push_back(m_front_cube_particles[BOTTOM_LEFT_IDX]);
    m_connection_pairs.push_back(m_front_cube_particles[BOTTOM_RIGHT_IDX]);

    m_connection_pairs.push_back(m_front_cube_particles[TOP_LEFT_IDX]);
    m_connection_pairs.push_back(m_front_cube_particles[BOTTOM_LEFT_IDX]);

    m_connection_pairs.push_back(m_front_cube_particles[TOP_RIGHT_IDX]);
    m_connection_pairs.push_back(m_front_cube_particles[BOTTOM_RIGHT_IDX]);

    m_connection_pairs.push_back(m_back_cube_particles[TOP_LEFT_IDX]);
    m_connection_pairs.push_back(m_back_cube_particles[TOP_RIGHT_IDX]);

    m_connection_pairs.push_back(m_back_cube_particles[BOTTOM_LEFT_IDX]);
    m_connection_pairs.push_back(m_back_cube_particles[BOTTOM_RIGHT_IDX]);

    m_connection_pairs.push_back(m_back_cube_particles[TOP_LEFT_IDX]);
    m_connection_pairs.push_back(m_back_cube_particles[BOTTOM_LEFT_IDX]);

    m_connection_pairs.push_back(m_back_cube_particles[TOP_RIGHT_IDX]);
    m_connection_pairs.push_back(m_back_cube_particles[BOTTOM_RIGHT_IDX]);

    m_connection_pairs.push_back(m_front_cube_particles[TOP_LEFT_IDX]);
    m_connection_pairs.push_back(m_back_cube_particles[TOP_LEFT_IDX]);

    m_connection_pairs.push_back(m_front_cube_particles[TOP_RIGHT_IDX]);
    m_connection_pairs.push_back(m_back_cube_particles[TOP_RIGHT_IDX]);

    m_connection_pairs.push_back(m_front_cube_particles[BOTTOM_LEFT_IDX]);
    m_connection_pairs.push_back(m_back_cube_particles[BOTTOM_LEFT_IDX]);

    m_connection_pairs.push_back(m_front_cube_particles[BOTTOM_RIGHT_IDX]);
    m_connection_pairs.push_back(m_back_cube_particles[BOTTOM_RIGHT_IDX]); // #23 (0-indexed)

    m_connection_pairs.push_back(m_central_particle);
    m_connection_pairs.push_back(m_front_cube_particles[TOP_LEFT_IDX]);

    m_connection_pairs.push_back(m_central_particle);
    m_connection_pairs.push_back(m_front_cube_particles[TOP_RIGHT_IDX]);

    m_connection_pairs.push_back(m_central_particle);
    m_connection_pairs.push_back(m_front_cube_particles[BOTTOM_LEFT_IDX]);

    m_connection_pairs.push_back(m_central_particle);
    m_connection_pairs.push_back(m_front_cube_particles[BOTTOM_RIGHT_IDX]);

    m_connection_pairs.push_back(m_central_particle);
    m_connection_pairs.push_back(m_back_cube_particles[TOP_LEFT_IDX]);

    m_connection_pairs.push_back(m_central_particle);
    m_connection_pairs.push_back(m_back_cube_particles[TOP_RIGHT_IDX]);

    m_connection_pairs.push_back(m_central_particle);
    m_connection_pairs.push_back(m_back_cube_particles[BOTTOM_LEFT_IDX]);

    m_connection_pairs.push_back(m_central_particle);
    m_connection_pairs.push_back(m_back_cube_particles[BOTTOM_RIGHT_IDX]);

    u32 constraint_count = 0;
    for(size_t i = 0; i < m_connection_pairs.size() - 1; i += 2)
    {

        if(i <= 23)
        {
            // 0-1
            // 2-3
            // 4-5
            // 6-7
            // 8-9
            // 10-11
            // 12-13
            // 14-15
            // 16-17
            // 18-19
            // 20-21
            // 22-23
            auto m_spring_force_generator_1 =
                std::make_shared<ParticleSpringForceGenerator>(m_connection_pairs[i], 50.0f, 1.5f);
            m_physics_system->registerParticleForceGenerator(m_connection_pairs[i + 1], m_spring_force_generator_1);

            auto m_spring_force_generator_2 =
                std::make_shared<ParticleSpringForceGenerator>(m_connection_pairs[i + 1], 50.0f, 1.5f);
            m_physics_system->registerParticleForceGenerator(m_connection_pairs[i], m_spring_force_generator_2);

            constraint_count += 2;
        }
        else
        {
            // 24-25
            // 26-27
            // 28-29
            // 30-31
            // 32-33
            // 34-35
            // 36-37
            // 38-39
            auto m_spring_force_generator_3 =
                std::make_shared<ParticleSpringForceGenerator>(m_connection_pairs[i], 50.0f, 1.25f);
            m_physics_system->registerParticleForceGenerator(m_connection_pairs[i + 1], m_spring_force_generator_3);

            auto m_spring_force_generator_4 =
                std::make_shared<ParticleSpringForceGenerator>(m_connection_pairs[i + 1], 50.0f, 1.25f);
            m_physics_system->registerParticleForceGenerator(m_connection_pairs[i], m_spring_force_generator_4);

            // std::shared_ptr<ParticleRod> rod = std::make_shared<ParticleRod>();
            // rod->m_particles[0]              = m_connection_pairs[i];
            // rod->m_particles[1]              = m_connection_pairs[i + 1];
            // rod->m_length                    = 1.25f;
            // m_physics_system->addContactGenerator(rod);

            constraint_count += 2;
        }
    }

    ENGINE_INFO("Connection pairs: {}", m_connection_pairs.size() / 2);
    ENGINE_INFO("Constraint count: {}", constraint_count);

    std::shared_ptr<GroundContactGenerator> ground_contact_generator = std::make_shared<GroundContactGenerator>();
    ground_contact_generator->init(m_physics_system->getParticles());
    m_physics_system->addContactGenerator(ground_contact_generator);
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
            m_sphere->draw(material_ruby, model);
            break;
        }
    }

    ///////////////////// drawing the cube /////////////////////

    for(size_t i = 0; i < m_front_cube_particles.size(); i++)
    {
        {
            auto& particle = m_front_cube_particles[i];
            mat4  model{};
            model = model.translate(particle->getPosition());
            model = model.scale({0.15f});
            m_sphere->draw(material_blue_shine, model);
        }

        {
            auto& particle = m_back_cube_particles[i];
            mat4  model{};
            model = model.translate(particle->getPosition());
            model = model.scale({0.15f});
            m_sphere->draw(material_blue_shine, model);
        }
    }

    // render central particle
    {
        mat4 model{};
        model = model.translate(m_central_particle->getPosition());
        model = model.scale({0.15f});
        m_sphere->draw(material_blue_shine, model);
    }

    std::vector<vec4> connection_vertices;
    for(size_t i = 0; i < m_connection_pairs.size(); i++)
    {
        connection_vertices.emplace_back(m_connection_pairs[i]->getPosition(), 1.0);
    }

    render_system->drawDebugLines(connection_vertices, material_emerald.diffuse, 7.5f);

    connection_vertices.clear();
}

void MassSpringBasicDemoLayer::OnEvent() {}

void MassSpringBasicDemoLayer::OnUpdateUI(const core::TimeStep& time_step)
{
    auto render_system = core::systems::RenderSystem::getInstance();
    mat4 projection    = render_system->getCameraProjection();
    mat4 view          = render_system->getCameraView();

    if(ImGui::Begin("Particles"))
    {
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

        //{
        //    ImGui::Combo("Front particle", &m_front_particle_idx, m_front_particle_str.c_str());
        //    std::shared_ptr<Particle> particle = m_front_cube_particles[m_front_particle_idx];
        //    ImGuizmo::SetGizmoSizeClipSpace(0.075f);
        //    mat4 model = {};
        //    model      = model.translate(particle->getPosition());
        //    ImGuizmo::Manipulate(&view.m[0],
        //                         &projection.m[0],
        //                         ImGuizmo::OPERATION::TRANSLATE,
        //                         ImGuizmo::MODE::LOCAL,
        //                         &model.m[0],
        //                         nullptr,
        //                         nullptr);
        //    if(ImGuizmo::IsUsing())
        //    {
        //        particle->setPosition({model.m[12], model.m[13], model.m[14]});

        //        // We need to block camera update when we are using ImGuizmo
        //        render_system->blockCameraUpdate();
        //    }
        //    else
        //    {
        //        render_system->blockCameraUpdate(false);
        //    }
        //}

        //{
        //    ImGui::Combo("Back particle", &m_back_particle_idx, m_back_particle_str.c_str());

        //    std::shared_ptr<Particle> particle = m_back_cube_particles[m_back_particle_idx];
        //    ImGuizmo::SetGizmoSizeClipSpace(0.075f);
        //    mat4 model = {};
        //    model      = model.translate(particle->getPosition());
        //    ImGuizmo::Manipulate(&view.m[0],
        //                         &projection.m[0],
        //                         ImGuizmo::OPERATION::TRANSLATE,
        //                         ImGuizmo::MODE::LOCAL,
        //                         &model.m[0],
        //                         nullptr,
        //                         nullptr);
        //    if(ImGuizmo::IsUsing())
        //    {
        //        particle->setPosition({model.m[12], model.m[13], model.m[14]});

        //        // We need to block camera update when we are using ImGuizmo
        //        render_system->blockCameraUpdate();
        //    }
        //    else
        //    {
        //        render_system->blockCameraUpdate(false);
        //    }
        //}

        //{
        //    std::shared_ptr<Particle> particle = m_central_particle;
        //    ImGuizmo::SetGizmoSizeClipSpace(0.075f);
        //    mat4 model = {};
        //    model      = model.translate(particle->getPosition());
        //    ImGuizmo::Manipulate(&view.m[0],
        //                         &projection.m[0],
        //                         ImGuizmo::OPERATION::TRANSLATE,
        //                         ImGuizmo::MODE::LOCAL,
        //                         &model.m[0],
        //                         nullptr,
        //                         nullptr);
        //    if(ImGuizmo::IsUsing())
        //    {
        //        particle->setPosition({model.m[12], model.m[13], model.m[14]});

        //        // We need to block camera update when we are using ImGuizmo
        //        render_system->blockCameraUpdate();
        //    }
        //    else
        //    {
        //        render_system->blockCameraUpdate(false);
        //    }
        //}
    }
    ImGui::End();
}

} // namespace sputnik::demos
