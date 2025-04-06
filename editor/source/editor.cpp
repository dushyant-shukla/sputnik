#include "editor.hpp"
#include "editor_viewport.hpp"

#include <graphics/api/renderer.h>

#include <iostream>

namespace sputnik::expeditor
{

Editor::Editor(sputnik::main::Application* const app) : m_app(app)
{
    // Should the initialization happen here?
}

Editor::~Editor() {}

void Editor::initialize()
{

    if(this == nullptr)
    {
        std::cout << "Editor is null\n";
        return;
    }
    else
    {
        std::cout << "Editor initialized: " << m_app->GetApplicationName() << "\n";
    }

    // const auto& [width, height] = graphics::api::Renderer::GetViewportDimensions();
    // m_viewport                  = std::make_unique<EditorViewport>(width, height);
}

void Editor::shutdown()
{
    std::cout << "Editor shutdown\n";
}

void Editor::update(const float& timestep)
{
    std::cout << "Editor update with timestep:\t" << timestep << "\n";

    // Bind editor viewport framebuffer
    // m_viewport->beginFrame();

    // Render editor viewport, i.e. render scene by calling demo dll's update function

    // Unbind editor viewport framebuffer
    // m_viewport->endFrame();
}

void Editor::beginFrame() {}

void Editor::endFrame() {}

} // namespace sputnik::expeditor
