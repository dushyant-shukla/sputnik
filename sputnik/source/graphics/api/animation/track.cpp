#include "pch.h"
#include "track.h"
#include "track_helpers.inl"

namespace sputnik::api::animation
{

/**
 * The Track class is templated. However, it is not meant to be used outside the animation system. Therefore, adding the
 * template definitions here. This makes the compiler generate code for the templates.
 */
template Track<float, 1>;
template Track<ramanujan::Vector3, 3>;
template Track<ramanujan::Quaternion, 4>;

template <typename T, unsigned int SIZE>
Track<T, SIZE>::Track() : m_interpolation(InterpolationType::LINEAR)
{
}

template <typename T, unsigned int SIZE>
float Track<T, SIZE>::GetStartTime()
{
    return m_frames[0].m_time;
}

template <typename T, unsigned int SIZE>
float Track<T, SIZE>::GetEndTime()
{
    return m_frames[m_frames.size() - 1].m_time;
}

template <typename T, unsigned int SIZE>
T Track<T, SIZE>::Sample(float time, bool looping)
{
    switch(m_interpolation)
    {
    case sputnik::api::animation::InterpolationType::CONSTANT:
        return SampleConstant(time, looping);

    case sputnik::api::animation::InterpolationType::LINEAR:
        return SampleLinear(time, looping);

    case sputnik::api::animation::InterpolationType::CUBIC:
        return SampleCubic(time, looping);

    default:
        return SampleCubic(time, looping);
    }
}

template <typename T, unsigned int SIZE>
KeyFrame<SIZE>& Track<T, SIZE>::operator[](size_t index)
{
    return m_frames[index];
}

template <typename T, unsigned int SIZE>
void Track<T, SIZE>::Resize(size_t new_size)
{
    return m_frames.resize(new_size);
}

template <typename T, unsigned int SIZE>
unsigned int Track<T, SIZE>::GetSize() const
{
    return static_cast<unsigned int>(m_frames.size());
}

template <typename T, unsigned int SIZE>
InterpolationType Track<T, SIZE>::GetInterpolation()
{
    return m_interpolation;
}

template <typename T, unsigned int SIZE>
void Track<T, SIZE>::SetInterpolation(InterpolationType interpolation_type)
{
    m_interpolation = interpolation_type;
}

template <typename T, unsigned int SIZE>
T Track<T, SIZE>::Hermite(float time, const T& p1, const T& s1, const T& p2, const T& s2)
{
    float tt  = time * time;
    float ttt = tt * time;

    // The second point might need to be negated during the neighborhood check
    T _p2 = p2;
    Neighborhood(p1, _p2);

    float h1 = 2.0f * ttt - 3.0f * tt + 1.0f;
    float h2 = -2.0f * ttt + 3.0f * tt;
    float h3 = ttt - 2.0f * tt + time;
    float h4 = ttt - tt;

    T result = (p1 * h1) + (_p2 * h2) + (s1 * h3) + (s2 * h4);
    return AdjustHermiteResult(result); // normalize the result
}

template <typename T, unsigned int SIZE>
int Track<T, SIZE>::GetKeyFrameIndex(float time, bool looping)
{
    unsigned int size = (unsigned int)m_frames.size();
    if(size <= 1)
    {
        return -1;
    }

    const float start_time = GetStartTime();
    // The first step is to adjust the time value. This will depend on whether the track is looping or not.
    // If the track is sampled as looping, the input time needs to be adjusted so that it falls between the start and
    // end frames.
    if(looping)
    {
        const float end_time = GetEndTime();
        float       duration = end_time - start_time;

        if(duration < 0.0f)
        {
            std::cout << "Warning! Track has a negative duration!";
            duration = fabsf(duration);
        }

        // Since the track is looping, time needs to be adjusted to a valid range. Therefore, make time relative to the
        // duration.
        time = fmodf(time - start_time, duration);
        if(time < 0.0f)
        {
            // if now time is negative, add the duration of the track
            time += duration;
        }
        time += start_time; // add the start time back
    }
    else // Track is not looping, then
    {
        // When the time value less than the start time, frame index must clamp to 0.
        if(time <= start_time)
        {
            return 0;
        }

        // For a time value greater than the second-to-last frame, frame index
        // should clamp to the second-to-last frame'e index.
        // Why second-to-last?
        // The Sample function always needs a current and a next frame. The next frame is found by adding 1 to the
        // result of this method.
        if(time >= m_frames[size - 2].m_time)
        {
            return (int)size - 2;
        }
    }

    // At this point, the time value is in a valid range
    // Loop through all the frames in reverse, this will give us the frames with times closest to the lookup time value
    // first
    for(int i = (int)size - 1; i >= 0; --i)
    {
        // Return the index of the first frame with time less than the lookup time value
        if(time >= m_frames[i].m_time)
        {
            return i;
        }
    }
    return -1; // invalid, we should not reach here
}

/**
 * This method keeps the animation sampling time in a valid range. It is intended to be called when the playback time of
 * an animation changes.
 */
template <typename T, unsigned int SIZE>
float Track<T, SIZE>::AdjustTimeToFitTrack(float time, bool looping)
{
    size_t size = m_frames.size();
    if(size <= 1)
    {
        return 0.0f;
    }

    const float start_time = GetStartTime();
    const float end_time   = GetEndTime();
    const float duration   = end_time - start_time;
    if(duration <= 0.0f)
    {
        return 0.0f;
    }

    if(looping)
    {
        time = fmodf(time - start_time, duration);
        if(time < 0.0f)
        {
            time += duration;
        }
        time += start_time;
    }
    else
    {
        // If the track does not loop, clamp the lookup time value to the first and last frame times.
        if(time <= start_time)
        {
            time = start_time;
        }
        else if(time >= end_time)
        {
            time = end_time;
        }
    }

    return time;
}

/**
 * Following methods are important as they help in casting the floating point arrays stored in Frame class into the
 * data-type that the Frame class represents. For example, Frame<3> is cast to Vector3. These methods are intended to be
 * used when sampling a Track class.
 */
template <>
float Track<float, 1>::Cast(float* value)
{
    return value[0];
}
template <>
ramanujan::Vector3 Track<ramanujan::Vector3, 3>::Cast(float* values)
{
    return {values[0], values[1], values[2]};
}
template <>
ramanujan::Quaternion Track<ramanujan::Quaternion, 4>::Cast(float* values)
{
    ramanujan::Quaternion result(values[0], values[1], values[2], values[3]);
    return ramanujan::Normalized(result);
}

/**
 * Performs constant(step) sampling. Constant sampling is often used for things such as visibility flags, where it
 * makes sense for the value of a variable to change from one frame to the next without any real interpolation.
 */
template <typename T, unsigned int SIZE>
T Track<T, SIZE>::SampleConstant(float time, bool looping)
{
    int frame_index = GetKeyFrameIndex(time, looping);
    if(frame_index < 0 || frame_index >= static_cast<int>(m_frames.size())) // check for validity of the frame
    {
        return T();
    }

    return Cast(&m_frames[frame_index].m_value[0]); // Cast the value of the frame to the correct type
}

template <typename T, unsigned int SIZE>
T Track<T, SIZE>::SampleLinear(float time, bool looping)
{
    int current_frame_index = GetKeyFrameIndex(time, looping);
    if(current_frame_index < 0 || current_frame_index >= static_cast<int>(m_frames.size() - 1))
    {
        return T();
    }

    int next_frame_index = current_frame_index + 1;

    float lookup_track_time  = AdjustTimeToFitTrack(time, looping);
    float current_frame_time = m_frames[current_frame_index].m_time;
    float delta_time         = m_frames[next_frame_index].m_time - current_frame_time;

    if(delta_time <= 0.0f)
    {
        return T();
    }

    float t = (lookup_track_time - current_frame_time) / delta_time;

    T start = Cast(&m_frames[current_frame_index].m_value[0]);
    T end   = Cast(&m_frames[next_frame_index].m_value[0]);

    return Interpolate(start, end, t);
}

template <typename T, unsigned int SIZE>
T Track<T, SIZE>::SampleCubic(float time, bool looping)
{
    int current_frame_index = GetKeyFrameIndex(time, looping);
    if(current_frame_index < 0 || current_frame_index >= static_cast<int>(m_frames.size() - 1))
    {
        return T();
    }

    int next_frame_index = current_frame_index + 1;

    float lookup_track_time  = AdjustTimeToFitTrack(time, looping);
    float current_frame_time = m_frames[current_frame_index].m_time;
    float delta_time         = m_frames[next_frame_index].m_time - current_frame_time;

    if(delta_time <= 0.0f)
    {
        return T();
    }

    float t = (lookup_track_time - current_frame_time) / delta_time;

    T point_1 = Cast(&m_frames[current_frame_index].m_value[0]);

    // We cannot use Cast() here as it would normalize a quaternion which would ruin the slope. We want a raw copy here.
    T slope_1;
    memcpy(&slope_1, m_frames[current_frame_index].m_out_tangents, SIZE * sizeof(float));
    slope_1 = slope_1 * delta_time;

    T point_2 = Cast(&m_frames[next_frame_index].m_value[0]);
    T slope_2;
    memcpy(&slope_2, m_frames[next_frame_index].m_in_tangents, SIZE * sizeof(float));
    slope_2 = slope_2 * delta_time;

    return Hermite(t, point_1, slope_1, point_2, slope_2);
}

} // namespace sputnik::api::animation
