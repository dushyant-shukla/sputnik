#include "pch.h"
#include "fast_track.h"

namespace sputnik::graphics::core
{

/**
 * The Track class is templated. However, it is not meant to be used outside the animation system. Therefore, adding the
 * template definitions here. This makes the compiler generate code for the templates.
 */
template FastTrack<float, 1>;
template FastTrack<ramanujan::Vector3, 3>;
template FastTrack<ramanujan::Quaternion, 4>;

template <typename T, unsigned int SIZE>
void FastTrack<T, SIZE>::UpdateIndexLookupTable()
{
    int num_frames = static_cast<int>(this->m_frames.size());
    if(num_frames <= 1) // a valid track has atleast two tracks
    {
        return;
    }

    float duration = this->GetEndTime() - this->GetStartTime();

    // The number of frames to be sampled depends on the context as different games have different requirements. Here we
    // are setting a limit of 60 samples per second, i.e., we have 60 frames for every second of animation.
    unsigned int num_samples = static_cast<unsigned int>(duration * 60.0f);
    m_sampled_frames.resize(num_samples);

    // For each sample, we need to find the time of the sample along the track.
    for(unsigned int i = 0; i < num_samples; ++i)
    {
        float t    = (float)i / (float)(num_samples - 1); // normalize the iteration index
        float time = t * duration + this->GetStartTime(); // multiply the normalized iteration index by the animation
                                                          // duration, and add the start time of the animation to it.

        // Now, we need to find the frame index for each sample. To do this, we need to find the frame that comes before
        // the sampling time for the current sample, and record it in the m_sampled_frames.
        unsigned int frame_index = 0;

        // Todo: Note: This can be further optimized by remembering the last used index of the innner loop iteration
        // index (j) since, on each outer loop iteration (i), the frame index only increases.
        for(int j = num_frames - 1; j >= 0; --j)
        {
            if(time >= this->m_frames[j].m_time)
            {
                frame_index = (unsigned int)j;

                // If the frame index is the last frame, return the frame index right before the last index. Keep in
                // mind, that the GetKeyFrameIndex() must never return index of the last frame.
                if((int)frame_index >= num_frames - 2)
                {
                    frame_index = num_frames - 2;
                }
            }
            break;
        }
        m_sampled_frames[i] = frame_index;
    }
}

template <>
FastTrack<float, 1> FastTrack<float, 1>::OptimizeTrack(const Track<float, 1>& input);

template <>
FastTrack<ramanujan::Vector3, 3>
FastTrack<ramanujan::Vector3, 3>::OptimizeTrack(const Track<ramanujan::Vector3, 3>& input);

template <>
FastTrack<ramanujan::Quaternion, 4>
FastTrack<ramanujan::Quaternion, 4>::OptimizeTrack(const Track<ramanujan::Quaternion, 4>& input);

template <typename T, unsigned int SIZE>
FastTrack<T, SIZE> OptimizeTrack(const Track<T, SIZE>& input)
{
    FastTrack<T, SIZE> result;
    result.SetInterpolation(input.GetInterpolation());
    unsigned int size = input.GetSize();
    result.Resize(size);
    for(unsigned int i = 0; i < size; ++i)
    {
        result[i] = input[i];
    }
    result.UpdateIndexLookupTable();
    return result;
}

template <typename T, unsigned int SIZE>
int FastTrack<T, SIZE>::GetKeyFrameIndex(float time, bool looping)
{
    std::vector<KeyFrame<SIZE>>& frames = this->m_frames;
    unsigned int                 size   = (unsigned int)frames.size();

    // a valid track must have atleast two key frames
    if(size <= 1)
    {
        return -1;
    }

    float start_time = this->m_frames[0].m_time;
    float end_time   = this->m_frames[size - 1].m_time;
    float duration   = end_time - start_time;

    if(looping)
    {
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
    else // Track is not looping, then clamp to the first or next to last frame
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
        if(time >= frames[size - 2].m_time)
        {
            return (int)size - 2;
        }
    }

    // At this point, the time value is in a valid range.
    float        t           = time / duration; // normalized sampling time
    unsigned int num_samples = static_cast<unsigned int>(duration * 60.0f);
    unsigned int index =
        static_cast<unsigned int>(t * (float)num_samples); // Required frame index is given by multiplying the
                                                           // normalized sampling time by the number of sampled.

    // If the frame is invalid, return -1
    if(index >= m_sampled_frames.size())
    {
        return -1;
    }
    return (int)m_sampled_frames[index];
}

} // namespace sputnik::graphics::core