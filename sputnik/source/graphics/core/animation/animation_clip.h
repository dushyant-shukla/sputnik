#pragma once

#include "pch.h"
#include "animation_track.h"
#include "pose.h"

namespace sputnik::graphics::core
{

/**
 * An animation clip contains the animation for all the animated joints in a pose over time. This class is used to
 * sample animations and generate poses to display.
 *
 * An animation clip always transforms the same set of joints. Therefore, there is not need to reset the pose to the
 * bind pose during sampling in each frame. However, when switching animations, there is no guarantee if the two clips
 * will animated the same tracks. Therefore, it becomes necessary to reset the transform hierarchy to the bind pose when
 * switching animation clips.
 */
template <typename ANIMATION_TRACK_TYPE>
class TAnimationClip
{

public:
    TAnimationClip();
    ~TAnimationClip() = default;

    unsigned int GetJointIdAtTrackIndex(unsigned int track_index);
    void         SetJointIdAtTrackIndex(unsigned int track_index, unsigned int joint_id);
    unsigned int GetNumJoints();

    /**
     * This method samples an animation clip for a given time into the Pose passed in as a reference. It returns a time
     * value.
     */
    float Sample(Pose& out_pose, float in_time); // Todo: what is the return value?

    /**
     * The [] operator returns a transform track for the specified joint. If no track exists for a given joint, one is
     * created and returned.
     */
    ANIMATION_TRACK_TYPE& operator[](unsigned int joint_id);

    /**
     * This method calculates the duration of an animation clip by determining the start and end times of an animation
     * clip.
     */
    void RecalculateDuration();

    std::string& GetName();
    void         SetName(const std::string& name);
    float        GetDuration();
    float        GetStartTime();
    float        GetEndTime();
    bool         IsLooping();
    void         SetLooping(bool looping);

protected:
    // protected methods

protected:
    std::vector<ANIMATION_TRACK_TYPE> m_tracks; // An animation clip is a collection of animation tracks, where each
                                                // track describes the motion of one joint over time.
    std::string m_name;
    float       m_start_time;
    float       m_end_time;
    bool        m_looping;

private:
    float AdjustTimeToFitRange(float in_time);
};

typedef TAnimationClip<AnimationTrack>     AnimationClip;
typedef TAnimationClip<FastAnimationTrack> FastAnimationClip;

FastAnimationClip OptimizeClip(AnimationClip& input);

} // namespace sputnik::graphics::core