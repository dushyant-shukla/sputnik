#pragma once

#include "SkyModel.h"

namespace sputnik::graphics::api
{

// A Practical Analytic Model for Daylight (A. J. Preetham, Peter Shirley, Brian Smits)
class PreethamSkyModel : public SkyModel
{
public:
    PreethamSkyModel();
    ~PreethamSkyModel();

    bool Initialize() override;
    void Update() override;
    void SetRenderUniforms(std::shared_ptr<OglShaderProgram>) override;

private:
    vec3 A, B, C, D, E, Z;
};

} // namespace sputnik::graphics::api
