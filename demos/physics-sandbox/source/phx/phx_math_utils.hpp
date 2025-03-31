#pragma once

#include "phx_types.hpp"

namespace phx
{

static constexpr float kPhxEpsilon  = std::numeric_limits<float>::epsilon();
static constexpr float kPhxFloatMax = std::numeric_limits<float>::max();
static constexpr float kPhxFloatMin = std::numeric_limits<float>::min();
static constexpr float kPhxPi       = 3.14159265358979323846f;

#define CMP_FLOAT_EQ(a, b) \
    (std::abs((a) - (b)) <= phx::kPhxEpsilon * std::max(1.0f, std::max(std::abs(a), std::abs(b))))

PhxVec3 phxMin(const PhxVec3& a, const PhxVec3& b);
PhxVec3 phxMax(const PhxVec3& a, const PhxVec3& b);

PhxVec3 phxGenerateRandomUnitVector();

PhxVec3 phxPerturbate(const PhxVec3& v, const float& epsilon = kPhxEpsilon);

PhxVec3 phxRotatePoint(const PhxQuat& q, const PhxVec3& p);

// clang-format off

#define phx_normalize(a)		glm::normalize(a)
#define phx_dot(a, b)			glm::dot(a, b)
#define phx_cross(a, b)			glm::cross(a, b)
#define phx_length(a)			glm::length(a)
#define phx_magnitude(a)		glm::length(a)
#define phx_magnitude_sq(a)		glm::dot(a, a)
#define phx_distance(a, b)		glm::distance(a, b)
#define phx_reflect(a, b)		glm::reflect(a, b)
#define phx_refract(a, b, c)	glm::refract(a, b, c)
#define phx_mix(a, b, c)		glm::mix(a, b, c)
#define phx_clamp(a, b, c)		glm::clamp(a, b, c)
#define phx_smoothstep(a, b, c) glm::smoothstep(a, b, c)
#define phx_step(a, b)			glm::step(a, b)
#define phx_abs(a)				glm::abs(a)
#define phx_atan2(a, b)			glm::atan(a, b)
#define phx_radians(a)			glm::radians(a)
#define phx_degrees(a)			glm::degrees(a)
#define phx_inv_quat(a)			glm::inverse(a)
//#define phx_lerp(a, b, c)		glm::lerp(a, b, c)
//#define phx_sqrt(a)				glm::sqrt(a)
//#define phx_pow(a, b)			glm::pow(a, b)
//#define phx_exp(a)				glm::exp(a)
//#define phx_log(a)				glm::log(a)
//#define phx_sin(a)				glm::sin(a)
//#define phx_cos(a)				glm::cos(a)
//#define phx_tan(a)				glm::tan(a)
//#define phx_asin(a)				glm::asin(a)
//#define phx_acos(a)				glm::acos(a)
//#define phx_atan(a)				glm::atan(a)

// clang-format on

} // namespace phx