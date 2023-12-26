#pragma once

#include <core/core.h>

#include <memory>

namespace sputnik::graphics::gl
{
class OglFramebuffer;
}

namespace sputnik::editor
{

using namespace sputnik::graphics::gl;

class EditorViewport
{
public:
    EditorViewport(const u32& width, const u32& height);
    ~EditorViewport();

    void beginFrame();
    void endFrame();
    void update(const core::TimeStep& timestep);

private:
    void resizeFramebuffer();
    void renderEditorViewport();
    bool shouldResizeFramebuffer();

private:
    std::unique_ptr<OglFramebuffer> m_framebuffer;

    u32                     m_width;
    u32                     m_height;
    std::pair<float, float> m_viewport_size{0.0f, 0.0f};
    std::pair<float, float> m_viewport_bounds[2];

    bool m_viewport_focused{false};
    bool m_viewport_hovered{false};
    bool m_block_camera_update{false};
};

} // namespace sputnik::editor
