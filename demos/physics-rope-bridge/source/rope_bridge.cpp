#include "rope_bridge.h"

#include <graphics/api/renderer.h>
#include <graphics/glcore/uniform.h>
#include <graphics/api/color_material.h>

#include <core/logging/logging_core.h>
#include <physics/physics_core.h>

#include <imgui.h>
#include <ImGuizmo.h>

namespace sputnik::demos
{

RopeBridgeDemoLayer::RopeBridgeDemoLayer(const std::string& name, unsigned particle_count)
    : core::Layer{name}
    , m_particle_world(particle_count * 10) // 10 contacts per particle
    , m_anchored_cables{nullptr}
    , m_cables{nullptr}
    , m_rods{nullptr}
    , m_mass_position{0.0f, 0.0f, 0.5f}
{
    m_particles.reserve(particle_count);

    for(unsigned i = 0; i < particle_count; ++i)
    {
        Particle* particle = new Particle();
        // m_particles[i]     = particle;
        m_particles.push_back(particle);
        m_particle_world.getParticles().push_back(particle);
    }

    m_ground_contact_generator.init(m_particle_world.getParticles());
    // m_particle_world.getContactGenerators().push_back(&m_ground_contact_generator);
}

RopeBridgeDemoLayer::~RopeBridgeDemoLayer() {}

void RopeBridgeDemoLayer::OnAttach()
{
    Renderer::SetCameraType(CameraType::Camera);

    m_static_shader = std::make_shared<Shader>("../../data/shaders/simple.vert", "../../data/shaders/simple.frag");

    m_simple_lighting_shader = std::make_shared<Shader>("../../data/shaders/simple-lighting/simple-lighting.vert",
                                                        "../../data/shaders/simple-lighting/simple-lighting.frag");

    m_static_mesh_texture = std::make_shared<Texture>("../../data/assets/dq.png");

    m_box    = Model::LoadModel("../../data/assets/box/Box.gltf");
    m_sphere = Model::LoadModel("../../data/assets/sphere.gltf");

    m_debug_lines = std::make_shared<sputnik::graphics::glcore::DebugDraw>();

    m_light.ambient  = {1.0f, 1.0f, 1.0f};
    m_light.diffuse  = {1.0f, 1.0f, 1.0f};
    m_light.specular = {1.0f, 1.0f, 1.0f};

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
    m_cables = new ParticleCable[kCableCount];
    for(unsigned i = 0; i < kCableCount; ++i)
    {
        m_cables[i].m_particles[0] = m_particles[i];
        m_cables[i].m_particles[1] = m_particles[i + 2];
        m_cables[i].m_max_length   = 1.9f;
        m_cables[i].m_restitution  = 0.3f;
        m_particle_world.getContactGenerators().push_back(&m_cables[i]);
    }

    m_anchored_cables = new AnchoredParticleCable[kAnchoredCableCount];
    for(unsigned i = 0; i < kAnchoredCableCount; ++i)
    {
        m_anchored_cables[i].m_particle = m_particles[i];
        m_anchored_cables[i].m_anchor   = {real(i / 2) * 2.2f - 5.5f, 6.0f, real(i % 2) * 1.6f - 0.8f};
        if(i < 6)
        {
            m_anchored_cables[i].m_max_length = real(i / 2) * 0.5f + 3.0f;
        }
        else
        {
            m_anchored_cables[i].m_max_length = 5.5f - real(i / 2) * 0.5f;
        }
        m_anchored_cables[i].m_restitution = 0.5f;
        m_particle_world.getContactGenerators().push_back(&m_anchored_cables[i]);
    }

    m_rods = new ParticleRod[kRodCount];
    for(unsigned i = 0; i < kRodCount; ++i)
    {
        m_rods[i].m_particles[0] = m_particles[i * 2];
        m_rods[i].m_particles[1] = m_particles[i * 2 + 1];
        m_rods[i].m_length       = 2.0f;
        m_particle_world.getContactGenerators().push_back(&m_rods[i]);
    }
}

void RopeBridgeDemoLayer::OnDetach()
{
    // delete particles
    for(auto& particle : m_particles)
    {
        delete particle;
    }

    delete[] m_rods;
    delete[] m_cables;
    delete[] m_anchored_cables;
}

void RopeBridgeDemoLayer::OnUpdate(const core::TimeStep& time_step)
{

    m_particle_world.startFrame();

    // const auto& editor_camera   = sputnik::graphics::api::EditorCamera::GetInstance();
    // Matrix4     projection      = editor_camera->GetCameraPerspective();
    // Matrix4     view            = editor_camera->GetCameraView();
    // vec3        camera_position = editor_camera->GetCameraPosition();

    const auto& camera          = Camera::GetInstance();
    mat4        projection      = camera->GetCameraPerspective();
    mat4        view            = camera->GetCameraView();
    vec3        camera_position = camera->GetCameraPosition();

    // render light source
    {
        m_static_shader->Bind();

        Uniform<mat4>::Set(m_static_shader->GetUniform("view"), view);
        Uniform<mat4>::Set(m_static_shader->GetUniform("projection"), projection);
        Uniform<vec3>::Set(m_static_shader->GetUniform("light"), m_light.position);
        m_static_mesh_texture->Set(m_static_shader->GetUniform("diffuse"), 0);

        mat4 model_mat4;
        model_mat4 = model_mat4.translate({m_light.position.x, m_light.position.y, m_light.position.z});
        model_mat4 = model_mat4.scale({0.20f, 0.20f, 0.20f});
        Uniform<mat4>::Set(m_static_shader->GetUniform("model"), model_mat4);

        m_sphere->Draw(m_static_shader, true, false, false);

        m_static_mesh_texture->Unset(0);
        m_static_shader->Unbind();
    }

    m_simple_lighting_shader->Bind();
    Uniform<mat4>::Set(m_simple_lighting_shader->GetUniform("view"), view);
    Uniform<mat4>::Set(m_simple_lighting_shader->GetUniform("projection"), projection);
    Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("eye_position"), camera_position);

    Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("light.position"), m_light.position);
    Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("light.ambient"), m_light.ambient);
    Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("light.diffuse"), m_light.diffuse);
    Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("light.specular"), m_light.specular);

    m_particle_world.simulatePhysics(time_step);

    for(unsigned i = 0; i < m_particles.size(); ++i)
    {
        const auto& particle = m_particles[i];
        // particle->integrate(time_step);
        mat4 model{};
        model              = model.translate(particle->getPosition());
        model              = model.scale({0.15f});
        mat4 normal_matrix = model.inverted().transpose();
        Uniform<mat4>::Set(m_simple_lighting_shader->GetUniform("model"), model);
        Uniform<mat4>::Set(m_simple_lighting_shader->GetUniform("normal_matrix"), normal_matrix);

        switch(i)
        {
        case 0:
        case 1:
        case 6:
        case 7:
        {
            Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("material.ambient"), material_pearl.ambient);
            Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("material.diffuse"), material_pearl.diffuse);
            Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("material.specular"), material_pearl.specular);
            Uniform<float>::Set(m_simple_lighting_shader->GetUniform("material.shininess"), material_pearl.shininess);
            break;
        }
        case 2:
        case 3:
        case 8:
        case 9:
        {
            Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("material.ambient"), material_ruby.ambient);
            Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("material.diffuse"), material_ruby.diffuse);
            Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("material.specular"), material_ruby.specular);
            Uniform<float>::Set(m_simple_lighting_shader->GetUniform("material.shininess"), material_ruby.shininess);
            break;
        }
        case 4:
        case 5:
        case 10:
        case 11:
        {
            Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("material.ambient"), material_gold.ambient);
            Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("material.diffuse"), material_gold.diffuse);
            Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("material.specular"), material_gold.specular);
            Uniform<float>::Set(m_simple_lighting_shader->GetUniform("material.shininess"), material_gold.shininess);
            break;
        }
        default:
            break;
        }

        m_sphere->Draw(m_simple_lighting_shader, false, false, false);
    }
    m_simple_lighting_shader->Unbind();

    // render joints
    glLineWidth(5.0);
    mat4 mvp = projection * view;
    {
        for(unsigned i = 0; i < kRodCount; ++i)
        {
            const auto& rod      = m_rods[i];
            vec3        position = rod.m_particles[0]->getPosition();
            m_debug_lines->Push({position.x, position.y, position.z});
            position = rod.m_particles[1]->getPosition();
            m_debug_lines->Push({position.x, position.y, position.z});
            m_debug_lines->UpdateOpenGLBuffers();
            m_debug_lines->Draw(DebugDrawMode::Lines, {0.0f, 0.0f, 1.0f}, mvp);
            m_debug_lines->Clear();
        }

        for(unsigned i = 0; i < kCableCount; ++i)
        {
            const auto& cable    = m_cables[i];
            vec3        position = cable.m_particles[0]->getPosition();
            m_debug_lines->Push({position.x, position.y, position.z});
            position = cable.m_particles[1]->getPosition();
            m_debug_lines->Push({position.x, position.y, position.z});
            m_debug_lines->UpdateOpenGLBuffers();
            m_debug_lines->Draw(DebugDrawMode::Lines, {0.0f, 1.0f, 0.0f}, mvp);
            m_debug_lines->Clear();
        }

        for(unsigned i = 0; i < kAnchoredCableCount; ++i)
        {
            const auto& cable    = m_anchored_cables[i];
            vec3        position = cable.m_particle->getPosition();
            m_debug_lines->Push({position.x, position.y, position.z});
            m_debug_lines->Push({cable.m_anchor.x, cable.m_anchor.y, cable.m_anchor.z});
            m_debug_lines->UpdateOpenGLBuffers();
            m_debug_lines->Draw(DebugDrawMode::Lines, {0.7f, 0.7f, 0.7f}, mvp);
            m_debug_lines->Clear();

            m_simple_lighting_shader->Bind();

            Uniform<mat4>::Set(m_simple_lighting_shader->GetUniform("view"), view);
            Uniform<mat4>::Set(m_simple_lighting_shader->GetUniform("projection"), projection);
            Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("eye_position"), camera_position);

            Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("light.position"), m_light.position);
            Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("light.ambient"), m_light.ambient);
            Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("light.diffuse"), m_light.diffuse);
            Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("light.specular"), m_light.specular);

            mat4 model{};
            model              = model.translate({cable.m_anchor.x, cable.m_anchor.y, cable.m_anchor.z});
            model              = model.scale({0.15f});
            mat4 normal_matrix = model.inverted().transpose();
            Uniform<mat4>::Set(m_simple_lighting_shader->GetUniform("model"), model);
            Uniform<mat4>::Set(m_simple_lighting_shader->GetUniform("normal_matrix"), normal_matrix);

            Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("material.ambient"), material_ruby.ambient);
            Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("material.diffuse"), material_ruby.diffuse);
            Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("material.specular"), material_ruby.specular);
            Uniform<float>::Set(m_simple_lighting_shader->GetUniform("material.shininess"), material_ruby.shininess);

            m_sphere->Draw(m_simple_lighting_shader, false, false, false);

            m_simple_lighting_shader->Unbind();
        }
    }

    ++debug_count;
}

void RopeBridgeDemoLayer::OnEvent() {}

void RopeBridgeDemoLayer::OnUpdateUI(const core::TimeStep& time_step)
{
    const auto& camera     = sputnik::graphics::api::Camera::GetInstance();
    mat4        projection = camera->GetCameraPerspective();
    mat4        view       = camera->GetCameraView();

    if(ImGui::Begin("Lighting"))
    {
        ImGui::SliderFloat("Light X", &m_light.position.x, -50.0f, 50.0f);
        ImGui::SliderFloat("Light Y", &m_light.position.y, -50.0f, 50.0f);
        ImGui::SliderFloat("Light Z", &m_light.position.z, -50.0f, 50.0f);

        ImGui::ColorEdit3("Light ambient", &m_light.ambient.x);
        ImGui::ColorEdit3("Light diffuse", &m_light.diffuse.x);
        ImGui::ColorEdit3("Light specular", &m_light.specular.x);

        ImGui::SliderFloat("Light constant", &m_light.constant, 0.0f, 1.0f);
        ImGui::SliderFloat("Light linear", &m_light.linear, 0.0f, 1.0f);
        ImGui::SliderFloat("Light quadratic", &m_light.quadratic, 0.0f, 1.0f);
    }
    ImGui::End();

    if(ImGui::Begin("Particles"))
    {
        // ImGui::Combo("Particle", &m_particle_idx, m_particle_str.c_str());
        // Particle* particle = m_particles[m_particle_idx];
        // ImGuizmo::SetGizmoSizeClipSpace(0.075f);
        // mat4 model = {};
        // model      = model.translate(particle->getPosition());
        // ImGuizmo::Manipulate(&view.m[0],
        //                      &projection.m[0],
        //                      ImGuizmo::OPERATION::TRANSLATE,
        //                      ImGuizmo::MODE::LOCAL,
        //                      &model.m[0],
        //                      nullptr,
        //                      nullptr);
        // if(ImGuizmo::IsUsing())
        //{
        //     particle->setPosition({model.m[12], model.m[13], model.m[14]});

        //    // We need to block camera update when we are using ImGuizmo
        //    Renderer::BlockCameraUpdate();
        //}
        // else
        //{
        //    Renderer::BlockCameraUpdate(false);
        //}
    }
    ImGui::End();
}

void RopeBridgeDemoLayer::UpdateAdditionalMass() {}

} // namespace sputnik::demos
