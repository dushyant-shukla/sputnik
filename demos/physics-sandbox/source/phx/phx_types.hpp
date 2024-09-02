#ifndef PHX_TYPES_HPP
#define PHX_TYPES_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

using PhxSize = size_t;

using PhxReal  = float;
using PhxVec2  = glm::vec2;
using PhxVec3  = glm::vec3;
using PhxUvec3 = glm::uvec3;
using PhxVec4  = glm::vec4;
using PhxMat3  = glm::mat3;
using PhxMat4  = glm::mat4;

using PhxRealArray = std::vector<PhxReal>;
using PhxVec2Array = std::vector<PhxVec2>;
using PhxVec3Array = std::vector<PhxVec3>;

using PhxBool      = bool;
using PhxBoolArray = std::vector<PhxBool>;

using PhxIndex      = uint32_t;
using PhxIndexArray = std::vector<PhxIndex>;

using PhxUint = uint32_t;
using PhxInt  = int32_t;

template <typename T>
using PhxArray = std::vector<T>;

#endif // !PHX_TYPES_HPP
