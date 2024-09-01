#pragma once

#include <glm/glm.hpp>

namespace phx
{

static constexpr float kEpsilon  = std::numeric_limits<float>::epsilon();
static constexpr float kFloatMax = std::numeric_limits<float>::max();
static constexpr float kFloatMin = std::numeric_limits<float>::min();
static constexpr float kPi       = 3.14159265358979323846f;

#define CMP_FLOAT_EQ(a, b) (std::abs((a) - (b)) <= kEpsilon * std::max(1.0f, std::max(std::abs(a), std::abs(b))))

glm::vec3 min(const glm::vec3& a, const glm::vec3& b);
glm::vec3 max(const glm::vec3& a, const glm::vec3& b);

glm::vec3 generateRandomUnitVector();

glm::vec3 purtubate(const glm::vec3& v, const float& epsilon = kEpsilon);

} // namespace phx