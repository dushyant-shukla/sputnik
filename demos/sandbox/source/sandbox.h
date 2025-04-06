#pragma once

#include <main/entry_point.h>
#include <main/application.h>
#include <core/layers/layer.h>
#include <core/time_step.h>

#include <vector2.h>
#include <vector3.h>

#include <memory>

namespace sputnik::demos
{

class SandboxDemoLayer : public core::Layer
{

public:
    SandboxDemoLayer(const std::string& name);
    virtual ~SandboxDemoLayer();

    virtual void OnAttach();
    virtual void OnDetach();
    virtual void OnUpdate(const core::TimeStep& time_step);
    virtual void OnEvent();
    virtual void OnUpdateUI(const core::TimeStep& time_step);

private:
};

class SandboxDemo : public sputnik::main::Application
{

public:
    SandboxDemo(const std::string& name) : sputnik::main::Application(name)
    {
        PushLayer(std::make_shared<SandboxDemoLayer>(name));
    }

    ~SandboxDemo() {}
};

} // namespace sputnik::demos

sputnik::main::Application* sputnik::main::CreateApplication()
{
    return new sputnik::demos::SandboxDemo("Sandbox Demo");
}
