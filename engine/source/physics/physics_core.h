#pragma once

#include <vector.hpp>

namespace sputnik::physics
{

using namespace ramanujan;
using namespace ramanujan::experimental;

const vec3 kGravity(real(0), real(-9.81), real(0));
const vec3 kHighGravity(real(0), real(-19.62), real(0));
const vec3 kRight(real(1), real(0), real(0));
const vec3 kUp(real(0), real(1), real(0));
const vec3 kOutOfScreen(real(0), real(0), real(1));
const vec3 kAxisX(real(1), real(0), real(0));
const vec3 kAxisY(real(0), real(1), real(0));
const vec3 kAxisZ(real(0), real(0), real(1));

} // namespace sputnik::physics
