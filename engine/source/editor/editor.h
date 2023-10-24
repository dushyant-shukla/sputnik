#pragma once

#include "core/time_step.h"
#include "graphics/glcore/gl_ui_layer.h"
#include "editor/editor_viewport_canvas.h"

namespace sputnik::editor
{

using namespace sputnik::graphics::glcore;

class Editor
{
public:
    Editor();
    ~Editor();

    void BeginFrame();
    void EndFrame();
    void Update(sputnik::core::TimeStep& time_step);

private:
    std::shared_ptr<GlUiLayer> m_ui_layer;
    std::shared_ptr<sputnik::graphics::EditorViewPortCanvas> m_editor_viewport_canvas;
};

} // namespace sputnik::editor
