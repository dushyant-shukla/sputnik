#pragma once

#include <memory>
#include <main/application.h>

namespace sputnik::editor
{

class EditorViewport;

class Editor
{

public:
    Editor(sputnik::main::Application* const app);
    ~Editor();

    void initialize();
    void shutdown();
    void update(const float& timestep);

private:
    void beginFrame();
    void endFrame();

private:
    sputnik::main::Application* m_app{nullptr};

    std::unique_ptr<EditorViewport> m_viewport{nullptr};
};

} // namespace sputnik::expeditor

static std::shared_ptr<sputnik::expeditor::Editor> s_editor = nullptr;

extern "C" __declspec(dllexport) void initialize(sputnik::main::Application* const app)
{
    s_editor = std::make_shared<sputnik::expeditor::Editor>(app);
    s_editor->initialize();
}

extern "C" __declspec(dllexport) void shutdown()
{
    s_editor->shutdown();
}

extern "C" __declspec(dllexport) void update(float timestep)
{
    s_editor->update(timestep);
}
