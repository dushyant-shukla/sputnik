#pragma once

namespace sputnik::graphics::core
{

enum class SkinningType
{
    NONE,
    BIND_POSE = NONE,
    REST_POSE,
    CPU,
    GPU
};

} // namespace sputnik::graphics::core