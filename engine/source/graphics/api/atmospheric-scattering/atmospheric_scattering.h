#pragma once

#include "core/layers/layer.h"

#include <string>

namespace sputnik::graphics::api
{

using namespace sputnik::core;

class AtmosphericScatteringLayer : public Layer
{

public:
    AtmosphericScatteringLayer();
    virtual ~AtmosphericScatteringLayer();

    virtual void OnAttach();
    virtual void OnDetach();
    virtual void OnUpdate(const sputnik::core::TimeStep& time_step);
    virtual void OnUpdateUI(const sputnik::core::TimeStep& time_step);

private:
};

} // namespace sputnik::graphics::api
