#include "basic_particles.h"

#include <graphics/glcore/uniform.h>
#include <graphics/glcore/renderer.h>
#include <matrix4.h>
#include <vector3.h>
#include <camera_transforms.h>
#include <imgui.h>
#include <editor/editor_camera.h>
#include <graphics/api/camera.h>

#include <glad/glad.h>

#include <vector.hpp>
#include <matrix.hpp>

namespace sputnik::demos
{

using namespace ramanujan;
using namespace experimental;
using namespace sputnik::graphics::glcore;

BasicParticlesDemoLayer::BasicParticlesDemoLayer(const std::string& name) : core::Layer(name) {}

BasicParticlesDemoLayer::~BasicParticlesDemoLayer() {}

void BasicParticlesDemoLayer::OnAttach()
{
    experimental::vec3 test_position{1.0f, 2.0f, 3.0f};
    auto               norm_res = test_position.normalized();

    experimental::vec3 test_position_3{4.0f, 5.0f, 6.0f};

    test_position += test_position_3;
    test_position -= test_position_3;
    test_position *= test_position_3;
    test_position /= test_position_3;

    test_position += 2.0f;
    test_position -= 2.0f;
    test_position *= 2.0f;
    test_position /= 2.0f;

    experimental::vec3 add_result = test_position + test_position_3;
    experimental::vec3 sub_result = test_position - test_position_3;
    experimental::vec3 mul_result = test_position * test_position_3;
    experimental::vec3 div_result = test_position / test_position_3;

    experimental::vec3 add_result_1 = test_position + 3.0f;
    experimental::vec3 sub_result_1 = test_position - 3.0f;
    experimental::vec3 mul_result_1 = test_position * 3.0f;
    experimental::vec3 div_result_1 = test_position / 3.0f;

    auto slerp_res = slerp(add_result, add_result_1, real(0.4));
    auto lerp_res  = lerp(add_result, add_result_1, real(0.4));
    auto nlerp_res = nlerp(add_result, add_result_1, real(0.4));
    auto bool_0    = add_result.isOrthogonal(add_result_1);
    auto bool_1    = add_result.isParallel(add_result_1);
    auto bool_2    = add_result.isZero();

    mat4 sample_matrix_4{real(0),
                         real(1),
                         real(2),
                         real(3),
                         real(4),
                         real(5),
                         real(6),
                         real(7),
                         real(8),
                         real(9),
                         real(10),
                         real(11),
                         real(12),
                         real(13),
                         real(14),
                         real(15)};

    // std::cout << sample_matrix_4 << std::endl;

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

    m_static_shader =
        std::make_shared<Shader>("../../../../data/shaders/simple.vert", "../../../../data/shaders/simple.frag");
    m_sky_shader = std::make_shared<Shader>("../../../../data/shaders/sky-rendering/sky.vert",
                                            "../../../../data/shaders/sky-rendering/sky.frag");
    m_simple_lighting_shader =
        std::make_shared<Shader>("../../../../data/shaders/simple-lighting/simple-lighting.vert",
                                 "../../../../data/shaders/simple-lighting/simple-lighting.frag");

    m_static_mesh_texture = std::make_shared<Texture>("../../../../data/assets/dq.png");

    m_box    = Model::LoadModel("../../../../data/assets/box/Box.gltf");
    m_sphere = Model::LoadModel("../../../../data/assets/sphere.gltf");

    m_platform_material.ambient   = {0.05f, 0.0f, 0.0f};
    m_platform_material.diffuse   = {0.5f, 0.4f, 0.4f};
    m_platform_material.specular  = {0.7f, 0.4f, 0.4f};
    m_platform_material.shininess = 0.078125f;

    const auto& editor_camera = sputnik::graphics::api::EditorCamera::GetInstance();
    editor_camera->SetPosition({0.0f, 0.0f, 10.0f});

    const auto& camera = sputnik::graphics::api::Camera::GetInstance();
    camera->SetPosition({0.0f, 0.0f, 10.0f});

    // setting up particles
    {
        sputnik::physics::Particle particle;
        particle.setPosition(0.0f, 4.0f, 0.0f);
        particle.setMass(2.0f); // 2.0Kg
        particle.setAcceleration({0.0f, -1.0f, 0.0f});
        particle.setDamping(0.09f);
        m_particles.emplace_back(particle);

        m_particle_material.ambient   = {0.24725f, 0.1995f, 0.0745f};
        m_particle_material.diffuse   = {0.75164f, 0.60648f, 0.22648f};
        m_particle_material.specular  = {0.628281f, 0.555802f, 0.366065f};
        m_particle_material.shininess = 0.4f;
    }

    // glPointSize(5.0f);
    // glLineWidth(1.5f);
}

// Todo: OnDetach() must be called at system cleanup before shutdown
void BasicParticlesDemoLayer::OnDetach() {}

void BasicParticlesDemoLayer::OnUpdate(const core::TimeStep& time_step)
{
    if(time_step.GetSeconds() <= Constants::EPSILON)
    {
        return;
    }

    Matrix4 model_matrix;
    Matrix4 normal_matrix;

    const auto& editor_camera = sputnik::graphics::api::EditorCamera::GetInstance();
    Matrix4     projection    = editor_camera->GetCameraPerspective();
    Matrix4     view          = editor_camera->GetCameraView();

    const auto& camera          = sputnik::graphics::api::Camera::GetInstance();
    const auto& p               = camera->GetCameraPerspective();
    const auto& v               = camera->GetCameraView();
    const auto& camera_position = camera->GetCameraPosition();
    for(unsigned int i = 0; i < 16; ++i)
    {
        projection.v[i] = p.m[i];
        view.v[i]       = v.m[i];
    }

    glEnable(GL_DEPTH_TEST);
    // render light source
    {
        m_static_shader->Bind();
        Uniform<Matrix4>::Set(m_static_shader->GetUniform("view"), view);
        Uniform<Matrix4>::Set(m_static_shader->GetUniform("projection"), projection);
        Uniform<vec3>::Set(m_static_shader->GetUniform("light"), m_light.position);

        m_static_mesh_texture->Set(m_static_shader->GetUniform("diffuse"), 0);

        Transform transform({m_light.position.x, m_light.position.y, m_light.position.z}, {}, {0.20f, 0.20f, 0.20f});
        model_matrix = ToMatrix4(transform);
        Uniform<Matrix4>::Set(m_static_shader->GetUniform("model"), model_matrix);

        m_sphere->Draw(m_static_shader, true, false, false);

        m_static_mesh_texture->Unset(0);
        m_static_shader->Unbind();
    }

    // phong lighting
    {
        m_simple_lighting_shader->Bind();

        Uniform<Matrix4>::Set(m_simple_lighting_shader->GetUniform("view"), view);
        Uniform<Matrix4>::Set(m_simple_lighting_shader->GetUniform("projection"), projection);

        // light
        Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("light.position"), m_light.position);
        Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("light.ambient"), m_light.ambient);
        Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("light.diffuse"), m_light.diffuse);
        Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("light.specular"), m_light.specular);

        // camera
        Uniform<vec3>::Set(m_simple_lighting_shader->GetUniform("eye_position"), camera_position);

        // render the platform
        {

            Transform transform({0.0f, -10.0f, 0.0f}, {}, {25.0f, 1.0f, 25.0f});
            model_matrix  = ToMatrix4(transform);
            normal_matrix = Transposed(Inverse(model_matrix));
            Uniform<Matrix4>::Set(m_simple_lighting_shader->GetUniform("model"), model_matrix);
            Uniform<Matrix4>::Set(m_simple_lighting_shader->GetUniform("normal_matrix"), normal_matrix);

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
                Transform   transform({pos.x, pos.y, pos.z}, {}, {0.15f});
                model_matrix  = ToMatrix4(transform);
                normal_matrix = Transposed(Inverse(model_matrix));
                Uniform<Matrix4>::Set(m_simple_lighting_shader->GetUniform("model"), model_matrix);
                Uniform<Matrix4>::Set(m_simple_lighting_shader->GetUniform("normal_matrix"), normal_matrix);

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

        m_simple_lighting_shader->Unbind();
    }
    glDisable(GL_DEPTH_TEST);

    // debug draw
    {
        // glDisable(GL_DEPTH_TEST);
        //  render any debug visuals
        // glEnable(GL_DEPTH_TEST);
    }

    // Render sky
    {
        // very important
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glDisable(GL_CULL_FACE);

        Matrix4 cubemap_view{};
        for(unsigned int i = 0; i < 16; ++i)
        {
            cubemap_view.v[i] = view.v[i];
        }
        cubemap_view.v[3]  = 0;
        cubemap_view.v[7]  = 0;
        cubemap_view.v[11] = 0;
        cubemap_view.v[15] = 1;
        cubemap_view.v[12] = 0;
        cubemap_view.v[13] = 0;
        cubemap_view.v[14] = 0;

        // mat4 proj{};
        // for(unsigned i = 0; i < 16; ++i)
        //{
        //     proj.m[i] = projection.v[i];
        // }

        Matrix4 cubemap_view_projection = projection * cubemap_view;
        // mat4 inv_cubemap_view_projection = cubemap_view_projection.inverse();
        Matrix4 inv_cubemap_view_projection = Inverse(cubemap_view_projection); // Todo:: This is buggy with mat4

        mPreethamSkyModel.SetDirection(mDirection);
        mPreethamSkyModel.Update();
        m_sky_shader->Bind();
        // Uniform<mat4>::Set(m_sky_shader->GetUniform("inv_view_projection"),
        //                                               inv_cubemap_view_projection);
        Uniform<Matrix4>::Set(m_sky_shader->GetUniform("inv_view_projection"), inv_cubemap_view_projection);
        mPreethamSkyModel.SetRenderUniforms(m_sky_shader);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // very important
        glDepthFunc(GL_LESS);
        glDisable(GL_DEPTH_TEST);
    }
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

    ImGui::Begin("Sky Scattering");

    ImGui::SliderFloat("Exposure", &m_exposure, 0, 10.0f);
    ImGui::SliderAngle("Sun Angle", &m_sun_angle, 0.0f, -180.0f);

    float turbidity = mPreethamSkyModel.GetTurbidity();
    ImGui::SliderFloat("Turbidity", &turbidity, 2.0f, 30.0f);
    mPreethamSkyModel.SetTurbidity(turbidity);

    mDirection = vec3(0.0f, sin(m_sun_angle), cos(m_sun_angle)).normalized();

    ImGui::Text("Sun Direction = [ %f, %f, %f ]", mDirection.x, mDirection.y, mDirection.z);

    ImGui::End();
}

} // namespace sputnik::demos