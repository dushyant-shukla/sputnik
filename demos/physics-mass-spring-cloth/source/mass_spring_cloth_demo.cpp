#include "mass_spring_cloth_demo.h"

// #include <graphics/api/renderer.h>
// #include <graphics/glcore/uniform.h>
#include <graphics/api/color_material.h>
#include <core/systems/render_system.h>

#include <core/logging/logging_core.h>
#include <physics/physics_core.h>

#include <imgui.h>
#include <ImGuizmo.h>

#include <core/core.h>

namespace sputnik::demos
{

MassSpringClothDemoLayer::MassSpringClothDemoLayer(const std::string& name) : core::Layer{name} {}

MassSpringClothDemoLayer::~MassSpringClothDemoLayer() {}

void MassSpringClothDemoLayer::OnAttach()
{

    m_sphere = Model::LoadModel("../../data/assets/sphere.gltf");
}

void MassSpringClothDemoLayer::OnDetach() {}

void MassSpringClothDemoLayer::OnUpdate(const core::TimeStep& time_step)
{

    if(time_step.GetSeconds() <= Constants::EPSILON)
    {
        return;
    }
}

void MassSpringClothDemoLayer::OnEvent() {}

void MassSpringClothDemoLayer::OnUpdateUI(const core::TimeStep& time_step) {}

} // namespace sputnik::demos
