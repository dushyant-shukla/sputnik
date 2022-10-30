#pragma once

#include "core/time_step.h"

namespace sputnik::core::layer
{

class Layer
{

public:
    Layer(const std::string& name);
    virtual ~Layer();

    virtual void OnAttach();
    virtual void OnDetach();
    virtual void OnUpdate(const TimeStep& time_step);
    virtual void OnEvent();
    virtual void OnUpdateUI(const core::TimeStep& time_step);

protected:
    const std::string& GetName() const;

protected:
    std::string m_name;
};

} // namespace sputnik::core::layer