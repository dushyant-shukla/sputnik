#pragma once

#include <vector.hpp>
#include <matrix.hpp>

namespace sputnik::graphics::api
{

using namespace ramanujan;
using namespace ramanujan::experimental;

struct Light
{
    alignas(16) vec3 position{0.0f, 20.0f, 0.0f};
    alignas(16) vec3 ambient{0.2f, 0.2f, 0.2f};
    alignas(16) vec3 diffuse{0.5f, 0.5f, 0.5f};
    alignas(16) vec3 specular{1.0f, 1.0f, 1.0f};
    alignas(4) float constant{0.0f};
    alignas(4) float linear{0.0f};
    alignas(4) float quadratic{0.0f};
};

struct PerFrameData
{
    alignas(16) mat4 projection;
    alignas(16) mat4 view;
    alignas(16) vec3 camera_position;
};

} // namespace sputnik::graphics::api
