#pragma once

#include <main/windows/windows_main.h>
#include <main/application.h>
#include <graphics/glcore/shader.h>
#include <graphics/glcore/texture.h>
#include <graphics/glcore/index_buffer.h>
#include <graphics/glcore/vertex_attribute.h>
#include <vector2.h>
#include <vector3.h>
#include <graphics/glcore/debug_draw.h>
#include <graphics/animation/track.h>
#include <graphics/animation/interpolation.h>

#include <memory>

namespace sputnik::demos
{

class CurvePlotting : public sputnik::main::Application
{

public:
    CurvePlotting();
    ~CurvePlotting();

    virtual void Initialize() override;
    virtual void Update(float delta_time) override;
    virtual void Render(float aspect_ratio) override;
    virtual void Shutdown() override;

private:
    sputnik::animation::ScalarFrame MakeFrame(float time, float value);
    sputnik::animation::ScalarFrame MakeFrame(float time, float in, float value, float out);
    sputnik::animation::VectorFrame MakeFrame(float time, const ramanujan::Vector3& value);
    sputnik::animation::VectorFrame
    MakeFrame(float time, const ramanujan::Vector3& in, const ramanujan::Vector3& value, const ramanujan::Vector3& out);
    sputnik::animation::QuaternionFrame MakeFrame(float time, const ramanujan::Quaternion& value);
    sputnik::animation::QuaternionFrame MakeFrame(float                        time,
                                                  const ramanujan::Quaternion& in,
                                                  const ramanujan::Quaternion& out,
                                                  const ramanujan::Quaternion& value);
    sputnik::animation::ScalarTrack     MakeScalarTrack(sputnik::animation::Interpolation interp, int numFrames, ...);
    sputnik::animation::VectorTrack     MakeVectorTrack(sputnik::animation::Interpolation interp, int numFrames, ...);
    sputnik::animation::QuaternionTrack
    MakeQuaternionTrack(sputnik::animation::Interpolation interp, int numFrames, ...);

private:
    std::vector<sputnik::animation::ScalarTrack> mScalarTracks;
    std::vector<bool>                            mScalarTracksLooping;

    sputnik::glcore::DebugDraw* mScalarTrackLines;
    sputnik::glcore::DebugDraw* mHandleLines;
    sputnik::glcore::DebugDraw* mHandlePoints;
    sputnik::glcore::DebugDraw* mReferenceLines;
};

} // namespace sputnik::demos