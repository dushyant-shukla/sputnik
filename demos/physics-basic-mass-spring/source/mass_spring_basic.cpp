#include "mass_spring_basic.h"

#include <graphics/api/renderer.h>
#include <graphics/glcore/uniform.h>
#include <graphics/api/color_material.h>

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
}

MassSpringBasicDemoLayer::~MassSpringBasicDemoLayer() {}

void MassSpringBasicDemoLayer::OnAttach()
{
    Renderer::SetCameraType(CameraType::Camera);

    m_static_shader = std::make_shared<Shader>("../../data/shaders/simple.vert", "../../data/shaders/simple.frag");

    m_simple_lighting_shader = std::make_shared<Shader>("../../data/shaders/simple-lighting/simple-lighting.vert",
                                                        "../../data/shaders/simple-lighting/simple-lighting.frag");

    m_static_mesh_texture = std::make_shared<Texture>("../../data/assets/dq.png");

    m_box    = Model::LoadModel("../../data/assets/box/Box.gltf");
    m_sphere = Model::LoadModel("../../data/assets/sphere.gltf");

    // anchor particle
    m_particles.push_back(new Particle());
    m_particles.back()->setPosition({0.0f, 1.0f, 0.0f});
    m_particles.back()->setMass(10.0f); // 10 kg
    m_particles.back()->setDamping(0.5f);
    m_particle_str += "Anchor";
    m_particle_str += '\0';

    // particle connected to the anchor with a spring
    m_particles.push_back(new Particle());
    m_particles.back()->setPosition({0.0f, 1.0f, 0.0f});
    m_particles.back()->setMass(10.0f); // 10 kg
    m_particles.back()->setDamping(0.5f);
    m_particles.back()->setAcceleration({0.0f, -9.81f, 0.0f});
    m_particle_str += "Anchored particle";
    m_particle_str += '\0';

    // particle a
    m_particles.push_back(new Particle());
    m_particles.back()->setPosition({1.0f, 1.0f, 0.0f});
    m_particles.back()->setMass(10.0f); // 10 kg
    m_particles.back()->setDamping(0.5f);
    m_particle_str += "Spring particle a";
    m_particle_str += '\0';

    // particle b
    m_particles.push_back(new Particle());
    m_particles.back()->setPosition({2.0f, 1.0f, 0.0f});
    m_particles.back()->setMass(10.0f); // 10 kg
    m_particles.back()->setDamping(0.5f);
    m_particle_str += "Spring particle b";
    m_particle_str += '\0';
    m_particle_str += '\0';

    m_spring_force_generator_a = new ParticleSpringForceGenerator(m_particles[3], 50.0f, 1.0f);
    m_particle_force_registry.add(m_particles[2], m_spring_force_generator_a);

    m_spring_force_generator_b = new ParticleSpringForceGenerator(m_particles[2], 50.0f, 1.0f);
    m_particle_force_registry.add(m_particles[3], m_spring_force_generator_b);

    m_anchored_bungee_force_generator =
        new ParticleAnchoredBungeeForceGenerator(m_particles[0]->getPosition(), 50.0f, 1.0f);
    m_particle_force_registry.add(m_particles[1], m_anchored_bungee_force_generator);

    m_light.ambient  = {1.0f, 1.0f, 1.0f};
    m_light.diffuse  = {1.0f, 1.0f, 1.0f};
    m_light.specular = {1.0f, 1.0f, 1.0f};
}

void MassSpringBasicDemoLayer::OnDetach()
{
    delete m_anchored_bungee_force_generator;
    delete m_spring_force_generator_a;
    delete m_spring_force_generator_b;

    for(auto& particle : m_particles)
    {
        delete particle;
    }
}

void MassSpringBasicDemoLayer::OnUpdate(const core::TimeStep& time_step)
{
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

    m_anchored_bungee_force_generator->setAnchor(m_particles[0]->getPosition());
    m_particle_force_registry.updateForces(time_step);

    m_simple_lighting_shader->Bind();
    Uniform<mat4>::Set(m_simple_lighting_shader->GetUniform("view"), view);
    Uniform<mat4>::Set(m_simple_lighting_shader->GetUniform("projection"), projection);
    Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("eye_position"), camera_position);

    Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("light.position"), m_light.position);
    Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("light.ambient"), m_light.ambient);
    Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("light.diffuse"), m_light.diffuse);
    Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("light.specular"), m_light.specular);

    for(unsigned i = 0; i < m_particles.size(); ++i)
    {
        const auto& particle = m_particles[i];
        particle->integrate(time_step);
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
        {
            Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("material.ambient"), material_pearl.ambient);
            Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("material.diffuse"), material_pearl.diffuse);
            Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("material.specular"), material_pearl.specular);
            Uniform<float>::Set(m_simple_lighting_shader->GetUniform("material.shininess"), material_pearl.shininess);
            break;
        }
        case 2:
        case 3:
        {
            Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("material.ambient"), material_ruby.ambient);
            Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("material.diffuse"), material_ruby.diffuse);
            Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("material.specular"), material_ruby.specular);
            Uniform<float>::Set(m_simple_lighting_shader->GetUniform("material.shininess"), material_ruby.shininess);
            break;
        }
        default:
            break;
        }

        m_sphere->Draw(m_simple_lighting_shader, false, false, false);
    }
    m_simple_lighting_shader->Unbind();
}

void MassSpringBasicDemoLayer::OnEvent() {}

void MassSpringBasicDemoLayer::OnUpdateUI(const core::TimeStep& time_step)
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
        ImGui::Combo("Particle", &m_particle_idx, m_particle_str.c_str());
        Particle* particle = m_particles[m_particle_idx];
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
            Renderer::BlockCameraUpdate();
        }
        else
        {
            Renderer::BlockCameraUpdate(false);
        }
    }
    ImGui::End();
}

} // namespace sputnik::demos
