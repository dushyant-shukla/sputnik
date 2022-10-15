#include "curve_plotting.h"

#include <quaternion.h>
#include <matrix4.h>
#include <camera_transforms.h>
#include <graphics/glcore/uniform.h>
#include <graphics/api/renderer.h>
#include <iostream>
#include <cstdarg>

namespace sputnik
{

sputnik::main::Application* sputnik::main::CreateApplication()
{
    return new demos::CurvePlotting();
}

namespace demos
{

CurvePlotting::CurvePlotting() {}

CurvePlotting::~CurvePlotting() {}

void CurvePlotting::Initialize()
{
    mScalarTracks.push_back(
        MakeScalarTrack(sputnik::animation::Interpolation::LINEAR, 2, MakeFrame(0.0f, 0.0f), MakeFrame(1.0f, 1.0f)));
    mScalarTracksLooping.push_back(false);

    mScalarTracks.push_back(
        MakeScalarTrack(sputnik::animation::Interpolation::LINEAR, 2, MakeFrame(0.0f, 0.0f), MakeFrame(0.5f, 1.0f)));
    mScalarTracksLooping.push_back(false);

    mScalarTracks.push_back(MakeScalarTrack(sputnik::animation::Interpolation::LINEAR,
                                            3,
                                            MakeFrame(0.25f, 0.0f),
                                            MakeFrame(0.5f, 1.0f),
                                            MakeFrame(0.75f, 0.0f)));
    mScalarTracksLooping.push_back(true);

    mScalarTracks.push_back(MakeScalarTrack(sputnik::animation::Interpolation::LINEAR,
                                            3,
                                            MakeFrame(0.25f, 0.0f),
                                            MakeFrame(0.5f, 1.0f),
                                            MakeFrame(0.75f, 0.0f)));
    mScalarTracksLooping.push_back(false);

    sputnik::animation::ScalarTrack stepTrack;
    stepTrack.Resize(11);
    stepTrack.SetInterpolation(sputnik::animation::Interpolation::CONSTANT);
    for(int i = 0; i < 11; ++i)
    {
        float time   = (float)i / 10.0f * 0.5f + 0.25f;
        stepTrack[i] = MakeFrame(time, (i % 2 == 0.0f) ? 0.0f : 1.0f);
    }
    mScalarTracks.push_back(stepTrack);
    mScalarTracks.push_back(stepTrack);
    mScalarTracksLooping.push_back(true);
    mScalarTracksLooping.push_back(false);

    mScalarTracks.push_back(MakeScalarTrack(sputnik::animation::Interpolation::CUBIC,
                                            2,
                                            MakeFrame(0.25f, 0.676221f, 0.0f, 0.676221f),
                                            MakeFrame(0.75f, 4.043837f, 1.0f, 4.043837f)));
    mScalarTracksLooping.push_back(false);

    mScalarTracks.push_back(MakeScalarTrack(sputnik::animation::Interpolation::CUBIC,
                                            3,
                                            MakeFrame(0.25f, 0, 0, 0),
                                            MakeFrame(0.5f, 0, 1, 0),
                                            MakeFrame(0.75f, 0, 0, 0)));
    mScalarTracksLooping.push_back(true);

    mScalarTracks.push_back(MakeScalarTrack(sputnik::animation::Interpolation::CUBIC,
                                            5,
                                            MakeFrame(0.25f, 0, 0, 0),
                                            MakeFrame(0.3833333f, -10.11282f, 0.5499259f, -10.11282f),
                                            MakeFrame(0.5f, 25.82528f, 1, 25.82528f),
                                            MakeFrame(0.6333333f, 7.925411f, 0.4500741f, 7.925411f),
                                            MakeFrame(0.75f, 0, 0, 0)));
    mScalarTracksLooping.push_back(true);

    mScalarTracks.push_back(MakeScalarTrack(sputnik::animation::Interpolation::CUBIC,
                                            5,
                                            MakeFrame(0.25f, 0, 0, 0),
                                            MakeFrame(0.3833333f, 13.25147f, 0.5499259f, -10.11282f),
                                            MakeFrame(0.5f, 10.2405f, 1, -5.545671f),
                                            MakeFrame(0.6333333f, 7.925411f, 0.4500741f, -11.40672f),
                                            MakeFrame(0.75f, 0, 0, 0)));
    mScalarTracksLooping.push_back(true);

    mScalarTrackLines = new sputnik::glcore::DebugDraw();
    mHandlePoints     = new sputnik::glcore::DebugDraw();
    mHandleLines      = new sputnik::glcore::DebugDraw();
    mReferenceLines   = new sputnik::glcore::DebugDraw();

    // Range in both X and Y is 0 to 20

    for(unsigned int i = 0; i < 10; ++i)
    {
        float yPosition = (i * 2.0f) + ((float)i * 0.2f) + 0.1f;
        float height    = 1.8f;

        float left  = 1.0f;
        float right = 14.0;

        mReferenceLines->Push(ramanujan::Vector3(left, yPosition, 0));
        mReferenceLines->Push(ramanujan::Vector3(left, yPosition + height, 0));

        mReferenceLines->Push(ramanujan::Vector3(left, yPosition, 0));
        mReferenceLines->Push(ramanujan::Vector3(right, yPosition, 0));
    }
    mReferenceLines->UpdateOpenGLBuffers();

    // Safe to assume that mScalarTracks has a size of 10

    for(unsigned int i = 0; i < (unsigned int)mScalarTracks.size(); ++i)
    {
        float yPosition = ((9 - i) * 2.0f) + ((float)(9 - i) * 0.2f) + 0.1f;
        float height    = 1.8f;

        float left   = 1.0f;
        float right  = 14.0;
        float xRange = right - left;

        for(unsigned int j = 1; j < 150; ++j)
        { // j goes from 0 to 149
            float thisJNorm = (float)(j - 1) / 149.0f;
            float nextJNorm = (float)(j) / 149.0f;

            float thisX = left + thisJNorm * xRange;
            float nextX = left + nextJNorm * xRange;

            float thisY = mScalarTracks[i].Sample(thisJNorm, mScalarTracksLooping[i]);
            float nextY = mScalarTracks[i].Sample(nextJNorm, mScalarTracksLooping[i]);

            thisY = yPosition + thisY * height;
            nextY = yPosition + nextY * height;

            mScalarTrackLines->Push(ramanujan::Vector3(thisX, thisY, 0.1f));
            mScalarTrackLines->Push(ramanujan::Vector3(nextX, nextY, 0.1f));
        }

        unsigned int numFrames = mScalarTracks[i].GetSize();
        for(unsigned int j = 0; j < numFrames; ++j)
        {
            float thisTime = mScalarTracks[i][j].m_time;
            float thisY    = yPosition + mScalarTracks[i].Sample(thisTime, mScalarTracksLooping[i]) * height;
            float thisX    = left + thisTime * xRange;
            mHandlePoints->Push(ramanujan::Vector3(thisX, thisY, 0.9f));

            if(j > 0)
            {
                float prevY = yPosition + mScalarTracks[i].Sample(thisTime - 0.0005f, mScalarTracksLooping[i]) * height;
                float prevX = left + (thisTime - 0.0005f) * xRange;

                ramanujan::Vector3 thisVec   = ramanujan::Vector3(thisX, thisY, 0.6f);
                ramanujan::Vector3 prevVec   = ramanujan::Vector3(prevX, prevY, 0.6f);
                ramanujan::Vector3 handleVec = thisVec + Normalized(prevVec - thisVec) * 0.75f;

                mHandleLines->Push(thisVec);
                mHandleLines->Push(handleVec);
            }

            if(j < numFrames - 1 && mScalarTracks[i].GetInterpolation() != sputnik::animation::Interpolation::CONSTANT)
            {
                float nextY = yPosition + mScalarTracks[i].Sample(thisTime + 0.0005f, mScalarTracksLooping[i]) * height;
                float nextX = left + (thisTime + 0.0005f) * xRange;

                ramanujan::Vector3 thisVec   = ramanujan::Vector3(thisX, thisY, 0.6f);
                ramanujan::Vector3 nextVec   = ramanujan::Vector3(nextX, nextY, 0.6f);
                ramanujan::Vector3 handleVec = thisVec + Normalized(nextVec - thisVec) * 0.75f;

                mHandleLines->Push(thisVec);
                mHandleLines->Push(handleVec);
            }
        }
    }
    mScalarTrackLines->UpdateOpenGLBuffers();
    mHandleLines->UpdateOpenGLBuffers();
    mHandlePoints->UpdateOpenGLBuffers();
}

void CurvePlotting::Update(float delta_time) {}

void CurvePlotting::Render(float aspect_ratio)
{
    ramanujan::Matrix4 model;
    ramanujan::Matrix4 view =
        ramanujan::LookAt(ramanujan::Vector3(0, 0, 5), ramanujan::Vector3(0, 0, 0), ramanujan::Vector3(0, 1, 0));
    ramanujan::Matrix4 projection = ramanujan::Ortho(0, aspect_ratio * 22.0f, 0, 22, 0.001f, 10);

    ramanujan::Matrix4 mvp = projection * view * model;

    mReferenceLines->Draw(sputnik::glcore::DebugDrawMode::Lines, ramanujan::Vector3(1, 1, 1), mvp);
    mScalarTrackLines->Draw(sputnik::glcore::DebugDrawMode::Lines, ramanujan::Vector3(0, 1, 0), mvp);
    mHandleLines->Draw(sputnik::glcore::DebugDrawMode::Lines, ramanujan::Vector3(1, 0, 0), mvp);
    mHandlePoints->Draw(sputnik::glcore::DebugDrawMode::Points, ramanujan::Vector3(0, 0, 1), mvp);
}

void CurvePlotting::Shutdown()
{
    mScalarTracks.clear();
    mScalarTracksLooping.clear();

    delete mScalarTrackLines;
    delete mHandlePoints;
    delete mHandleLines;
    delete mReferenceLines;
}

sputnik::animation::ScalarFrame CurvePlotting::MakeFrame(float time, float value)
{
    return MakeFrame(time, 0.0f, value, 0.0f);
}

sputnik::animation::ScalarFrame CurvePlotting::MakeFrame(float time, float in, float value, float out)
{
    sputnik::animation::ScalarFrame result;
    result.m_time            = time;
    result.m_in_tangents[0]  = in;
    result.m_value[0]        = value;
    result.m_out_tangents[0] = out;
    return result;
}

sputnik::animation::VectorFrame CurvePlotting::MakeFrame(float time, const ramanujan::Vector3& value)
{
    return MakeFrame(time, ramanujan::Vector3(), value, ramanujan::Vector3());
}

sputnik::animation::VectorFrame CurvePlotting::MakeFrame(float                     time,
                                                         const ramanujan::Vector3& in,
                                                         const ramanujan::Vector3& value,
                                                         const ramanujan::Vector3& out)
{
    sputnik::animation::VectorFrame result;
    result.m_time            = time;
    result.m_in_tangents[0]  = in.x;
    result.m_in_tangents[1]  = in.y;
    result.m_in_tangents[2]  = in.z;
    result.m_value[0]        = value.x;
    result.m_value[1]        = value.y;
    result.m_value[2]        = value.z;
    result.m_out_tangents[0] = out.x;
    result.m_out_tangents[1] = out.y;
    result.m_out_tangents[2] = out.z;
    return result;
}

sputnik::animation::QuaternionFrame CurvePlotting::MakeFrame(float time, const ramanujan::Quaternion& value)
{
    return MakeFrame(time, ramanujan::Quaternion(0, 0, 0, 0), value, ramanujan::Quaternion(0, 0, 0, 0));
}

sputnik::animation::QuaternionFrame CurvePlotting::MakeFrame(float                        time,
                                                             const ramanujan::Quaternion& in,
                                                             const ramanujan::Quaternion& out,
                                                             const ramanujan::Quaternion& value)
{
    sputnik::animation::QuaternionFrame result;
    result.m_time     = time;
    result.m_in_tangents[0]    = in.x;
    result.m_in_tangents[1]    = in.y;
    result.m_in_tangents[2]    = in.z;
    result.m_in_tangents[3]    = in.w;
    result.m_value[0] = value.x;
    result.m_value[1] = value.y;
    result.m_value[2] = value.z;
    result.m_value[3] = value.w;
    result.m_out_tangents[0]   = out.x;
    result.m_out_tangents[1]   = out.y;
    result.m_out_tangents[2]   = out.z;
    result.m_out_tangents[3]   = out.w;
    return result;
}

sputnik::animation::ScalarTrack
CurvePlotting::MakeScalarTrack(sputnik::animation::Interpolation interp, int numFrames, ...)
{
    sputnik::animation::ScalarTrack result;
    result.SetInterpolation(interp);
    result.Resize(numFrames);

    va_list args;
    va_start(args, numFrames);

    for(int i = 0; i < numFrames; ++i)
    {
        result[i] = va_arg(args, sputnik::animation::ScalarFrame);
    }

    va_end(args);

    return result;
}

sputnik::animation::VectorTrack
CurvePlotting::MakeVectorTrack(sputnik::animation::Interpolation interp, int numFrames, ...)
{
    sputnik::animation::VectorTrack result;
    result.SetInterpolation(interp);
    result.Resize(numFrames);

    va_list args;
    va_start(args, numFrames);

    for(int i = 0; i < numFrames; ++i)
    {
        result[i] = va_arg(args, sputnik::animation::VectorFrame);
    }

    va_end(args);

    return result;
}

sputnik::animation::QuaternionTrack
CurvePlotting::MakeQuaternionTrack(sputnik::animation::Interpolation interp, int numFrames, ...)
{
    sputnik::animation::QuaternionTrack result;
    result.SetInterpolation(interp);
    result.Resize(numFrames);

    va_list args;
    va_start(args, numFrames);

    for(int i = 0; i < numFrames; ++i)
    {
        result[i] = va_arg(args, sputnik::animation::QuaternionFrame);
    }

    va_end(args);

    return result;
}
} // namespace demos

} // namespace sputnik