#include "pch.h"
#include "rearrange_bones.h"

#include <vector4.h>

namespace sputnik::graphics::core
{

/**
 * This function rearranges the bones in a skeleton so that the skeleton can take advantage of the optimized version of
 * Pose::GetMatrixPallete().
 */
BoneMap RearrangeSkeleton(Skeleton& skeleton)
{
    const Pose& rest_pose  = skeleton.GetRestPose();
    const Pose& bind_pose  = skeleton.GetBindPose();
    size_t      num_joints = rest_pose.GetNumJoints();
    if(num_joints == 0)
    {
        return BoneMap();
    }
    std::vector<std::vector<int>> hierarchy(num_joints);
    std::list<int>                process;
    for(size_t i = 0; i < num_joints; ++i)
    {
        int parent = rest_pose.GetParent(i);
        if(parent >= 0)
        {
            hierarchy[parent].push_back(static_cast<int>(i));
        }
        else
        {
            process.push_back(static_cast<int>(i));
        }
    }

    BoneMap map_forward;
    BoneMap map_backward;

    int index = 0;
    while(process.size() > 0)
    {
        int current = *process.begin();
        process.pop_front();
        std::vector<int>& children     = hierarchy[current];
        size_t            num_children = children.size();
        for(unsigned int i = 0; i < num_children; ++i)
        {
            process.push_back(children[i]);
        }

        map_forward[index]    = current;
        map_backward[current] = index;
        ++index;
    }

    map_forward[-1]  = -1;
    map_backward[-1] = -1;

    Pose                     new_rest_pose(num_joints);
    Pose                     new_bind_pose(num_joints);
    std::vector<std::string> new_names(num_joints);

    for(unsigned int i = 0; i < num_joints; ++i)
    {
        int joint = map_forward[i];
        new_rest_pose.SetLocalTransform(i, rest_pose.GetLocalTransform(joint));
        new_bind_pose.SetLocalTransform(i, bind_pose.GetLocalTransform(joint));
        new_names[i] = skeleton.GetJointName(joint);

        int parent = map_backward[bind_pose.GetParent(joint)];
        new_rest_pose.SetParent(i, parent);
        new_bind_pose.SetParent(i, parent);
    }

    skeleton.Set(new_rest_pose, new_bind_pose, new_names);
    return map_backward;
} // End of RearrangeSkeleton()

void RearrangeMesh(Mesh& mesh, BoneMap& bone_map)
{
    std::vector<ramanujan::IVector4>& influences = mesh.GetInfluences();
    unsigned int                      size       = static_cast<unsigned int>(influences.size());

    for(unsigned int i = 0; i < size; ++i)
    {
        influences[i].x = bone_map[influences[i].x];
        influences[i].y = bone_map[influences[i].y];
        influences[i].z = bone_map[influences[i].z];
        influences[i].w = bone_map[influences[i].w];
    }

    mesh.ResetOpenglBuffersToBindPose();
}

void RearrangeClip(AnimationClip& clip, BoneMap& bone_map)
{
    unsigned int size = clip.GetNumJoints();
    for(unsigned int i = 0; i < size; ++i)
    {
        int          joint     = static_cast<int>(clip.GetJointIdAtTrackIndex(i));
        unsigned int new_joint = static_cast<unsigned int>(bone_map[joint]);
        clip.SetJointIdAtTrackIndex(i, new_joint);
        int a = 10;
    }
}

void RearrangeFastClip(FastAnimationClip& clip, BoneMap& bone_map)
{
    unsigned int size = clip.GetNumJoints();

    for(unsigned int i = 0; i < size; ++i)
    {
        int          joint    = (int)clip.GetJointIdAtTrackIndex(i);
        unsigned int newJoint = (unsigned int)bone_map[joint];
        clip.SetJointIdAtTrackIndex(i, newJoint);
    }
}

} // namespace sputnik::graphics::core