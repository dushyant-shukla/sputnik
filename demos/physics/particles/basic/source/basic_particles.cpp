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

namespace sputnik::demos
{

BasicParticlesDemoLayer::BasicParticlesDemoLayer(const std::string& name) : core::Layer(name) {}

BasicParticlesDemoLayer::~BasicParticlesDemoLayer() {}

void BasicParticlesDemoLayer::OnAttach()
{
    ramanujan::experimental::vec3 test_position{1.0f, 2.0f, 3.0f};
    test_position.x = 50.0f;
    test_position.y = 40.0f;
    test_position.z = 80.0f;

    m_static_shader = std::make_shared<sputnik::graphics::glcore::Shader>("../../../../data/shaders/simple.vert",
                                                                          "../../../../data/shaders/simple.frag");

    cgltf_data* gltf_static_mesh = sputnik::gltf::GltfLoader::LoadFile("../../../../data/assets/sphere.gltf");
    m_static_mesh_texture = std::make_shared<sputnik::graphics::glcore::Texture>("../../../../data/assets/dq.png");
    m_static_meshes       = sputnik::gltf::GltfLoader::LoadMeshes(gltf_static_mesh);

     const auto& editor_camera = sputnik::graphics::api::EditorCamera::GetInstance();
     editor_camera->SetPosition({0.0f, 0.0f, 10.0f});

    glPointSize(5.0f);
    glLineWidth(1.5f);
}

// Todo: OnDetach() must be called at system cleanup before shutdown
void BasicParticlesDemoLayer::OnDetach() {}

void BasicParticlesDemoLayer::OnUpdate(const core::TimeStep& time_step)
{
    ramanujan::Matrix4 model;
    ramanujan::Matrix4 projection = ramanujan::Perspective(60.0f, 1600.0f / 900.0f, 0.01f, 1000.0f);
    ramanujan::Matrix4 view = ramanujan::LookAt({0.0f, 5.0f, 7.0f}, {0.0f, 3.0f, 0.0f}, {0.0f, 1.0f, 0.0f}); // woman

    const auto& editor_camera = sputnik::graphics::api::EditorCamera::GetInstance();
    projection                = editor_camera->GetCameraPerspective();
    view                      = editor_camera->GetCameraView();
    ramanujan::Matrix4 mvp    = projection * view * model;

    // render static meshes
    {
        m_static_shader->Bind();
        ramanujan::Transform transform({0.0f, 0.0f, 0.0f}, {}, {0.25f});
        model = ramanujan::ToMatrix4(transform);
        sputnik::graphics::glcore::Uniform<ramanujan::Matrix4>::Set(m_static_shader->GetUniform("model"), model);
        sputnik::graphics::glcore::Uniform<ramanujan::Matrix4>::Set(m_static_shader->GetUniform("view"), view);
        sputnik::graphics::glcore::Uniform<ramanujan::Matrix4>::Set(m_static_shader->GetUniform("projection"),
                                                                    projection);
        sputnik::graphics::glcore::Uniform<ramanujan::Vector3>::Set(m_static_shader->GetUniform("light"),
                                                                    {0.0f, 5.0f, -10.0f});
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