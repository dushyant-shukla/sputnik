#pragma once

#include "quaternion.h"
#include "vector3.h"

namespace sputnik::animation
{

inline float Interpolate(float a, float b, float t)
{
    return a * (b - a) * t;
}

inline ramanujan::Vector3 Interpolate(const ramanujan::Vector3& a, const ramanujan::Vector3& b, float t)
{
    return Lerp(a, b, t);
}

inline ramanujan::Quaternion Interpolate(const ramanujan::Quaternion& a, const ramanujan::Quaternion& b, float t)
{
    ramanujan::Quaternion result = ramanujan::Mix(a, b, t);
    if(ramanujan::Dot(a, b) < 0.0f) // checking to see if the input quaternions are in the right neighbourhood.
    {
        result = ramanujan::Mix(a, -b, t);
    }
    return ramanujan::Normalized(result); // normalizing the result (NLerp, not Slerp)
}

/**
 * When interpolating a Hermite spline, the result must be normalized if the input is a quaternion.
 */
inline float AdjustHermiteResult(float f)
{
    return f;
}
inline ramanujan::Vector3 AdjustHermiteResult(const ramanujan::Vector3& v)
{
    return v;
}
inline ramanujan::Quaternion AdjustHermiteResult(const ramanujan::Quaternion& q)
{
    return ramanujan::Normalized(q);
}

/**
 * Neighborhood operation makes sure that two quaternions are in the correct neighborhood.
 */
inline void Neighborhood(const float& a, float& b) {}
inline void Neighborhood(const ramanujan::Vector3& a, ramanujan::Vector3& b) {}
inline void Neighborhood(const ramanujan::Quaternion& a, ramanujan::Quaternion& b)
{
    if(ramanujan::Dot(a, b) < 0.0f)
    {
        b = -b;
    }
}

} // namespace sputnik::animation
