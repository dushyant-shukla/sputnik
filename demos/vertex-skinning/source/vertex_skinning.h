#pragma once

#include <main/application.h>
#include <graphics/glcore/shader.h>
#include <graphics/core/animation/animation_clip.h>
#include <graphics/core/animation/pose.h>
#include <graphics/glcore/debug_draw.h>
#include <graphics/core/animation/skeleton.h>
#include <graphics/glcore/texture.h>
#include <graphics/core/animation/skinning_type.h>
#include <graphics/core/geometry/mesh.h>
#include <main/entry_point.h>
#include <core/layers/layer.h>
#include <core/time_step.h>

#include <memory>
#include <vector>

namespace sputnik::demos
{

class VertexSkinningDemo : public core::Layer
{
public:
    VertexSkinningDemo(const std::string& name);
    virtual ~VertexSkinningDemo();

    virtual void OnAttach();
    virtual void OnDetach();
    virtual void OnUpdate(const core::TimeStep& time_step);
    virtual void OnEvent();
    virtual void OnUpdateUI(const core::TimeStep& time_step);

private:
    std::shared_ptr<sputnik::graphics::glcore::Texture> m_diffuse_texture;

    sputnik::graphics::core::SkinningType    m_skinning_type;
    std::shared_ptr<sputnik::graphics::glcore::Shader> m_static_shader;
    std::shared_ptr<sputnik::graphics::glcore::Shader> m_skinning_shader;

    std::shared_ptr<sputnik::graphics::glcore::DebugDraw> m_rest_pose_visual;
    std::shared_ptr<sputnik::graphics::glcore::DebugDraw> m_bind_pose_visual;
    std::shared_ptr<sputnik::graphics::glcore::DebugDraw> m_current_pose_visual;

    std::vector<sputnik::graphics::core::Mesh> m_meshes;
    std::vector<ramanujan::Matrix4>            m_pose_palette;

    sputnik::graphics::core::Skeleton m_skeleton;
    sputnik::graphics::core::Pose     m_rest_pose;
    sputnik::graphics::core::Pose     m_current_pose;

    bool m_show_rest_pose;
    bool m_show_current_pose;
    bool m_show_bind_pose;

    std::vector<sputnik::graphics::core::AnimationClip> m_clips;
    int                                                 m_current_clip;
    float                                               m_playback_time;
    std::string                                         m_clip_types_str;

    // GUI stuff
    int         m_skinning_type_index;
    std::string m_skinnig_types_str;
};

class VertexSkinning : public sputnik::main::Application
{

public:
    VertexSkinning(const std::string& name) : sputnik::main::Application::Application(name)
    {
        PushLayer(std::make_shared<VertexSkinningDemo>(name));
    }

    ~VertexSkinning() {}

    // virtual void Initialize() override;
    // virtual void Update(float delta_time) override;
    // virtual void Render(float aspect_ratio) override;
    // virtual void Shutdown() override;

    // private:
    //     std::shared_ptr<sputnik::glcore::Texture> m_diffuse_texture;
    //
    //     sputnik::api::animation::SkinningType    m_skinning_type;
    //     std::shared_ptr<sputnik::glcore::Shader> m_static_shader;
    //     std::shared_ptr<sputnik::glcore::Shader> m_skinning_shader;
    //
    //     std::shared_ptr<sputnik::glcore::DebugDraw> m_rest_pose_visual;
    //     std::shared_ptr<sputnik::glcore::DebugDraw> m_bind_pose_visual;
    //     std::shared_ptr<sputnik::glcore::DebugDraw> m_current_pose_visual;
    //
    //     std::vector<sputnik::api::Mesh> m_meshes;
    //     std::vector<ramanujan::Matrix4> m_pose_palette;
    //
    //     sputnik::api::animation::Skeleton m_skeleton;
    //     sputnik::api::animation::Pose     m_current_pose;
    //
    //     bool m_show_rest_pose;
    //     bool m_show_current_pose;
    //     bool m_show_bind_pose;
    //
    //     std::vector<sputnik::api::animation::AnimationClip> m_clips;
    //     int                                                 m_current_clip;
    //     float                                               m_playback_time;
    //     std::string                                         m_clip_types_str;
    //
    //     // GUI stuff
    //     int         m_skinning_type_index;
    //     std::string m_skinnig_types_str;
};

} // namespace sputnik::demos

sputnik::main::Application* sputnik::main::CreateApplication()
{
    return new sputnik::demos::VertexSkinning("Vertex Skinning");
}
