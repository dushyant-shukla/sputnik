#pragma once

#include <main/application.h>
// #include <graphics/glcore/shader.h>
#include <graphics/core/animation/animation_clip.h>
#include <graphics/core/animation/pose.h>
// #include <graphics/glcore/debug_draw.h>
#include <graphics/core/animation/skeleton.h>
// #include <graphics/glcore/texture.h>
#include <graphics/core/animation/skinning_type.h>
#include <graphics/core/geometry/mesh.h>
#include <main/entry_point.h>
#include <core/layers/layer.h>
#include <core/time_step.h>
#include <graphics/core/animation/ccd_solver.h>
#include <core/systems/render_system.h>

#include <memory>
#include <vector>

namespace sputnik::demos
{

using namespace sputnik::core::systems;
using namespace ramanujan;
using namespace sputnik::graphics::core;

class IkCcdDemo : public core::Layer
{
public:
    IkCcdDemo(const std::string& name);
    virtual ~IkCcdDemo();

    virtual void OnAttach();
    virtual void OnDetach();
    virtual void OnUpdate(const core::TimeStep& time_step);
    virtual void OnEvent();
    virtual void OnUpdateUI(const core::TimeStep& time_step);

private:
    Transform m_target;
    CCDSolver m_solver;
    // std::shared_ptr<sputnik::graphics::glcore::DebugDraw> m_ccd_lines;
    // std::shared_ptr<sputnik::graphics::glcore::DebugDraw> m_ccd_points;
    RenderSystem* m_render_system{nullptr};

    float m_camera_pitch;
    float m_camera_yaw;
    float m_camera_distance;

    Vector3 m_camera_position;
    Matrix4 m_projection;
    Matrix4 m_view;
    Matrix4 m_mvp;
};

class IkCcd : public sputnik::main::Application
{

public:
    IkCcd(const std::string& name) : sputnik::main::Application::Application(name)
    {
        PushLayer(std::make_shared<IkCcdDemo>(name));
    }

    ~IkCcd() {}
};

} // namespace sputnik::demos

sputnik::main::Application* sputnik::main::CreateApplication()
{
    return new sputnik::demos::IkCcd("Inverse Kinematics(CCD)");
}
