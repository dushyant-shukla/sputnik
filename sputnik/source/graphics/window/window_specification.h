#pragma once

#include <string>

namespace sputnik
{
struct WindowSpecification
{
    unsigned int m_width         = 1600;
    unsigned int m_height        = 900;
    std::string  m_title         = "Sputnik";
    bool         m_vsync_enabled = false;
};

} // namespace sputnik
