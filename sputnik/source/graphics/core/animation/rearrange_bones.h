#pragma once

#include "pch.h"
#include "skeleton.h"
#include "animation_clip.h"
#include "graphics/core/geometry/mesh.h"

namespace sputnik::graphics::core
{

typedef std::map<int, int> BoneMap;

BoneMap RearrangeSkeleton(Skeleton& skeleton);
void    RearrangeMesh(Mesh& mesh, BoneMap& bone_map);
void    RearrangeClip(AnimationClip& clip, BoneMap& bone_map);
void    RearrangeFastClip(FastAnimationClip& clip, BoneMap& bone_map);

} // namespace sputnik::graphics::core