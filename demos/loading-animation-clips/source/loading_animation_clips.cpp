#include "loading_animation_clips.h"

#include <graphics/api/renderer.h>
#include <graphics/glcore/gltf_loader.h>
#include <matrix4.h>
#include <vector3.h>
#include <camera_transforms.h>

namespace sputnik
{

sputnik::main::Application* sputnik::main::CreateApplication()
{
    return new demos::LoadingAnimationClips();
}

namespace demos
{

LoadingAnimationClips::LoadingAnimationClips()
    : sputnik::main::Application::Application()
    , m_show_current_pose(true)
    , m_show_rest_pose(true)
    , m_current_clip(0)
    , m_playback_time(0.0f)
    , m_current_pose_visual(nullptr)
    , m_rest_pose_visual(nullptr)
{
}

LoadingAnimationClips::~LoadingAnimationClips() {}

void LoadingAnimationClips::Initialize()
{
    // cgltf_data* gltf = sputnik::gltf::GltfLoader::LoadFile("../../data/assets/Woman.gltf");
    cgltf_data* gltf = sputnik::gltf::GltfLoader::LoadFile("../../data/assets/spiderman/scene.gltf");
    m_rest_pose      = sputnik::gltf::GltfLoader::LoadRestPose(gltf);
    sputnik::gltf::GltfLoader::LoadAnimationClips(gltf, m_clips);
    sputnik::gltf::GltfLoader::FreeFile(gltf);

    m_rest_pose_visual = new sputnik::glcore::DebugDraw();
    m_rest_pose_visual->FromPose(m_rest_pose);
    m_rest_pose_visual->UpdateOpenGLBuffers();

    m_current_pose = m_rest_pose;

    m_current_pose_visual = new sputnik::glcore::DebugDraw();
    m_current_pose_visual->FromPose(m_current_pose);
    m_current_pose_visual->UpdateOpenGLBuffers();
}

void LoadingAnimationClips::Update(float delta_time)
{
    m_playback_time = m_clips[m_current_clip].Sample(m_current_pose, m_playback_time + delta_time);
    m_current_pose_visual->FromPose(m_current_pose);
}

void LoadingAnimationClips::Render(float aspect_ratio)
{
    ramanujan::Matrix4 projection = ramanujan::Perspective(60.0f, aspect_ratio, 0.01f, 1000.0f);
    ramanujan::Matrix4 view       = ramanujan::LookAt({0.0f, 4.0f, -500.0f}, {0.0f, 4.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
    ramanujan::Matrix4 mvp        = projection * view;

    if(m_show_rest_pose)
    {
        m_rest_pose_visual->Draw(sputnik::glcore::DebugDrawMode::Lines, {1.0f, 0.0f, 0.0f}, mvp);
    }
    if(m_show_current_pose)
    {
        m_current_pose_visual->UpdateOpenGLBuffers();
        m_current_pose_visual->Draw(sputnik::glcore::DebugDrawMode::Lines, {0.0f, 0.0f, 1.0f}, mvp);
    }
}

void LoadingAnimationClips::Shutdown()
{
    delete m_rest_pose_visual;
    delete m_current_pose_visual;
    m_clips.clear();
}

} // namespace demos

} // namespace sputnik