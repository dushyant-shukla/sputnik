#pragma once

#include "core/time_step.h"

namespace sputnik::core
{

class Layer
{

public:
    Layer(const std::string& name);
    virtual ~Layer();

    virtual void OnAttach();
    virtual void OnDetach();
    virtual void OnPreUpdate(const TimeStep& time_step);
    virtual void OnUpdate(const TimeStep& time_step);
    virtual void OnPostUpdate(const TimeStep& time_step);
    virtual void OnEvent();
    virtual void OnPreUpdateUI(const TimeStep& time_step);
    virtual void OnUpdateUI(const core::TimeStep& time_step);
    virtual void OnPostUpdateUI(const TimeStep& time_step);

protected:
    const std::string& GetName() const;

protected:
    std::string m_name;
};

} // namespace sputnik::core