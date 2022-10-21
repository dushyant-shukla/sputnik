#pragma once

#include <platform/windows/windows_main.h>
#include <main/application.h>
#include <graphics/glcore/shader.h>
#include <graphics/api/animation/animation_clip.h>
#include <graphics/api/animation/pose.h>
#include <graphics/glcore/debug_draw.h>
#include <graphics/api/animation/skeleton.h>
#include <graphics/glcore/texture.h>
#include <graphics/api/animation/skinning_type.h>
#include <graphics/api/mesh.h>

#include <memory>
#include <vector>

namespace sputnik::demos
{

class CpuSkinning : public sputnik::main::Application
{

public:
    CpuSkinning();
    ~CpuSkinning();

    virtual void Initialize() override;
    virtual void Update(float delta_time) override;
    virtual void Render(float aspect_ratio) override;
    virtual void Shutdown() override;

private:
    std::shared_ptr<sputnik::glcore::Texture> m_diffuse_texture;

    sputnik::api::animation::SkinningType    m_skinning_type;
    std::shared_ptr<sputnik::glcore::Shader> m_static_shader;
    std::shared_ptr<sputnik::glcore::Shader> m_skinning_shader;

    std::shared_ptr<sputnik::glcore::DebugDraw> m_rest_pose_visual;
    std::shared_ptr<sputnik::glcore::DebugDraw> m_bind_pose_visual;
    std::shared_ptr<sputnik::glcore::DebugDraw> m_current_pose_visual;

    std::vector<sputnik::api::Mesh> m_meshes;
    std::vector<ramanujan::Matrix4> m_pose_palette;

    sputnik::api::animation::Skeleton m_skeleton;
    sputnik::api::animation::Pose     m_current_pose;

    bool m_show_rest_pose;
    bool m_show_current_pose;
    bool m_show_bind_pose;

    std::vector<sputnik::api::animation::AnimationClip> m_clips;
    size_t                                              m_current_clip;
    float                                               m_playback_time;
};

} // namespace sputnik::demos