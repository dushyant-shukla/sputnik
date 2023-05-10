#pragma once

#include "core/layers/layer.h"
#include "graphics/glcore/gl_frame_buffer.h"

#include <vector2.h>

namespace sputnik::graphics
{

using namespace sputnik::core;

class EditorViewPortCanvas : public Layer
{

public:
    EditorViewPortCanvas(unsigned int width, unsigned int height);
    virtual ~EditorViewPortCanvas();

    virtual void OnAttach();
    virtual void OnDetach();

    virtual void OnPreUpdate(const TimeStep& time_step) override;
    virtual void OnUpdate(const TimeStep& time_step) override;
    virtual void OnPostUpdate(const TimeStep& time_step) override;

    virtual void OnPreUpdateUI(const TimeStep& time_step) override;
    virtual void OnUpdateUI(const TimeStep& time_step) override;
    virtual void OnPostUpdateUI(const TimeStep& time_step) override;

    virtual void OnEvent();

private:
    // private data
    unsigned int                         m_window_width{0};
    unsigned int                         m_window_height{0};
    ramanujan::Vector2                   m_viewport_size{0.0f};
    std::shared_ptr<glcore::FrameBuffer> m_framebuffer;
    glcore::FrameBufferSpecification     m_framebuffer_spec;
};

} // namespace sputnik::graphics