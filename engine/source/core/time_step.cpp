#include "pch.h"
#include "time_step.h"

namespace sputnik::core
{
TimeStep::TimeStep(float time) : m_time(time) {}

TimeStep::operator float() const
{
    return m_time;
}

float TimeStep::GetSeconds() const
{
    return m_time;
}

float TimeStep::GetMilliSeconds() const
{
    return m_time * 1e3f;
}
} // namespace sputnik::core