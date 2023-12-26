#pragma once

#include <string>
#include "api/input_manager.h"

namespace sputnik::graphics::window
{

using namespace sputnik::engine::api;

struct WindowSpecification
{
    unsigned int  m_width         = 1600;
    unsigned int  m_height        = 900;
    std::string   m_title         = "Sputnik";
    bool          m_vsync_enabled = false;
    InputManager* m_input         = InputManager::GetInstance();
};

} // namespace sputnik::graphics::window
