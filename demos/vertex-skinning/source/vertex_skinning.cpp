#include "vertex_skinning.h"

#include <graphics/glcore/uniform.h>
#include <graphics/glcore/renderer.h>
#include <graphics/glcore/gltf_loader.h>
#include <matrix4.h>
#include <vector3.h>
#include <camera_transforms.h>
#include <imgui.h>
#include <graphics/core/animation/rearrange_bones.h>
#include <graphics/api/editor_camera.h>

namespace sputnik::demos
{

VertexSkinningDemo::VertexSkinningDemo(const std::string& name)
    : core::Layer(name)
    , m_show_current_pose(false)
    , m_show_rest_pose(false)
    , m_show_bind_pose(false)
    , m_current_clip(0)
    , m_playback_time(0.0f)
    , m_current_pose_visual(nullptr)
    , m_rest_pose_visual(nullptr)
    , m_bind_pose_visual(nullptr)
    , m_skinning_type(sputnik::graphics::core::SkinningType::BIND_POSE)
    , m_skinning_type_index(0)
{
    m_skinnig_types_str += "BIND POSE";
    m_skinnig_types_str += '\0';
    m_skinnig_types_str += "REST POSE";
    m_skinnig_types_str += '\0';
    m_skinnig_types_str += "CPU";
    m_skinnig_types_str += '\0';
    m_skinnig_types_str += "GPU";
    m_skinnig_types_str += '\0';
    m_skinnig_types_str += '\0';
}
VertexSkinningDemo::~VertexSkinningDemo() {}

void VertexSkinningDemo::OnAttach()
{
    m_static_shader   = std::make_shared<sputnik::graphics::glcore::Shader>("../../data/shaders/simple.vert",
                                                                          "../../data/shaders/simple.frag");
    m_skinning_shader = std::make_shared<sputnik::graphics::glcore::Shader>("../../data/shaders/skinned.vert",
                                                                            "../../data/shaders/simple.frag");

    cgltf_data* gltf  = sputnik::gltf::GltfLoader::LoadFile("../../data/assets/Woman.gltf");
    m_diffuse_texture = std::make_shared<sputnik::graphics::glcore::Texture>("../../data/assets/Woman.png");

    // cgltf_data* gltf  = sputnik::gltf::GltfLoader::LoadFile("../../data/assets/CesiumMan/CesiumMan.gltf");
    // m_diffuse_texture =
    // std::make_shared<sputnik::graphics::glcore::Texture>("../../data/assets/CesiumMan/CesiumMan_img0.jpg");

    // cgltf_data* gltf  = sputnik::gltf::GltfLoader::LoadFile("../../data/assets/Fox/Fox.gltf");
    // m_diffuse_texture = std::make_shared<sputnik::graphics::glcore::Texture>("../../data/assets/Fox/Texture.png");

    // cgltf_data* gltf  = sputnik::gltf::GltfLoader::LoadFile("../../data/assets/nathan/scene.gltf");
    // m_diffuse_texture = std::make_shared<sputnik::graphics::glcore::Texture>(
    //     "../../data/assets/nathan/textures/rp_nathan_animated_003_mat_baseColor.jpeg");

    // cgltf_data* gltf = sputnik::gltf::GltfLoader::LoadFile("../../data/assets/spiderman/scene.gltf");
    // m_diffuse_texture = std::make_shared<sputnik::graphics::glcore::Texture>(
    //    "../../data/assets/spiderman/textures/RootNode_baseColor.jpeg");

    m_skeleton = sputnik::gltf::GltfLoader::LoadSkeleton(gltf);
    m_meshes   = sputnik::gltf::GltfLoader::LoadMeshes(gltf);
    sputnik::gltf::GltfLoader::LoadAnimationClips(gltf, m_clips);
    sputnik::gltf::GltfLoader::FreeFile(gltf);

    sputnik::graphics::core::BoneMap bones = sputnik::graphics::core::RearrangeSkeleton(m_skeleton);
    for(unsigned int i = 0, size = static_cast<unsigned int>(m_meshes.size()); i < size; ++i)
    {
        sputnik::graphics::core::RearrangeMesh(m_meshes[i], bones);
    }
    for(unsigned int i = 0, size = static_cast<unsigned int>(m_clips.size()); i < size; ++i)
    {
        sputnik::graphics::core::RearrangeClip(m_clips[i], bones);
    }

    m_fast_clips.resize(m_clips.size());
    for(size_t i = 0, size = m_clips.size(); i < size; ++i)
    {
        m_fast_clips[i] = sputnik::graphics::core::OptimizeClip(m_clips[i]);
        m_clip_types_str += m_clips[i].GetName();
        m_clip_types_str += '\0';
    }
    m_clip_types_str += '\0';

    m_rest_pose_visual = std::make_shared<sputnik::graphics::glcore::DebugDraw>();
    m_rest_pose_visual->FromPose(m_skeleton.GetRestPose());
    m_rest_pose_visual->UpdateOpenGLBuffers();

    m_bind_pose_visual = std::make_shared<sputnik::graphics::glcore::DebugDraw>();
    m_bind_pose_visual->FromPose(m_skeleton.GetBindPose());
    m_bind_pose_visual->UpdateOpenGLBuffers();

    m_rest_pose    = m_skeleton.GetRestPose();
    m_current_pose = m_skeleton.GetRestPose();

    m_current_pose_visual = std::make_shared<sputnik::graphics::glcore::DebugDraw>();
    m_current_pose_visual->FromPose(m_current_pose);
    m_current_pose_visual->UpdateOpenGLBuffers();

    glPointSize(5.0f);
    glLineWidth(1.5f);
}

// Todo: On Detach must be called at system cleanup before shutdown
void VertexSkinningDemo::OnDetach()
{
    m_clips.clear();
    m_fast_clips.clear();
}

void VertexSkinningDemo::OnUpdate(const core::TimeStep& time_step)
{
    m_playback_time = m_fast_clips[m_current_clip].Sample(m_current_pose, m_playback_time + time_step);
    // m_playback_time = m_clips[m_current_clip].Sample(m_current_pose, m_playback_time + time_step);
    m_current_pose_visual->FromPose(m_current_pose);

    if(m_skinning_type != sputnik::graphics::core::SkinningType::NONE)
    {
        if(m_skinning_type == sputnik::graphics::core::SkinningType::REST_POSE)
        {
            m_rest_pose.GetMatrixPalette(m_pose_palette);
        }
        else
        {
            m_current_pose.GetMatrixPalette(m_pose_palette);
        }
        const std::vector<ramanujan::Matrix4>& inverse_bind_pose = m_skeleton.GetInverseBindPose();
        for(size_t i = 0; i < m_pose_palette.size(); ++i)
        {
            m_pose_palette[i] = m_pose_palette[i] * inverse_bind_pose[i];
        }

        if(m_skinning_type == sputnik::graphics::core::SkinningType::CPU)
        {
            for(unsigned int i = 0, size = (unsigned int)m_meshes.size(); i < size; ++i)
            {
                m_meshes[i].CpuSkin(m_pose_palette); // Todo: when do we update uv and indices for cpu skin?
            }
        }
    }

#ifdef NON_OPTIMIZED

    // This implementation is not optimized
    switch(m_skinning_type)
    {
    case sputnik::api::animation::SkinningType::NONE:
        break;
    case sputnik::api::animation::SkinningType::CPU:
    {
        for(unsigned int i = 0, size = (unsigned int)m_meshes.size(); i < size; ++i)
        {
            m_meshes[i].CpuSkin(m_skeleton, m_current_pose); // Todo: when do we update uv and indices for cpu skin?
        }
        break;
    }
    case sputnik::api::animation::SkinningType::GPU:
    {
        m_current_pose.GetMatrixPalette(m_pose_palette);
        const std::vector<ramanujan::Matrix4>& inverse_bind_pose = m_skeleton.GetInverseBindPose();
        for(size_t i = 0; i < m_pose_palette.size(); ++i)
        {
            m_pose_palette[i] = m_pose_palette[i] * inverse_bind_pose[i];
        }

        // This is important when changing the animation model or changing the animation clips.
        for(unsigned int i = 0, size = (unsigned int)m_meshes.size(); i < size; ++i)
        {
            m_meshes[i].ResetOpenglBuffersToBindPose();
        }
        break;
    }
    default:
        break;
    }

#endif // 0

    // ramanujan::Matrix4 model      = ramanujan::ToMatrix4(ramanujan::AngleAxis(135.0f, {0.0f, 1.0f, 0.0f}));
    ramanujan::Matrix4 model;
    // ramanujan::Matrix4 projection = ramanujan::Perspective(60.0f, aspect_ratio, 0.01f, 1000.0f);
    ramanujan::Matrix4 projection = ramanujan::Perspective(60.0f, 1600.0f / 900.0f, 0.01f, 1000.0f);
    ramanujan::Matrix4 view = ramanujan::LookAt({0.0f, 5.0f, 7.0f}, {0.0f, 3.0f, 0.0f}, {0.0f, 1.0f, 0.0f}); // woman
    // ramanujan::Matrix4 view = ramanujan::LookAt({0.0f, 0.0f, 10.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}); //
    // woman

    // ramanujan::Matrix4 view = ramanujan::LookAt({0.0f, 20.0f, -350.0f}, {0.0f, 20.0f, 0.0f}, {0.0f, 1.0f,
    // 0.0f}); // spiderman + nathan

    const auto& editor_camera = sputnik::graphics::api::EditorCamera::GetInstance();
    projection                = editor_camera->GetCameraPerspective();
    view                      = editor_camera->GetCameraView();

    ramanujan::Matrix4 mvp = projection * view * model;

    std::shared_ptr<sputnik::graphics::glcore::Shader> active_shader = m_static_shader;
    if(m_skinning_type == sputnik::graphics::core::SkinningType::GPU ||
       m_skinning_type == sputnik::graphics::core::SkinningType::REST_POSE)
    {
        active_shader = m_skinning_shader;
    }

    active_shader->Bind();
    sputnik::graphics::glcore::Uniform<ramanujan::Matrix4>::Set(active_shader->GetUniform("model"), model);
    sputnik::graphics::glcore::Uniform<ramanujan::Matrix4>::Set(active_shader->GetUniform("view"), view);
    sputnik::graphics::glcore::Uniform<ramanujan::Matrix4>::Set(active_shader->GetUniform("projection"), projection);
    sputnik::graphics::glcore::Uniform<ramanujan::Vector3>::Set(active_shader->GetUniform("light"),
                                                                {0.0f, 10.0f, 10.0f});

    if(m_skinning_type == sputnik::graphics::core::SkinningType::GPU ||
       m_skinning_type == sputnik::graphics::core::SkinningType::REST_POSE)
    {
        sputnik::graphics::glcore::Uniform<ramanujan::Matrix4>::Set(active_shader->GetUniform("skin_transforms"),
                                                                    m_pose_palette);
    }

    m_diffuse_texture->Set(active_shader->GetUniform("diffuse"), 0);
    for(unsigned int i = 0, size = (unsigned int)m_meshes.size(); i < size; ++i)
    {
        int weights    = -1;
        int influences = -1;
        if(m_skinning_type == sputnik::graphics::core::SkinningType::GPU ||
           m_skinning_type == sputnik::graphics::core::SkinningType::REST_POSE)
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
    if(m_show_rest_pose && m_skinning_type == sputnik::graphics::core::SkinningType::REST_POSE)
    {
        m_rest_pose_visual->Draw(sputnik::graphics::glcore::DebugDrawMode::Lines, {1.0f, 0.0f, 0.0f}, mvp);
    }
    if(m_show_bind_pose && m_skinning_type == sputnik::graphics::core::SkinningType::NONE)
    {
        m_bind_pose_visual->Draw(sputnik::graphics::glcore::DebugDrawMode::Lines, {0.0f, 1.0f, 0.0f}, mvp);
    }
    if(m_show_current_pose && (m_skinning_type == sputnik::graphics::core::SkinningType::CPU ||
                               m_skinning_type == sputnik::graphics::core::SkinningType::GPU))
    {
        m_current_pose_visual->UpdateOpenGLBuffers();
        m_current_pose_visual->Draw(sputnik::graphics::glcore::DebugDrawMode::Lines, {0.0f, 0.0f, 1.0f}, mvp);
    }
    glEnable(GL_DEPTH_TEST);
}

void VertexSkinningDemo::OnEvent() {}

void VertexSkinningDemo::OnUpdateUI(const core::TimeStep& time_step)
{
    if(ImGui::Begin("Vertex Skinning"))
    {
        if(ImGui::Combo("Animation Clip", &m_current_clip, m_clip_types_str.c_str()))
        {
        }
        if(ImGui::Combo("Skinning Type", &m_skinning_type_index, m_skinnig_types_str.c_str()))
        {
            m_skinning_type = static_cast<sputnik::graphics::core::SkinningType>(m_skinning_type_index);
            // This is important when changing the animation model/current clip/skinning type, etc.
            for(unsigned int i = 0, size = (unsigned int)m_meshes.size(); i < size; ++i)
            {
                m_meshes[i].ResetOpenglBuffersToBindPose(); // update pose to bind pose.
            }
        }
        ImGui::Checkbox("Show rest pose", &m_show_rest_pose);
        ImGui::Checkbox("Show bind pose", &m_show_bind_pose);
        ImGui::Checkbox("Show current pose", &m_show_current_pose);
    }
    ImGui::End();
}

} // namespace sputnik::demos