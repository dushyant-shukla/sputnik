#pragma once

namespace sputnik::api::animation
{

enum class SkinningType
{
    NONE,
    BIND_POSE = NONE,
    REST_POSE,
    CPU,
    GPU
};

} // namespace sputnik::api::animation