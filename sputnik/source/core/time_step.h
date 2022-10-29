#pragma once

namespace sputnik::core
{

class TimeStep
{

public:
    TimeStep(float time = 0.0f);

    operator float() const;

    float GetSeconds() const;
    float GetMilliSeconds() const;

private:
    float m_time;
};

} // namespace sputnik::core