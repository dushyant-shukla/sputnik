#pragma once

#include "core/core.h"

#include <cgltf.h>
#include "graphics/core/animation/pose.h"
#include "graphics/core/animation/animation_clip.h"
#include "graphics/core/animation/skeleton.h"
#include "graphics/core/geometry/mesh.h"

// Todo: Major refactoring required. This class needs a major upgrade.
namespace sputnik::gltf
{
using Node = cgltf_node;
using Data = cgltf_data;
using Skin = cgltf_skin;

class GltfLoader
{

public:
    NON_INSTATIABLE(GltfLoader)

    static Data*                                      LoadFile(const char* path);
    static void                                       FreeFile(Data* data);
    static std::vector<std::string>                   LoadJointNanes(Data* data);
    static sputnik::graphics::core::Pose              LoadRestPose(Data* data);
    static void                                       LoadAnimationClips(Data*                                                data,
                                                                         std::vector<sputnik::graphics::core::AnimationClip>& out_animation_clips);
    static sputnik::graphics::core::Pose              LoadBindPose(Data* data);
    static sputnik::graphics::core::Skeleton          LoadSkeleton(Data* data);
    static std::vector<sputnik::graphics::core::Mesh> LoadMeshes(Data* data);
    static std::vector<sputnik::graphics::core::Mesh> LoadStaticMeshes(cgltf_data* data);
};

} // namespace sputnik::gltf