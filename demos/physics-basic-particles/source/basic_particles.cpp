#include "basic_particles.h"

#include <graphics/glcore/uniform.h>
#include <graphics/glcore/renderer.h>
#include <matrix4.h>
#include <vector3.h>
#include <camera_transforms.h>
#include <imgui.h>
#include <editor/editor_camera.h>
#include <graphics/api/camera.h>
#include <core/input/input_manager.h>
#include <physics/geometry.h>

#include <vector.hpp>
#include <matrix.hpp>

#include <glad/glad.h>
#include <optional>

namespace sputnik::demos
{

using namespace ramanujan;
using namespace experimental;
using namespace sputnik::graphics::glcore;
using namespace sputnik::physics;

BasicParticlesDemoLayer::BasicParticlesDemoLayer(const std::string& name) : core::Layer(name) {}

BasicParticlesDemoLayer::~BasicParticlesDemoLayer() {}

void BasicParticlesDemoLayer::OnAttach()
{
    Triangle t;
    int      a = 10;
    // experimental::vec3 test_position{1.0f, 2.0f, 3.0f};
    // auto               norm_res = test_position.normalized();

    // experimental::vec3 test_position_3{4.0f, 5.0f, 6.0f};

    // test_position += test_position_3;
    // test_position -= test_position_3;
    // test_position *= test_position_3;
    // test_position /= test_position_3;

    // test_position += 2.0f;
    // test_position -= 2.0f;
    // test_position *= 2.0f;
    // test_position /= 2.0f;

    // experimental::vec3 add_result = test_position + test_position_3;
    // experimental::vec3 sub_result = test_position - test_position_3;
    // experimental::vec3 mul_result = test_position * test_position_3;
    // experimental::vec3 div_result = test_position / test_position_3;

    // experimental::vec3 add_result_1 = test_position + 3.0f;
    // experimental::vec3 sub_result_1 = test_position - 3.0f;
    // experimental::vec3 mul_result_1 = test_position * 3.0f;
    // experimental::vec3 div_result_1 = test_position / 3.0f;

    // auto slerp_res = slerp(add_result, add_result_1, real(0.4));
    // auto lerp_res  = lerp(add_result, add_result_1, real(0.4));
    // auto nlerp_res = nlerp(add_result, add_result_1, real(0.4));
    // auto bool_0    = add_result.isOrthogonal(add_result_1);
    // auto bool_1    = add_result.isParallel(add_result_1);
    // auto bool_2    = add_result.isZero();

    // mat4 sample_mat4{real(3),
    //                  real(4),
    //                  real(5),
    //                  real(8),
    //                  real(34),
    //                  real(56),
    //                  real(38),
    //                  real(59),
    //                  real(50),
    //                  real(2),
    //                  real(37),
    //                  real(11),
    //                  real(80),
    //                  real(13),
    //                  real(78),
    //                  real(15)};
    // std::cout << sample_mat4 << std::endl;
    // auto cof_mat = sample_mat4.inverted();
    // std::cout << cof_mat << std::endl;

    // float deter = sample_mat4.determinant();
    // std::cout << "determinant: " << deter << std::endl;

    // std::cout << "MATRIX4" << std::endl;
    // Matrix4 sample_matrix_4{real(3),
    //                         real(4),
    //                         real(5),
    //                         real(8),
    //                         real(34),
    //                         real(56),
    //                         real(38),
    //                         real(59),
    //                         real(50),
    //                         real(2),
    //                         real(37),
    //                         real(11),
    //                         real(80),
    //                         real(13),
    //                         real(78),
    //                         real(15)};
    // deter = sample_mat4.determinant();
    // std::cout << "determinant: " << deter << std::endl;

    // experimental::vec3 lerp_result  = lerp(test_position, test_position_3, 0.5f);
    // experimental::vec3 slerp_result = slerp(test_position, test_position_3, 0.5f);

    // experimental::vec3 test_position_4 = test_position.cross(test_position_3);

    // std::cout << "Before updating: " << test_position << std::endl;

    // test_position.x = 50.0f;
    // test_position.y = 40.0f;
    // test_position.z = 80.0f;
    // std::cout << "After updating: " << test_position << std::endl;

    // test_position.z = 160.0f;
    // std::cout << "After updating: " << test_position << std::endl;

    // experimental::vec3 test_position_1{25.0f};
    // std::cout << "Before updating: " << test_position_1 << std::endl;

    // test_position_1.y = 70.0f;
    // std::cout << "After updating: " << test_position_1 << std::endl;

    // test_position_1.z = 320.0f;
    // std::cout << "After updating: " << test_position_1 << std::endl;

    // test_position.normalize();

    // test_position_3 = test_position_1.normalized();

    // bool b1 = test_position == test_position_1;
    // bool b2 = test_position <= test_position_1;
    // bool b3 = test_position < test_position_1;
    // bool b4 = test_position > test_position_1;
    // bool b5 = test_position >= test_position_1;
    // bool b6 = test_position != test_position_1;

    // bool is_zero       = test_position.isZero();
    // bool is_parallel   = test_position.isParallel(test_position_1);
    // bool is_orthogonal = test_position.isOrthogonal(test_position_1);

    // bool is_orthonormalized = orthonormalize(add_result, mul_result, div_result);

    // float angl = angle(add_result, mul_result);

    // auto project        = projection(add_result, mul_result);
    // auto reject         = rejection(add_result, mul_result);
    // auto reflect        = reflection(add_result, mul_result);
    // auto is_orthonormal = orthonormalize(add_result, sub_result, mul_result);

    // auto xy = project.xy();

    ////experimental::color4 test_color{50.0f};
    ////std::cout << "Before updating: " << test_color << std::endl;

    ////test_color.r = 100.0f;
    ////std::cout << "After updating: " << test_color << std::endl;

    // experimental::vec2 test_uv_1{0.0f, 5.0f};
    // experimental::vec2 test_uv_2{0.0f, 1.0f};
    // test_uv_1 = test_uv_2;

    m_static_shader = std::make_shared<Shader>("../../data/shaders/simple.vert", "../../data/shaders/simple.frag");
    m_sky_shader    = std::make_shared<Shader>("../../data/shaders/sky-rendering/sky.vert",
                                            "../../data/shaders/sky-rendering/sky.frag");
    m_simple_lighting_shader = std::make_shared<Shader>("../../data/shaders/simple-lighting/simple-lighting.vert",
                                                        "../../data/shaders/simple-lighting/simple-lighting.frag");

    m_static_mesh_texture = std::make_shared<Texture>("../../../../data/assets/dq.png");

    m_box    = Model::LoadModel("../../data/assets/box/Box.gltf");
    m_sphere = Model::LoadModel("../../data/assets/sphere.gltf");

    // red rubber
    // m_platform_material.ambient   = {0.05f, 0.0f, 0.0f};
    // m_platform_material.diffuse   = {0.5f, 0.4f, 0.4f};
    // m_platform_material.specular  = {0.7f, 0.4f, 0.4f};
    // m_platform_material.shininess = 0.078125f;

    // rubber green
    m_platform_material.ambient   = {0.0f, 0.05f, 0.0f};
    m_platform_material.diffuse   = {0.4f, 0.5f, 0.4f};
    m_platform_material.specular  = {0.04f, 0.7f, 0.04f};
    m_platform_material.shininess = 0.078125f;

    m_particle_silver.ambient   = {0.19225f, 0.19225f, 0.19225f};
    m_particle_silver.diffuse   = {0.50754f, 0.50754f, 0.50754f};
    m_particle_silver.specular  = {0.508273f, 0.508273f, 0.508273f};
    m_particle_silver.shininess = 0.4f;
    m_particle_materials[0]     = m_particle_silver;

    m_particle_gold.ambient   = {0.24725f, 0.1995f, 0.0745f};
    m_particle_gold.diffuse   = {0.75164f, 0.60648f, 0.22648f};
    m_particle_gold.specular  = {0.628281f, 0.555802f, 0.366065f};
    m_particle_gold.shininess = 0.4f;
    m_particle_materials[1]   = m_particle_gold;

    m_particle_pearl.ambient   = {0.25f, 0.20725f, 0.20725f};
    m_particle_pearl.diffuse   = {1.0f, 0.829f, 0.829f};
    m_particle_pearl.specular  = {0.296648f, 0.296648f, 0.296648f};
    m_particle_pearl.shininess = 0.088f;
    m_particle_materials[2]    = m_particle_pearl;

    m_particle_chrome.ambient   = {0.25f, 0.25f, 0.25f};
    m_particle_chrome.diffuse   = {0.4f, 0.4f, 0.4f};
    m_particle_chrome.specular  = {0.774597f, 0.774597f, 0.774597f};
    m_particle_chrome.shininess = 0.088f;
    m_particle_materials[3]     = m_particle_chrome;

    m_particle_ruby.ambient   = {0.1745f, 0.01175f, 0.01175f};
    m_particle_ruby.diffuse   = {0.61424f, 0.04136f, 0.04136f};
    m_particle_ruby.specular  = {0.727811f, 0.626959f, 0.626959f};
    m_particle_ruby.shininess = 0.6f;
    m_particle_materials[4]   = m_particle_ruby;

    const auto& editor_camera = sputnik::graphics::api::EditorCamera::GetInstance();
    editor_camera->SetPosition({0.0f, 0.0f, 10.0f});

    const auto& camera = sputnik::graphics::api::Camera::GetInstance();
    camera->SetPosition({0.0f, 5.0f, 10.0f});

    // setting up particles
    {
        sputnik::physics::Particle particle;
        particle.setPosition(0.0f, 10.0f, 0.0f);
        particle.setMass(2.0f);                        // 2.0Kg
        particle.setAcceleration({0.0f, -1.0f, 0.0f}); // gravity
        particle.setDamping(0.09f);
        m_particles.emplace_back(particle);

        m_particle_material.ambient   = {0.24725f, 0.1995f, 0.0745f};
        m_particle_material.diffuse   = {0.75164f, 0.60648f, 0.22648f};
        m_particle_material.specular  = {0.628281f, 0.555802f, 0.366065f};
        m_particle_material.shininess = 0.4f;
    }

    // glPointSize(5.0f);
    // glLineWidth(1.5f);

    // Firework setup
    for(auto& firework_rule : m_fireworkwork_rules)
    {
        firework_rule.m_type = 0;
    }

    for(auto& fire_work : m_fireworks)
    {
        fire_work.m_type = 0;
    }

    InitFireworkRules();
}

// Todo: OnDetach() must be called at system cleanup before shutdown
void BasicParticlesDemoLayer::OnDetach() {}

void BasicParticlesDemoLayer::OnUpdate(const core::TimeStep& time_step)
{
    if(time_step.GetSeconds() <= Constants::EPSILON)
    {
        return;
    }

    // create projectiles
    const auto& input_manager = core::InputManager::GetInstance();
    if(input_manager->IsMouseButtonTriggered(MOUSE_BUTTON_RIGHT))
    {
        sputnik::physics::Particle particle;

        // pistol
        particle.setPosition(0.0f, 5.0f, 0.0f);
        particle.setMass(2.0f); // 2.0Kg
        particle.setVelocity(0.0f, 0.0f, -35.0f);
        particle.setAcceleration({0.0f, -1.0f, 0.0f});
        particle.setDamping(0.99f);
        m_particles.emplace_back(particle);

        // artillery
        particle.setPosition(0.0f, 5.0f, 0.0f);
        particle.setMass(200.0f); // 2.0Kg
        particle.setVelocity(0.0f, 30.0f, -40.0f);
        particle.setAcceleration({0.0f, -20.0f, 0.0f});
        particle.setDamping(0.99f);

        m_particles.emplace_back(particle);
    }

    if(input_manager->IsKeyPressed(KEY_1))
    {
        SpawnFireworks(1, 1, std::nullopt);
    }
    if(input_manager->IsKeyPressed(KEY_2))
    {
        SpawnFireworks(2, 1, std::nullopt);
    }
    if(input_manager->IsKeyPressed(KEY_3))
    {
        SpawnFireworks(3, 1, std::nullopt);
    }
    if(input_manager->IsKeyPressed(KEY_4))
    {
        SpawnFireworks(4, 1, std::nullopt);
    }
    if(input_manager->IsKeyPressed(KEY_5))
    {
        SpawnFireworks(5, 1, std::nullopt);
    }
    if(input_manager->IsKeyPressed(KEY_6))
    {
        SpawnFireworks(5, 1, std::nullopt);
    }
    if(input_manager->IsKeyPressed(KEY_7))
    {
        SpawnFireworks(7, 1, std::nullopt);
    }
    if(input_manager->IsKeyPressed(KEY_8))
    {
        SpawnFireworks(8, 1, std::nullopt);
    }
    if(input_manager->IsKeyPressed(KEY_9))
    {
        SpawnFireworks(9, 1, std::nullopt);
    }

    Matrix4 model_matrix;
    Matrix4 normal_matrix;

    // const auto& editor_camera = sputnik::graphics::api::EditorCamera::GetInstance();
    // Matrix4     projection    = editor_camera->GetCameraPerspective();
    // Matrix4     view          = editor_camera->GetCameraView();

    const auto& camera          = sputnik::graphics::api::Camera::GetInstance();
    const auto& p               = camera->GetCameraPerspective();
    const auto& v               = camera->GetCameraView();
    const auto& camera_position = camera->GetCameraPosition();
    // for(unsigned int i = 0; i < 16; ++i)
    //{
    //     projection.v[i] = p.m[i];
    //     view.v[i]       = v.m[i];
    // }

    glEnable(GL_DEPTH_TEST);
    // render light source
    {
        m_static_shader->Bind();
        Uniform<mat4>::Set(m_static_shader->GetUniform("view"), v);
        Uniform<mat4>::Set(m_static_shader->GetUniform("projection"), p);
        Uniform<vec3>::Set(m_static_shader->GetUniform("light"), m_light.position);

        m_static_mesh_texture->Set(m_static_shader->GetUniform("diffuse"), 0);

        // old math api
        // Transform transform({m_light.position.x, m_light.position.y, m_light.position.z}, {}, {0.20f, 0.20f, 0.20f});
        // model_matrix = ToMatrix4(transform);
        // Uniform<Matrix4>::Set(m_static_shader->GetUniform("model"), model_matrix);

        mat4 model_mat4;
        model_mat4 = model_mat4.translate({m_light.position.x, m_light.position.y, m_light.position.z});
        model_mat4 = model_mat4.scale({0.20f, 0.20f, 0.20f});
        Uniform<mat4>::Set(m_static_shader->GetUniform("model"), model_mat4);

        m_sphere->Draw(m_static_shader, true, false, false);

        m_static_mesh_texture->Unset(0);
        m_static_shader->Unbind();
    }

    // phong lighting
    {
        m_simple_lighting_shader->Bind();

        // Uniform<Matrix4>::Set(m_simple_lighting_shader->GetUniform("view"), view);
        // Uniform<Matrix4>::Set(m_simple_lighting_shader->GetUniform("projection"), projection);
        Uniform<mat4>::Set(m_simple_lighting_shader->GetUniform("view"), v);
        Uniform<mat4>::Set(m_simple_lighting_shader->GetUniform("projection"), p);

        // light
        Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("light.position"), m_light.position);
        // Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("light.position"), mDirection);
        Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("light.ambient"), m_light.ambient);
        Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("light.diffuse"), m_light.diffuse);
        Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("light.specular"), m_light.specular);

        // camera
        Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("eye_position"), camera_position);

        // render the platform
        {

            // old maths+transform api
            // Transform transform({0.0f, -5.0f, 0.0f}, {}, {100.0f, 1.0f, 100.0f});
            // model_matrix  = ToMatrix4(transform);
            // normal_matrix = Transposed(Inverse(model_matrix));
            // Uniform<Matrix4>::Set(m_simple_lighting_shader->GetUniform("model"), model_matrix);
            // Uniform<Matrix4>::Set(m_simple_lighting_shader->GetUniform("normal_matrix"), normal_matrix);

            mat4 model_mat4;
            model_mat4       = model_mat4.translate({0.0f, -5.0f, 0.0f});
            model_mat4       = model_mat4.scale({100.0f, 1.0f, 100.0f});
            mat4 normal_mat4 = model_mat4.inverted().transpose();
            Uniform<mat4>::Set(m_simple_lighting_shader->GetUniform("model"), model_mat4);
            Uniform<mat4>::Set(m_simple_lighting_shader->GetUniform("normal_matrix"), normal_mat4);

            // material
            Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("material.ambient"), m_platform_material.ambient);
            Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("material.diffuse"), m_platform_material.diffuse);
            Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("material.specular"), m_platform_material.specular);
            Uniform<float>::Set(m_simple_lighting_shader->GetUniform("material.shininess"),
                                m_platform_material.shininess);

            m_box->Draw(m_simple_lighting_shader, false, false, false);
        }

        // render particles
        {
            for(unsigned int i = 0; i < m_particles.size(); ++i)
            {
                m_particles[i].integrate(time_step.GetSeconds());

                const auto& pos = m_particles[i].getPosition();

                // old maths+transform api
                // Transform   transform({pos.x, pos.y, pos.z}, {}, {0.15f});
                // model_matrix  = ToMatrix4(transform);
                // normal_matrix = Transposed(Inverse(model_matrix));
                // Uniform<Matrix4>::Set(m_simple_lighting_shader->GetUniform("model"), model_matrix);
                // Uniform<Matrix4>::Set(m_simple_lighting_shader->GetUniform("normal_matrix"), normal_matrix);

                mat4 model_mat4;
                model_mat4       = model_mat4.translate({pos.x, pos.y, pos.z});
                model_mat4       = model_mat4.scale({0.15f});
                mat4 normal_mat4 = model_mat4.inverted().transpose();
                Uniform<mat4>::Set(m_simple_lighting_shader->GetUniform("model"), model_mat4);
                Uniform<mat4>::Set(m_simple_lighting_shader->GetUniform("normal_matrix"), normal_mat4);

                // material
                Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("material.ambient"),
                                   m_particle_material.ambient);
                Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("material.diffuse"),
                                   m_particle_material.diffuse);
                Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("material.specular"),
                                   m_particle_material.specular);
                Uniform<float>::Set(m_simple_lighting_shader->GetUniform("material.shininess"),
                                    m_particle_material.shininess);

                m_sphere->Draw(m_simple_lighting_shader, false, false, false);
            }
        }

        // render fireworks
        {
            for(auto& firework : m_fireworks)
            {
                if(firework.m_type > 0) // if valid
                {
                    // if the firework needs removing
                    if(firework.update(time_step))
                    {

                        FireworkRule& firework_rule = m_fireworkwork_rules[firework.m_type - 1];
                        firework.m_type             = 0; // stop the firework for physics and graphics processing
                        for(const auto& firework_payload : firework_rule.m_payloads)
                        {
                            SpawnFireworks(firework_payload.m_type, firework_payload.m_count, firework);
                            std::cout << "Firework payload spawned" << std::endl;
                        }
                    }

                    const auto& pos = firework.getPosition();

                    // old maths+transform api
                    // Transform   transform({pos.x, pos.y, pos.z}, {}, {0.15f});
                    // model_matrix  = ToMatrix4(transform);
                    // normal_matrix = Transposed(Inverse(model_matrix));
                    // Uniform<Matrix4>::Set(m_simple_lighting_shader->GetUniform("model"), model_matrix);
                    // Uniform<Matrix4>::Set(m_simple_lighting_shader->GetUniform("normal_matrix"), normal_matrix);

                    mat4 model_mat4;
                    model_mat4       = model_mat4.translate({pos.x, pos.y, pos.z});
                    model_mat4       = model_mat4.scale({0.15f});
                    mat4 normal_mat4 = model_mat4.inverted().transpose();
                    Uniform<mat4>::Set(m_simple_lighting_shader->GetUniform("model"), model_mat4);
                    Uniform<mat4>::Set(m_simple_lighting_shader->GetUniform("normal_matrix"), normal_mat4);

                    // m_particle_material.ambient   = {0.24725f, 0.1995f, 0.0745f};
                    // m_particle_material.diffuse   = {0.75164f, 0.60648f, 0.22648f};
                    // m_particle_material.specular  = {0.628281f, 0.555802f, 0.366065f};
                    // m_particle_material.shininess = 0.4f;
                    //  material
                    // Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("material.ambient"),
                    //                    s_random.randomVector({ 0.01, 0.01, 0.01 }, {0.24725f, 0.1995f, 0.0745f}));
                    // Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("material.diffuse"),
                    //                    s_random.randomVector({0.2, 0.2, 0.2}, {0.75164f, 0.60648f, 0.22648f}));
                    // Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("material.specular"),
                    //                    s_random.randomVector({0.3, 0.3, 0.3}, {0.628281f, 0.555802f, 0.366065f}));

                    int         index    = s_random.randomInt(4);
                    const auto& material = m_particle_materials[index];

                    Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("material.ambient"), material.ambient);
                    Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("material.diffuse"), material.diffuse);
                    Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("material.specular"), material.specular);
                    Uniform<float>::Set(m_simple_lighting_shader->GetUniform("material.shininess"), material.shininess);

                    m_sphere->Draw(m_simple_lighting_shader, false, false, false);
                }
            }
        }

        m_simple_lighting_shader->Unbind();
    }
    glDisable(GL_DEPTH_TEST);
}

void BasicParticlesDemoLayer::OnEvent() {}

void BasicParticlesDemoLayer::OnUpdateUI(const core::TimeStep& time_step)
{
    if(ImGui::Begin("Basic Particles"))
    {
    }
    ImGui::End();

    if(ImGui::Begin("Lighting"))
    {
        ImGui::SliderFloat("Light X", &m_light.position.x, -50.0f, 50.0f);
        ImGui::SliderFloat("Light Y", &m_light.position.y, -50.0f, 50.0f);
        ImGui::SliderFloat("Light Z", &m_light.position.z, -50.0f, 50.0f);

        ImGui::ColorEdit3("Light ambient", &m_light.ambient.x);
        ImGui::ColorEdit3("Light diffuse", &m_light.diffuse.x);
        ImGui::ColorEdit3("Light specular", &m_light.specular.x);
    }
    ImGui::End();

    ImGui::Begin("Atmoshperic Scattering");

    ImGui::SliderFloat("Exposure", &m_exposure, 0, 10.0f);
    ImGui::SliderAngle("Sun Angle", &m_sun_angle, 0.0f, -180.0f);

    float turbidity = mPreethamSkyModel.GetTurbidity();
    ImGui::SliderFloat("Turbidity", &turbidity, 2.0f, 30.0f);
    mPreethamSkyModel.SetTurbidity(turbidity);

    mDirection = vec3(0.0f, sin(m_sun_angle), cos(m_sun_angle)).normalized();

    ImGui::Text("Sun Direction = [ %f, %f, %f ]", mDirection.x, mDirection.y, mDirection.z);

    ImGui::End();
}

void BasicParticlesDemoLayer::SpawnFirework(unsigned int type, const std::optional<Firework>& parent)
{
    FireworkRule& firework_rule = m_fireworkwork_rules[type - 1];
    firework_rule.Create(m_fireworks[m_next_firework], parent);
    m_next_firework = (m_next_firework + 1) % kMaxFireworks;
}

void BasicParticlesDemoLayer::SpawnFireworks(unsigned int                   type,
                                             unsigned int                   number,
                                             const std::optional<Firework>& parent)
{
    for(unsigned int i = 0; i < number; ++i)
    {
        SpawnFirework(type, parent);
    }
}

void BasicParticlesDemoLayer::InitFireworkRules()
{
    m_fireworkwork_rules[0].SetRuleParameters(1, 0.5f, 1.4f, {-5.0f, 25.0f, -5.0f}, {5.0f, 28.0f, 5.0f}, 0.1f);
    m_fireworkwork_rules[0].m_payloads.emplace_back(3, 5);
    m_fireworkwork_rules[0].m_payloads.emplace_back(5, 5);

    m_fireworkwork_rules[1].SetRuleParameters(2, 0.5f, 1.0f, {-5.0f, 10.0f, -5.0f}, {5.0f, 20.0f, 5.0f}, 0.8f);
    m_fireworkwork_rules[1].m_payloads.emplace_back(4, 2);

    m_fireworkwork_rules[2].SetRuleParameters(3, 0.5f, 1.5f, {-5.0f, -5.0f, -5.0f}, {5.0f, 5.0f, 5.0f}, 0.1f);

    m_fireworkwork_rules[3].SetRuleParameters(4, 0.25f, 0.5f, {-20.0f, 5.0f, -5.0f}, {20.0f, 5.0f, 5.0f}, 0.2f);

    m_fireworkwork_rules[4].SetRuleParameters(5, 0.5f, 1.0f, {-20.0f, 2.0f, -5.0f}, {20.0f, 18.0f, 5.0f}, 0.01f);
    m_fireworkwork_rules[4].m_payloads.emplace_back(3, 5);

    m_fireworkwork_rules[5].SetRuleParameters(6, // type
                                              3,
                                              5,               // age range
                                              vec3(-5, 5, -5), // min velocity
                                              vec3(5, 10, 5),  // max velocity
                                              0.95f            // damping
    );

    m_fireworkwork_rules[6].SetRuleParameters(7, // type
                                              4,
                                              5,                // age range
                                              vec3(-5, 50, -5), // min velocity
                                              vec3(5, 60, 5),   // max velocity
                                              0.01f             // damping
    );
    m_fireworkwork_rules[6].m_payloads.emplace_back(8, 10);

    m_fireworkwork_rules[7].SetRuleParameters(8, // type
                                              0.25f,
                                              0.5f,             // age range
                                              vec3(-1, -1, -1), // min velocity
                                              vec3(1, 1, 1),    // max velocity
                                              0.01f             // damping
    );

    m_fireworkwork_rules[8].SetRuleParameters(9, // type
                                              3,
                                              5,                 // age range
                                              vec3(-15, 10, -5), // min velocity
                                              vec3(15, 15, 5),   // max velocity
                                              0.95f              // damping
    );
}

} // namespace sputnik::demos