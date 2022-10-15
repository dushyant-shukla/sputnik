#pragma once

#include "pch.h"
#include "frame.h"
#include "interpolation.h"
#include "quaternion.h"
#include "vector3.h"

namespace sputnik::api::animation
{

/**
 * This structure represents an animation track. An animation track is a collection of keyframes in an animation.
 * Interpolating a Track returns the data-type of the track (type parameter: scalar, vector, quaternion). A track must
 * have atleast two keyframes to interpolate between.
 *
 * For skinned mesh animation, the animation tracks always animate the joint transforms (T). However, animation tracks
 * can be used to animate other values in a game as well, such as intensity of a light or switching between two
 * dimensional sprites for a flipbook effect.
 *
 * @param T The quantity being interpolated through a constant, linear or a cubic curve, namely a scalar, vector3 or a
 * quaternion.
 * @param SIZE The component size of the type of value being interpolated (scalar(1), a vector3(3) or a quaternion(4).
 */
template <typename T, unsigned int SIZE>
class Track
{

public:
    Track();
    ~Track() = default;

    void          Resize(unsigned int new_size);
    unsigned int  GetSize();
    Interpolation GetInterpolation();
    void          SetInterpolation(Interpolation interpolation);
    float         GetStartTime();
    float         EndTime();

    /**
     * This method samples the track at a given time. The booleann flag represents whether the track is looping or not.
     */
    T Sample(float time, bool looping);

    /**
     * The subscript operator returns a reference to the Frame at an index.
     */
    Frame<SIZE>& operator[](unsigned int index);

protected:
    /**
     * A track can constitute a constant, linear, or a cubic curve. This method handles interpolation for Track with a
     * constant curve.
     */
    T SampleConstant(float time, bool looping);

    /**
     * A track can constitute a constant, linear, or a cubic curve. This method handles interpolation for Track with a
     * linear curve.
     */
    T SampleLinear(float time, bool looping);

    /**
     * A track can constitute a constant, linear, or a cubic curve. This method handles interpolation for Track with a
     * cubic curve.
     */
    T SampleCubic(float time, bool looping);

    /**
     * This method evaluates a Hermite spline.
     */
    T Hermite(float time, const T& p1, const T& s1, const T& p2, const T& s2);

    /**
     * This method retrives the frame index for a given time. This is the last frame right before the request time.
     */
    int FrameIndex(float time, bool looping);

    /**
     * This function takes in a time value that is outside the range of the track, and adjusts it to be a valid time on
     * the track.
     */
    float AdjustTimeToFitTrack(float time, bool loop);

    /**
     * A utility method for casting an array of floating point values (data inside a frame) to the template
     * parameter-type of the track. This function will have a specialization for each type of track.
     */
    T Cast(float* value);

protected:
    std::vector<Frame<SIZE>> m_frames;
    Interpolation            m_interpolation;
};

/**
 * Common Track types
 */
typedef Track<float, 1>                 ScalarTrack;
typedef Track<ramanujan::Vector3, 3>    VectorTrack;
typedef Track<ramanujan::Quaternion, 4> QuaternionTrack;

} // namespace sputnik::api::animation
