#include "pch.h"
#include "PreethamSkyModel.h"

#include <graphics/glcore/uniform.h>
#include <vector3.h>
#include <vector4.h>

#define _USE_MATH_DEFINES
#include <math.h>

namespace sputnik::graphics::api
{
// -----------------------------------------------------------------------------------------------------------------------------------

float zenith_chromacity(const vec4& c0, const vec4& c1, const vec4& c2, float sunTheta, float turbidity)
{
    vec4 thetav = vec4(sunTheta * sunTheta * sunTheta, sunTheta * sunTheta, sunTheta, 1);
    return vec3(turbidity * turbidity, turbidity, 1).dot(vec3(thetav.dot(c0), thetav.dot(c1), thetav.dot(c2)));
    // return glm::dot(glm::vec3(turbidity * turbidity, turbidity, 1),
    //                 glm::vec3(glm::dot(thetav, c0), glm::dot(thetav, c1), glm::dot(thetav, c2)));
}

// -----------------------------------------------------------------------------------------------------------------------------------

float zenith_luminance(float sunTheta, float turbidity)
{
    float chi = (4.f / 9.f - turbidity / 120) * (M_PI - 2 * sunTheta);
    return (4.0453 * turbidity - 4.9710) * tan(chi) - 0.2155 * turbidity + 2.4192;
}

// -----------------------------------------------------------------------------------------------------------------------------------

float perez(float theta, float gamma, float A, float B, float C, float D, float E)
{
    return (1.f + A * exp(B / (cos(theta) + 0.01))) * (1.f + C * exp(D * gamma) + E * cos(gamma) * cos(gamma));
}

// -----------------------------------------------------------------------------------------------------------------------------------

PreethamSkyModel::PreethamSkyModel() {}

// -----------------------------------------------------------------------------------------------------------------------------------

PreethamSkyModel::~PreethamSkyModel() {}

// -----------------------------------------------------------------------------------------------------------------------------------

bool PreethamSkyModel::Initialize()
{
    return true;
}

// -----------------------------------------------------------------------------------------------------------------------------------

void PreethamSkyModel::Update()
{
    assert(m_turbidity >= 1);

    const float sunTheta = std::acos(std::clamp(m_direction.y, 0.f, 1.f));

    // A.2 Skylight Distribution Coefficients and Zenith Values: compute Perez distribution coefficients
    A = vec3(-0.0193, -0.0167, 0.1787) * m_turbidity + vec3(-0.2592, -0.2608, -1.4630);
    B = vec3(-0.0665, -0.0950, -0.3554) * m_turbidity + vec3(0.0008, 0.0092, 0.4275);
    C = vec3(-0.0004, -0.0079, -0.0227) * m_turbidity + vec3(0.2125, 0.2102, 5.3251);
    D = vec3(-0.0641, -0.0441, 0.1206) * m_turbidity + vec3(-0.8989, -1.6537, -2.5771);
    E = vec3(-0.0033, -0.0109, -0.0670) * m_turbidity + vec3(0.0452, 0.0529, 0.3703);

    // A.2 Skylight Distribution Coefficients and Zenith Values: compute zenith color
    Z.x = zenith_chromacity(vec4(0.00166, -0.00375, 0.00209, 0),
                            vec4(-0.02903, 0.06377, -0.03202, 0.00394),
                            vec4(0.11693, -0.21196, 0.06052, 0.25886),
                            sunTheta,
                            m_turbidity);
    Z.y = zenith_chromacity(vec4(0.00275, -0.00610, 0.00317, 0),
                            vec4(-0.04214, 0.08970, -0.04153, 0.00516),
                            vec4(0.15346, -0.26756, 0.06670, 0.26688),
                            sunTheta,
                            m_turbidity);
    Z.z = zenith_luminance(sunTheta, m_turbidity);
    Z.z *= 1000; // conversion from kcd/m^2 to cd/m^2

    // 3.2 Skylight Model: pre-divide zenith color by distribution denominator
    Z.x /= perez(0, sunTheta, A.x, B.x, C.x, D.x, E.x);
    Z.y /= perez(0, sunTheta, A.y, B.y, C.y, D.y, E.y);
    Z.z /= perez(0, sunTheta, A.z, B.z, C.z, D.z, E.z);

    // For low dynamic range simulation, normalize luminance to have a fixed value for sun
    if(m_normalized_sun_y)
        Z.z = m_normalized_sun_y / perez(sunTheta, 0, A.z, B.z, C.z, D.z, E.z);
}

// -----------------------------------------------------------------------------------------------------------------------------------

void PreethamSkyModel::SetRenderUniforms(std::shared_ptr<sputnik::graphics::glcore::Shader> shader_program)
{
    shader_program->Bind();
    sputnik::graphics::glcore::Uniform<ramanujan::Vector3>::Set(shader_program->GetUniform("direction"),
                                                                {m_direction.x, m_direction.y, m_direction.z});

    sputnik::graphics::glcore::Uniform<ramanujan::Vector3>::Set(shader_program->GetUniform("p_A"), {A.x, A.y, A.z});
    sputnik::graphics::glcore::Uniform<ramanujan::Vector3>::Set(shader_program->GetUniform("p_B"), {B.x, B.y, B.z});
    sputnik::graphics::glcore::Uniform<ramanujan::Vector3>::Set(shader_program->GetUniform("p_C"), {C.x, C.y, C.z});
    sputnik::graphics::glcore::Uniform<ramanujan::Vector3>::Set(shader_program->GetUniform("p_D"), {D.x, D.y, D.z});
    sputnik::graphics::glcore::Uniform<ramanujan::Vector3>::Set(shader_program->GetUniform("p_E"), {E.x, E.y, E.z});
    sputnik::graphics::glcore::Uniform<ramanujan::Vector3>::Set(shader_program->GetUniform("p_Z"), {Z.x, Z.y, Z.z});
}

// -----------------------------------------------------------------------------------------------------------------------------------
} // namespace sputnik::graphics::api
