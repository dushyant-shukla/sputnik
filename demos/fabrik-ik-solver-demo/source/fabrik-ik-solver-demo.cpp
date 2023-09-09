#include "fabrik-ik-solver-demo.h"

#include <graphics/glcore/uniform.h>
#include <graphics/glcore/renderer.h>
#include <graphics/glcore/gltf_loader.h>
#include <matrix4.h>
#include <vector3.h>
#include <camera_transforms.h>
#include <imgui.h>
#include <graphics/core/animation/rearrange_bones.h>
#include <editor/editor_camera.h>
#include <ImGuizmo.h>

namespace sputnik::demos
{

FabrikIKDemo::FabrikIKDemo(const std::string& name)
    : core::Layer(name)
    , m_camera_distance(7.0f)
    , m_camera_yaw(60.0f)
    , m_camera_pitch(45.0f)
    , m_projection(Perspective(60.0f, 1600.0f / 900.0f, 0.01f, 1000.0f))
{
    m_camera_position = Vector3(
        m_camera_distance * cosf(m_camera_yaw * Constants::DEG_TO_RAD) * sinf(m_camera_pitch * Constants::DEG_TO_RAD),
        m_camera_distance * cosf(m_camera_pitch * Constants::DEG_TO_RAD),
        m_camera_distance * sinf(m_camera_yaw * Constants::DEG_TO_RAD) * sinf(m_camera_pitch * Constants::DEG_TO_RAD));
    m_view = LookAt(m_camera_position, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
    m_mvp  = m_projection * m_view;
}

FabrikIKDemo::~FabrikIKDemo() {}

void FabrikIKDemo::OnAttach()
{
    m_solver.Resize(6);
    m_solver[0].rotation = AngleAxis(90.0f * Constants::DEG_TO_RAD, {1.0f, 0.0f, 0.0f});
    m_solver[1].position = {0.0f, 0.0f, 1.0f};
    m_solver[2].position = {0.0f, 0.0f, 1.5f};
    m_solver[3].position = {0.0f, 0.0f, 0.5f};
    m_solver[3].rotation = AngleAxis(90.0f * Constants::DEG_TO_RAD, {0.0f, 1.0f, 0.0f});
    m_solver[4].position = {0.0f, 0.0f, 0.5f};
    m_solver[5].position = {0.0f, 0.0f, 0.5f};

    m_target.position = {1.0f, -2.0f, 0.0f};

    m_ccd_lines  = std::make_shared<sputnik::graphics::glcore::DebugDraw>();
    m_ccd_points = std::make_shared<sputnik::graphics::glcore::DebugDraw>();

    glPointSize(15.0f);
    glLineWidth(5.0f);
}

// Todo: On Detach must be called at system cleanup before shutdown
void FabrikIKDemo::OnDetach() {}

void FabrikIKDemo::OnUpdate(const core::TimeStep& time_step)
{
    m_solver.Solve(m_target);

    glDisable(GL_DEPTH_TEST);

    m_ccd_lines->LinesFromIKSolver(m_solver);
    m_ccd_points->PointsFromIKSolver(m_solver);
    m_ccd_lines->UpdateOpenGLBuffers();
    m_ccd_points->UpdateOpenGLBuffers();
    m_ccd_lines->Draw(sputnik::graphics::glcore::DebugDrawMode::Lines, {1.0f, 0.0f, 1.0f}, m_mvp);
    m_ccd_points->Draw(sputnik::graphics::glcore::DebugDrawMode::Points, {1.0f, 1.0f, 0.0f}, m_mvp);

    glEnable(GL_DEPTH_TEST);
}

void FabrikIKDemo::OnEvent() {}

void FabrikIKDemo::OnUpdateUI(const core::TimeStep& time_step)
{
    ImGuizmo::SetGizmoSizeClipSpace(0.075f);
    ramanujan::Matrix4 target_transform = ToMatrix4(m_target);
    ImGuizmo::Manipulate(&(m_view.v[0]),
                         &(m_projection.v[0]),
                         ImGuizmo::OPERATION::TRANSLATE,
                         ImGuizmo::MODE::LOCAL,
                         &(target_transform.v[0]),
                         nullptr,
                         nullptr);
    if(ImGuizmo::IsUsing())
    {
        m_target = ToTransform(target_transform);
    }


    if(ImGui::Begin("Inverse Kinematics(CCD)"))
    {
        ImGui::Text("Target:X");
        ImGui::SameLine();
        ImGui::DragFloat("##taregt_x", &(m_target.position.x), 0.01f, -9999.0, 9999.0, "%.2f");

        ImGui::Text("Target:Y");
        ImGui::SameLine();
        ImGui::DragFloat("##taregt_y", &(m_target.position.y), 0.01f, -9999.0, 9999.0, "%.2f");

        ImGui::Text("Target:Z");
        ImGui::SameLine();
        ImGui::DragFloat("##taregt_z", &(m_target.position.z), 0.01f, -9999.0, 9999.0, "%.2f");
    }
    ImGui::End();
}

} // namespace sputnik::demos