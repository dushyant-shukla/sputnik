#include "basic_particles.h"

#include <graphics/glcore/uniform.h>
#include <graphics/glcore/renderer.h>
#include <graphics/glcore/gltf_loader.h>
#include <matrix4.h>
#include <vector3.h>
#include <camera_transforms.h>
#include <imgui.h>
#include <graphics/core/animation/rearrange_bones.h>
#include <editor/editor_camera.h>

#include <vector.hpp>
#include <matrix.hpp>

namespace sputnik::demos
{

using namespace ramanujan;
using namespace ramanujan::experimental;

BasicParticlesDemoLayer::BasicParticlesDemoLayer(const std::string& name) : core::Layer(name) {}

BasicParticlesDemoLayer::~BasicParticlesDemoLayer() {}

void BasicParticlesDemoLayer::OnAttach()
{
    ramanujan::experimental::vec3 test_position{1.0f, 2.0f, 3.0f};
    auto                          norm_res = test_position.normalized();

    ramanujan::experimental::vec3 test_position_3{4.0f, 5.0f, 6.0f};

    test_position += test_position_3;
    test_position -= test_position_3;
    test_position *= test_position_3;
    test_position /= test_position_3;

    test_position += 2.0f;
    test_position -= 2.0f;
    test_position *= 2.0f;
    test_position /= 2.0f;

    ramanujan::experimental::vec3 add_result = test_position + test_position_3;
    ramanujan::experimental::vec3 sub_result = test_position - test_position_3;
    ramanujan::experimental::vec3 mul_result = test_position * test_position_3;
    ramanujan::experimental::vec3 div_result = test_position / test_position_3;

    ramanujan::experimental::vec3 add_result_1 = test_position + 3.0f;
    ramanujan::experimental::vec3 sub_result_1 = test_position - 3.0f;
    ramanujan::experimental::vec3 mul_result_1 = test_position * 3.0f;
    ramanujan::experimental::vec3 div_result_1 = test_position / 3.0f;

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

    std::cout << sample_matrix_4 << std::endl;

    // ramanujan::experimental::vec3 lerp_result  = lerp(test_position, test_position_3, 0.5f);
    // ramanujan::experimental::vec3 slerp_result = slerp(test_position, test_position_3, 0.5f);

    // ramanujan::experimental::vec3 test_position_4 = test_position.cross(test_position_3);

    // std::cout << "Before updating: " << test_position << std::endl;

    // test_position.x = 50.0f;
    // test_position.y = 40.0f;
    // test_position.z = 80.0f;
    // std::cout << "After updating: " << test_position << std::endl;

    // test_position.z = 160.0f;
    // std::cout << "After updating: " << test_position << std::endl;

    // ramanujan::experimental::vec3 test_position_1{25.0f};
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

    ////ramanujan::experimental::color4 test_color{50.0f};
    ////std::cout << "Before updating: " << test_color << std::endl;

    ////test_color.r = 100.0f;
    ////std::cout << "After updating: " << test_color << std::endl;

    // ramanujan::experimental::vec2 test_uv_1{0.0f, 5.0f};
    // ramanujan::experimental::vec2 test_uv_2{0.0f, 1.0f};
    // test_uv_1 = test_uv_2;

    m_static_shader = std::make_shared<sputnik::graphics::glcore::Shader>("../../../../data/shaders/simple.vert",
                                                                          "../../../../data/shaders/simple.frag");

    cgltf_data* gltf_static_mesh = sputnik::gltf::GltfLoader::LoadFile("../../../../data/assets/sphere.gltf");
    m_static_mesh_texture = std::make_shared<sputnik::graphics::glcore::Texture>("../../../../data/assets/dq.png");
    m_static_meshes       = sputnik::gltf::GltfLoader::LoadMeshes(gltf_static_mesh);

    const auto& editor_camera = sputnik::graphics::api::EditorCamera::GetInstance();
    editor_camera->SetPosition({0.0f, 0.0f, 10.0f});

    // sputnik::physics::Particle particle;
    m_particle.setPosition(0.0f, 4.0f, 0.0f);
    m_particle.setMass(2.0f); // 2.0Kg
    m_particle.setAcceleration({0.0f, -1.0f, 0.0f});
    m_particle.setDamping(0.09f);
    m_particles.emplace_back(m_particle);

    glPointSize(5.0f);
    glLineWidth(1.5f);
}

// Todo: OnDetach() must be called at system cleanup before shutdown
void BasicParticlesDemoLayer::OnDetach() {}

void BasicParticlesDemoLayer::OnUpdate(const core::TimeStep& time_step)
{
    if(time_step.GetSeconds() <= ramanujan::Constants::EPSILON)
    {
        return;
    }

    ramanujan::Matrix4 model;
    ramanujan::Matrix4 projection = ramanujan::Perspective(60.0f, 1600.0f / 900.0f, 0.01f, 1000.0f);
    ramanujan::Matrix4 view = ramanujan::LookAt({0.0f, 5.0f, 7.0f}, {0.0f, 3.0f, 0.0f}, {0.0f, 1.0f, 0.0f}); // woman

    const auto& editor_camera = sputnik::graphics::api::EditorCamera::GetInstance();
    projection                = editor_camera->GetCameraPerspective();
    view                      = editor_camera->GetCameraView();
    ramanujan::Matrix4 mvp    = projection * view * model;

    // render static meshes (works)
    // m_particle.integrate(time_step.GetSeconds());
    //{
    //    m_static_shader->Bind();
    //    // ramanujan::Transform transform({0.0f, 0.0f, 0.0f}, {}, {0.25f});
    //    const auto& pos = m_particle.getPosition();
    //    std::cout << pos << std::endl;
    //    ramanujan::Transform transform({pos.x, pos.y, pos.z}, {}, {0.15f});
    //    // ramanujan::Transform transform({pos[0], pos[1], pos[2]}, {}, {0.15f});
    //    model = ramanujan::ToMatrix4(transform);
    //    sputnik::graphics::glcore::Uniform<ramanujan::Matrix4>::Set(m_static_shader->GetUniform("model"), model);
    //    sputnik::graphics::glcore::Uniform<ramanujan::Matrix4>::Set(m_static_shader->GetUniform("view"), view);
    //    sputnik::graphics::glcore::Uniform<ramanujan::Matrix4>::Set(m_static_shader->GetUniform("projection"),
    //                                                                projection);
    //    sputnik::graphics::glcore::Uniform<ramanujan::Vector3>::Set(m_static_shader->GetUniform("light"),
    //                                                                {0.0f, 5.0f, 10.0f});
    //    m_static_mesh_texture->Set(m_static_shader->GetUniform("diffuse"), 0);
    //    for(unsigned int i = 0, size = (unsigned int)m_static_meshes.size(); i < size; ++i)
    //    {
    //        m_static_meshes[i].Bind(m_static_shader->GetAttribute("position"),
    //                                m_static_shader->GetAttribute("normal"),
    //                                m_static_shader->GetAttribute("uv"),
    //                                -1,
    //                                -1);
    //        m_static_meshes[i].Draw();
    //        m_static_meshes[i].Unbind(m_static_shader->GetAttribute("position"),
    //                                  m_static_shader->GetAttribute("normal"),
    //                                  m_static_shader->GetAttribute("uv"),
    //                                  -1,
    //                                  -1);
    //    }
    //    m_static_mesh_texture->Unset(0);
    //    m_static_shader->Unbind();
    //}

    for(unsigned int i = 0; i < m_particles.size(); ++i)
    {
        m_particles[i].integrate(time_step.GetSeconds());

        // render static meshes
        {
            m_static_shader->Bind();
            // ramanujan::Transform transform({0.0f, 0.0f, 0.0f}, {}, {0.25f});
            const auto& pos = m_particles[i].getPosition();
            // std::cout << pos << std::endl;
            ramanujan::Transform transform({pos[0], pos[1], pos[2]}, {}, {0.15f});
            // ramanujan::Transform transform({pos.x, pos.y, pos.z}, {}, {0.15f});
            model = ramanujan::ToMatrix4(transform);
            sputnik::graphics::glcore::Uniform<ramanujan::Matrix4>::Set(m_static_shader->GetUniform("model"), model);
            sputnik::graphics::glcore::Uniform<ramanujan::Matrix4>::Set(m_static_shader->GetUniform("view"), view);
            sputnik::graphics::glcore::Uniform<ramanujan::Matrix4>::Set(m_static_shader->GetUniform("projection"),
                                                                        projection);
            sputnik::graphics::glcore::Uniform<ramanujan::Vector3>::Set(m_static_shader->GetUniform("light"),
                                                                        {0.0f, 5.0f, 10.0f});
            m_static_mesh_texture->Set(m_static_shader->GetUniform("diffuse"), 0);
            for(unsigned int i = 0, size = (unsigned int)m_static_meshes.size(); i < size; ++i)
            {
                m_static_meshes[i].Bind(m_static_shader->GetAttribute("position"),
                                        m_static_shader->GetAttribute("normal"),
                                        m_static_shader->GetAttribute("uv"),
                                        -1,
                                        -1);
                m_static_meshes[i].Draw();
                m_static_meshes[i].Unbind(m_static_shader->GetAttribute("position"),
                                          m_static_shader->GetAttribute("normal"),
                                          m_static_shader->GetAttribute("uv"),
                                          -1,
                                          -1);
            }
            m_static_mesh_texture->Unset(0);
            m_static_shader->Unbind();
        }
    }

    // render static meshes
    //{
    //    m_static_shader->Bind();
    //    ramanujan::Transform transform({0.0f, 0.0f, 0.0f}, {}, {0.25f});
    //    model = ramanujan::ToMatrix4(transform);
    //    sputnik::graphics::glcore::Uniform<ramanujan::Matrix4>::Set(m_static_shader->GetUniform("model"), model);
    //    sputnik::graphics::glcore::Uniform<ramanujan::Matrix4>::Set(m_static_shader->GetUniform("view"), view);
    //    sputnik::graphics::glcore::Uniform<ramanujan::Matrix4>::Set(m_static_shader->GetUniform("projection"),
    //                                                                projection);
    //    sputnik::graphics::glcore::Uniform<ramanujan::Vector3>::Set(m_static_shader->GetUniform("light"),
    //                                                                {0.0f, 5.0f, 10.0f});
    //    m_static_mesh_texture->Set(m_static_shader->GetUniform("diffuse"), 0);
    //    for(unsigned int i = 0, size = (unsigned int)m_static_meshes.size(); i < size; ++i)
    //    {
    //        m_static_meshes[i].Bind(m_static_shader->GetAttribute("position"),
    //                                m_static_shader->GetAttribute("normal"),
    //                                m_static_shader->GetAttribute("uv"),
    //                                -1,
    //                                -1);
    //        m_static_meshes[i].Draw();
    //        m_static_meshes[i].Unbind(m_static_shader->GetAttribute("position"),
    //                                  m_static_shader->GetAttribute("normal"),
    //                                  m_static_shader->GetAttribute("uv"),
    //                                  -1,
    //                                  -1);
    //    }
    //    m_static_mesh_texture->Unset(0);
    //    m_static_shader->Unbind();
    //}

    // end render static meshes

    glDisable(GL_DEPTH_TEST);
    // render any debug visuals
    glEnable(GL_DEPTH_TEST);
}

void BasicParticlesDemoLayer::OnEvent() {}

void BasicParticlesDemoLayer::OnUpdateUI(const core::TimeStep& time_step)
{
    if(ImGui::Begin("Basic Particles"))
    {
    }
    ImGui::End();
}

} // namespace sputnik::demos