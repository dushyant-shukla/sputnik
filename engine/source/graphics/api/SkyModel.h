#pragma once

#include <vector.hpp>
#include <graphics/glcore/shader.h>

namespace sputnik::graphics::api
{

using namespace ramanujan;
using namespace ramanujan::experimental;

class SkyModel
{
public:
    virtual bool Initialize()                                                                         = 0;
    virtual void Update()                                                                             = 0;
    virtual void SetRenderUniforms(std::shared_ptr<sputnik::graphics::glcore::Shader> shader_program) = 0;

    inline vec3 GetDirection() { return m_direction; }
    inline void SetDirection(vec3 dir) { m_direction = -1 * dir; }

    inline float GetTurbidity() { return m_turbidity; }
    inline void  SetTurbidity(float t) { m_turbidity = t; }

protected:
    vec3  m_direction;
    float m_normalized_sun_y = 1.15f;
    float m_albedo           = 0.1f;
    float m_turbidity        = 4.0f;
};
} // namespace sputnik::graphics::api
