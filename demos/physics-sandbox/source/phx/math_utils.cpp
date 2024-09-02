#include "math_utils.hpp"

#include <random>
#include <cmath>

namespace phx
{

PhxVec3 phxMin(const PhxVec3& a, const PhxVec3& b)
{
    return PhxVec3(fminf(a.x, b.x), fminf(a.y, b.y), fminf(a.z, b.z));
}

PhxVec3 phxMax(const PhxVec3& a, const PhxVec3& b)
{
    return PhxVec3(fmaxf(a.x, b.x), fmaxf(a.y, b.y), fmaxf(a.z, b.z));
}

PhxVec3 phxGenerateRandomUnitVector()
{
    // This method generates a random unit vector by generating a random point on the unit sphere.
    PhxVec3 result = {};

    std::random_device rd; // Non deterministic generator. Will be used to obtain a seed for the random number engine.
    std::mt19937       gen(rd());                          // Seeding the Mersenne Twister generator
    std::uniform_real_distribution<float> dis(0.0f, 1.0f); // Generating a uniform distribution between 0 and 1

    // Generating random spherical coordinates
    float theta = dis(gen) * 2.0f * kPhxPi;     // Generating a random azimuthal angle between 0 and 2 * pi
    float phi   = acos(2.0f * dis(gen) - 1.0f); // Generating a random polar angle between 0 and pi. This ensures that
                                              // cos(phi) is uniformly distributed between -1 and 1, which results in a
                                              // uniform distribution of points on the sphere.

    // Convert spherical coordinates to cartesian coordinates.
    result.x = sin(phi) * cos(theta);
    result.y = sin(phi) * sin(theta);
    result.z = cos(phi);

    // return result;
    return purtubate(result);
}

PhxVec3 purtubate(const PhxVec3& v, const float& epsilon)
{
    PhxVec3 result = v;

    std::random_device rd; // Non deterministic generator. Will be used to obtain a seed for the random number engine.
    std::mt19937       gen(rd()); // Seeding the Mersenne Twister generator
    std::uniform_real_distribution<float> dis(
        -epsilon,
        epsilon); // Generating a uniform distribution between -epsilon and epsilon

    result.x += dis(gen);
    result.y += dis(gen);
    result.z += dis(gen);

    return phx_normalize(result);
}

} // namespace phx