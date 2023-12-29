#pragma once

#include <string>

namespace sputnik::core
{

struct SystemInformation
{
    std::string vendor;
    std::string renderer;
    std::string gl_version;
    std::string shading_language_version;
    bool        is_vsync_enabled{true}; // VSync is enabled by default
};

} // namespace sputnik::core
