#include "sandbox.h"

namespace sputnik::demos
{

SandboxDemoLayer::SandboxDemoLayer(const std::string& name) : core::Layer{name} {}

SandboxDemoLayer::~SandboxDemoLayer() {}

void SandboxDemoLayer::OnAttach() {}

void SandboxDemoLayer::OnDetach() {}

void SandboxDemoLayer::OnUpdate(const core::TimeStep& time_step) {}

void SandboxDemoLayer::OnEvent() {}

void SandboxDemoLayer::OnUpdateUI(const core::TimeStep& time_step) {}

} // namespace sputnik::demos
