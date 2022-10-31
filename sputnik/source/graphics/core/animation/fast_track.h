#pragma once

#include "pch.h"
#include "track.h"

namespace sputnik::graphics::core
{

template <typename T, unsigned int SIZE>
class FastTrack : public Track<T, SIZE>
{

public:
    /**
     * This method samples the animation at fixed time intervals and record the frame before the animation time for each
     * interval. This method is intended to be called at the load time.
     */
    void UpdateIndexLookupTable();
    
    FastTrack OptimizeTrack(const Track<T, SIZE>& input);

protected:
    /**
     * This method is responsible for finding the frame right before a given time. It uses a lookup array instead of
     * looping through every frame of the track. This ensures that frame index lookup for all the sampling times have a
     * very similar performance.
     */
    virtual int GetKeyFrameIndex(float time, bool looping) override;

protected:
    /**
     * FastTrack samples an animation track at uniform time intervals. For each time interval, the frame on the left of
     * the play head (the frame right before time) is recorded into this vector.
     */
    std::vector<unsigned int> m_sampled_frames;
};

/**
 * Common FastTrack types
 */
typedef FastTrack<float, 1>                 FastScalarTrack;
typedef FastTrack<ramanujan::Vector3, 3>    FastVectorTrack;
typedef FastTrack<ramanujan::Quaternion, 4> FastQuaternionTrack;

} // namespace sputnik::graphics::core
