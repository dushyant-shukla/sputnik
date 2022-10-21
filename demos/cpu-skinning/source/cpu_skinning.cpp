#include "cpu_skinning.h"

#include <graphics/glcore/uniform.h>
#include <graphics/api/renderer.h>
#include <graphics/glcore/gltf_loader.h>
#include <matrix4.h>
#include <vector3.h>
#include <camera_transforms.h>

namespace sputnik
{

sputnik::main::Application* sputnik::main::CreateApplication()
{
    return new demos::CpuSkinning();
}

namespace demos
{

CpuSkinning::CpuSkinning()
    : sputnik::main::Application::Application()
    , m_show_current_pose(false)
    , m_show_rest_pose(false)
    , m_show_bind_pose(false)
    , m_current_clip(0)
    , m_playback_time(0.0f)
    , m_current_pose_visual(nullptr)
    , m_rest_pose_visual(nullptr)
    , m_bind_pose_visual(nullptr)
    , m_skinning_type(sputnik::api::animation::SkinningType::CPU)
{
}

CpuSkinning::~CpuSkinning() {}

void CpuSkinning::Initialize()
{
    m_static_shader =
        std::make_shared<sputnik::glcore::Shader>("../../data/shaders/simple.vert", "../../data/shaders/simple.frag");
    // m_skinning_shader =
    //     std::make_shared<sputnik::glcore::Shader>("../../data/shaders/skinned.vert",
    //     "../../data/shaders/simple.frag");

    cgltf_data* gltf  = sputnik::gltf::GltfLoader::LoadFile("../../data/assets/Woman.gltf");
    m_diffuse_texture = std::make_shared<sputnik::glcore::Texture>("../../data/assets/Woman.png");

     //cgltf_data* gltf  = sputnik::gltf::GltfLoader::LoadFile("../../data/assets/nathan/scene.gltf");
     //m_diffuse_texture = std::make_shared<sputnik::glcore::Texture>(
     //    "../../data/assets/nathan/textures/rp_nathan_animated_003_mat_baseColor.jpeg");

     //cgltf_data* gltf = sputnik::gltf::GltfLoader::LoadFile("../../data/assets/spiderman/scene.gltf");
     //m_diffuse_texture = std::make_shared<sputnik::glcore::Texture>(
     //   "../../data/assets/spiderman/textures/RootNode_baseColor.jpeg");

    m_skeleton = sputnik::gltf::GltfLoader::LoadSkeleton(gltf);
    m_meshes   = sputnik::gltf::GltfLoader::LoadMeshes(gltf);
    sputnik::gltf::GltfLoader::LoadAnimationClips(gltf, m_clips);
    sputnik::gltf::GltfLoader::FreeFile(gltf);

    m_rest_pose_visual = std::make_shared<sputnik::glcore::DebugDraw>();
    m_rest_pose_visual->FromPose(m_skeleton.GetRestPose());
    m_rest_pose_visual->UpdateOpenGLBuffers();

    m_bind_pose_visual = std::make_shared<sputnik::glcore::DebugDraw>();
    m_bind_pose_visual->FromPose(m_skeleton.GetBindPose());
    m_bind_pose_visual->UpdateOpenGLBuffers();

    m_current_pose = m_skeleton.GetRestPose();

    m_current_pose_visual = std::make_shared<sputnik::glcore::DebugDraw>();
    m_current_pose_visual->FromPose(m_current_pose);
    m_current_pose_visual->UpdateOpenGLBuffers();
}

void CpuSkinning::Update(float delta_time)
{
    m_playback_time = m_clips[m_current_clip].Sample(m_current_pose, m_playback_time + delta_time);
    m_current_pose_visual->FromPose(m_current_pose);

    switch(m_skinning_type)
    {
    case sputnik::api::animation::SkinningType::NONE:
        break;
    case sputnik::api::animation::SkinningType::CPU:
    {
        for(unsigned int i = 0, size = (unsigned int)m_meshes.size(); i < size; ++i)
        {
            m_meshes[i].CpuSkin(m_skeleton, m_current_pose);
        }
        break;
    }
    case sputnik::api::animation::SkinningType::GPU:
    {
        m_current_pose.GetMatrixPalette(m_pose_palette);
        break;
    }
    default:
        break;
    }
}

void CpuSkinning::Render(float aspect_ratio)
{
    //ramanujan::Matrix4 model      = ramanujan::ToMatrix4(ramanujan::AngleAxis(90.0f, {0.0f, 1.0f, 0.0f}));
    ramanujan::Matrix4 model;
    ramanujan::Matrix4 projection = ramanujan::Perspective(60.0f, aspect_ratio, 0.01f, 1000.0f);
    ramanujan::Matrix4 view = ramanujan::LookAt({0.0f, 5.0f, 7.0f}, {0.0f, 3.0f, 0.0f}, {0.0f, 1.0f, 0.0f}); // woman
    //ramanujan::Matrix4 view = ramanujan::LookAt({0.0f, 0.0f, 10.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}); // woman
    //ramanujan::Matrix4 view = ramanujan::LookAt({0.0f, 20.0f, -350.0f}, {0.0f, 20.0f, 0.0f}, {0.0f, 1.0f, 0.0f}); // spiderman + nathan

    ramanujan::Matrix4 mvp = projection * view * model;

    std::shared_ptr<sputnik::glcore::Shader> active_shader = m_static_shader;
    if(m_skinning_type == sputnik::api::animation::SkinningType::GPU)
    {
        active_shader = m_skinning_shader;
        sputnik::glcore::Uniform<ramanujan::Matrix4>::Set(active_shader->GetUniform("pose"), m_pose_palette);
        sputnik::glcore::Uniform<ramanujan::Matrix4>::Set(active_shader->GetUniform("inv_bind_pose"),
                                                          m_skeleton.GetInverseBindPose());
    }

    active_shader->Bind();
    sputnik::glcore::Uniform<ramanujan::Matrix4>::Set(active_shader->GetUniform("model"), model);
    sputnik::glcore::Uniform<ramanujan::Matrix4>::Set(active_shader->GetUniform("view"), view);
    sputnik::glcore::Uniform<ramanujan::Matrix4>::Set(active_shader->GetUniform("projection"), projection);
    sputnik::glcore::Uniform<ramanujan::Vector3>::Set(active_shader->GetUniform("light"), {1.0f, 1.0f, 1.0f});

    m_diffuse_texture->Set(active_shader->GetUniform("diffuse"), 0);
    for(unsigned int i = 0, size = (unsigned int)m_meshes.size(); i < size; ++i)
    {
        int weights    = -1;
        int influences = -1;
        if(m_skinning_type == sputnik::api::animation::SkinningType::GPU)
        {
            weights    = active_shader->GetAttribute("weights");
            influences = active_shader->GetAttribute("joints");
        }

        m_meshes[i].Bind(active_shader->GetAttribute("position"),
                         active_shader->GetAttribute("normal"),
                         active_shader->GetAttribute("uv"),
                         weights,
                         influences);
        m_meshes[i].Draw();
        m_meshes[i].Unbind(active_shader->GetAttribute("position"),
                           active_shader->GetAttribute("normal"),
                           active_shader->GetAttribute("uv"),
                           weights,
                           influences);
    }
    m_diffuse_texture->Unset(0);
    active_shader->Unbind();

    glDisable(GL_DEPTH_TEST);
    if(m_show_rest_pose)
    {
        m_rest_pose_visual->Draw(sputnik::glcore::DebugDrawMode::Lines, {1.0f, 0.0f, 0.0f}, mvp);
    }
    if(m_show_bind_pose)
    {
        m_bind_pose_visual->Draw(sputnik::glcore::DebugDrawMode::Lines, {0.0f, 1.0f, 0.0f}, mvp);
    }
    if(m_show_current_pose)
    {
        m_current_pose_visual->UpdateOpenGLBuffers();
        m_current_pose_visual->Draw(sputnik::glcore::DebugDrawMode::Lines, {0.0f, 0.0f, 1.0f}, mvp);
    }
    glEnable(GL_DEPTH_TEST);
}

void CpuSkinning::Shutdown()
{
    m_clips.clear();
}

} // namespace demos

} // namespace sputnik