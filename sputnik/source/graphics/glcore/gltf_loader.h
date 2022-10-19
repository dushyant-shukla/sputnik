#pragma once

#include "core/core.h"

#include <cgltf.h>
#include "graphics/api/animation/pose.h"
#include "graphics/api/animation/animation_clip.h"

// Todo: Major refactoring required. This class needs a major upgrade.
namespace sputnik::gltf
{
using Node = cgltf_node;
using Data = cgltf_data;

class GltfLoader
{

public:
    NON_INSTATIABLE(GltfLoader);

    static Data*                         LoadFile(const char* path);
    static void                          FreeFile(cgltf_data* data);
    static std::vector<std::string>      LoadJointNanes(Data* data);
    static sputnik::api::animation::Pose LoadRestPose(Data* data);
    static void                          LoadAnimationClips(Data*                                                data,
                                                            std::vector<sputnik::api::animation::AnimationClip>& out_animation_clips);
};

} // namespace sputnik::gltf