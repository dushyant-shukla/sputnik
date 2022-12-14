#pragma once

#include <platform/windows/windows_main.h>
#include <main/application.h>
#include <graphics/glcore/shader.h>
#include <graphics/api/animation/animation_clip.h>
#include <graphics/api/animation/pose.h>
#include <graphics/glcore/debug_draw.h>

#include <memory>
#include <vector>

namespace sputnik::demos
{

class GpuSkinning : public sputnik::main::Application
{

public:
    GpuSkinning();
    ~GpuSkinning();

    virtual void Initialize() override;
    virtual void Update(float delta_time) override;
    virtual void Render(float aspect_ratio) override;
    virtual void Shutdown() override;

private:
    sputnik::glcore::DebugDraw*                         m_rest_pose_visual;
    sputnik::glcore::DebugDraw*                         m_current_pose_visual;
    sputnik::api::animation::Pose                       m_rest_pose;
    sputnik::api::animation::Pose                       m_current_pose;
    bool                                                m_show_rest_pose;
    bool                                                m_show_current_pose;
    std::vector<sputnik::api::animation::AnimationClip> m_clips;
    size_t                                              m_current_clip;
    float                                               m_playback_time;
};

} // namespace sputnik::demos