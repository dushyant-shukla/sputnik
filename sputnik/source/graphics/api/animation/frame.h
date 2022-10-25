#pragma once

namespace sputnik::api::animation
{

/**
 * The structure is used for storing keyframes in an animation track. An animation track is a collection of the
 * keyframes.
 */
template <unsigned int SIZE>
struct KeyFrame
{
    float m_value[SIZE];
    float m_in_tangents[SIZE];
    float m_out_tangents[SIZE];
    float m_time;
};

typedef KeyFrame<1> ScalarFrame;
typedef KeyFrame<3> VectorFrame;
typedef KeyFrame<4> QuaternionFrame;

} // namespace sputnik::api::animation