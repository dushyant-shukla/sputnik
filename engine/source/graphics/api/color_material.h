#pragma once

#include <vector.hpp>

namespace sputnik::graphics::api
{

using namespace ramanujan;
using namespace ramanujan::experimental;

struct Material
{
    vec3  ambient{1.0f, 0.5f, 0.31f};
    vec3  diffuse{1.0f, 0.5f, 0.31f};
    vec3  specular{0.5f, 0.5f, 0.5f};
    float shininess{32.0f};
};

} // namespace sputnik::graphics::api
