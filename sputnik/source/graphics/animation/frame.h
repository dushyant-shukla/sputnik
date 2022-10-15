#pragma once

namespace sputnik::animation
{

/**
 * The structure is used for storing keyframes in an animation track. An animation track is a collection of the
 * keyframes.
 */
template <unsigned int SIZE>
struct Frame
{
    float m_value[SIZE];
    float m_in_tangents[SIZE];
    float m_out_tangents[SIZE];
    float m_time;
};

typedef Frame<1> ScalarFrame;
typedef Frame<3> VectorFrame;
typedef Frame<4> QuaternionFrame;

} // namespace sputnik::renderer::animation