#pragma once

#include <vector.hpp>

namespace sputnik::graphics::api
{

using namespace ramanujan;
using namespace ramanujan::experimental;

struct Light
{
    vec3  position{0.0f, 20.0f, 0.0f};
    vec3  ambient{0.2f, 0.2f, 0.2f};
    vec3  diffuse{0.5f, 0.5f, 0.5f};
    vec3  specular{1.0f, 1.0f, 1.0f};
    float constant{1.0f};
    float linear{0.09f};
    float quadratic{0.032f};
};

} // namespace sputnik::graphics::api
