#pragma once

#include "core/time_step.h"
#include "graphics/glcore/gl_ui_layer.h"
#include <vector.hpp>

namespace sputnik::editor
{

class EditorViewport;

using namespace ramanujan::experimental;
using namespace sputnik::graphics::glcore;

class EditorNew
{
public:
    virtual ~EditorNew();

    static EditorNew* getInstance();

    virtual void beginFrame();
    virtual void endFrame();
    virtual void update(sputnik::core::TimeStep& time_step);

    static void drawWidgetFloat(const std::string& label, float& value, const float& widget_width);

    static void
    drawWidgetVec3(const std::string& label, vec3& value, const float& widget_width, float default_value = 0.0f);

    static void
    drawWidgetVec4(const std::string& label, vec4& value, const float& widget_width, float default_value = 0.0f);

    static void
    drawWidgetVec2(const std::string& label, vec2& value, const float& widget_width, float default_value = 0.0f);

    static void
    drawWidgetColor(const std::string& label, vec4& value, const float& widget_width, float default_value = 0.0f);

    static void setMayaThemecolors();

protected:
    EditorNew();

    void beginDockspace();
    void endDockspace();
    void renderMenuBar();

private:
    std::unique_ptr<EditorViewport> m_viewport;
    SystemInformation               m_system_information;
};

} // namespace sputnik::editor
