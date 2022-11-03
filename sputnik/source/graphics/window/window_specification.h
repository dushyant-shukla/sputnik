#pragma once

#include <string>
#include "core/input/input_manager.h"

namespace sputnik::graphics::window
{
struct WindowSpecification
{
    unsigned int                 m_width         = 1600;
    unsigned int                 m_height        = 900;
    std::string                  m_title         = "Sputnik";
    bool                         m_vsync_enabled = false;
    sputnik::core::InputManager* m_input         = sputnik::core::InputManager::GetInstance();
};

} // namespace sputnik::graphics::window
