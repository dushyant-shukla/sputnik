#include "pch.h"
#include "gltf_loader.h"
#include "graphics/api/animation/pose.h"
#include "graphics/api/animation/track.h"
#include "graphics/api/animation/animation_clip.h"

#include <transform.h>

namespace sputnik
{

using namespace api::animation;
using namespace ramanujan;

namespace gltf
{

using Node = cgltf_node;
using Data = cgltf_data;

namespace helper
{

/**
 * This method gets the local transform of a gltf node.
 *
 * \param node The gltf node
 * \return Local transform of a gltf node
 */
ramanujan::Transform GetNodeLocalTransform(Node& node)
{
    ramanujan::Transform local_transform;

    if(node.has_matrix)
    {
        ramanujan::Matrix4 transform(&node.matrix[0]);
        local_transform = ramanujan::ToTransform(transform);
    }

    if(node.has_translation)
    {
        local_transform.position = ramanujan::Vector3(&node.translation[0]);
    }

    if(node.has_rotation)
    {
        local_transform.rotation =
            ramanujan::Quaternion(node.rotation[0], node.rotation[1], node.rotation[2], node.rotation[3]);
    }

    if(node.has_scale)
    {
        local_transform.scale = ramanujan::Vector3(&node.scale[0]);
    }

    return local_transform;
}

/**
 * This method gets the the index of a gltf node from an array. It performs a linear lookup through all the nodes in
 * gltf file.
 *
 * \param target The target node
 * \param all_nodes A pointer to all the nodes in a gltf file
 * \param num_nodes Number of nodes in a gltf file
 * \return Index of the node
 */
int GetNodeIndex(Node* target, Node* all_nodes, size_t num_nodes)
{
    if(target == 0)
    {
        return -1;
    }
    for(unsigned int i = 0; i < num_nodes; ++i)
    {
        if(target == &all_nodes[i])
        {
            return static_cast<int>(i);
        }
    }
    return -1;
}

/**
 * .
 */
Pose LoadRestPose(Data* data)
{
    size_t joint_count = data->nodes_count;
    Pose         rest_pose(joint_count);

    for(size_t joint_index = 0; joint_index < joint_count; ++joint_index)
    {
        Node*     node            = &(data->nodes[joint_index]);
        Transform local_transform = GetNodeLocalTransform(*node);
        rest_pose.SetLocalTransform(joint_index, local_transform);
        int parent_node_index = GetNodeIndex(node->parent, data->nodes, joint_count);
        rest_pose.SetParent(joint_index, parent_node_index);
    }

    return rest_pose;
}

std::vector<std::string> LoadJointNanes(Data* data)
{
    size_t                   joint_count = data->nodes_count;
    std::vector<std::string> joint_names(joint_count, "Not Set");

    for(size_t joint_index = 0; joint_index < joint_count; ++joint_index)
    {
        Node* node = &(data->nodes[joint_index]);
        if(node->name == 0)
        {
            joint_names[joint_index] = "EMPTY NODE";
        }
        else
        {
            joint_names[joint_index] = node->name;
        }
    }
    return joint_names;
}

void GetScalarValues(std::vector<float>& out, unsigned int comp_count, const cgltf_accessor& in_accessor)
{
    out.resize(in_accessor.count * comp_count);
    for(cgltf_size i = 0; i < in_accessor.count; ++i)
    {
        cgltf_accessor_read_float(&in_accessor, i, &out[i * comp_count], comp_count);
    }
}

template <typename T, unsigned int N>
void CreateTrackFromChannel(Track<T, N>& result, const cgltf_animation_channel& channel)
{
    cgltf_animation_sampler& sampler          = *channel.sampler;
    Interpolation            interpolation    = Interpolation::CONSTANT;
    bool                     is_sampler_cubic = false;
    if(sampler.interpolation == cgltf_interpolation_type_linear)
    {
        interpolation = Interpolation::LINEAR;
    }
    else if(sampler.interpolation == cgltf_interpolation_type_cubic_spline)
    {
        is_sampler_cubic = true;
        interpolation    = Interpolation::CUBIC;
    }

    result.SetInterpolation(interpolation);

    std::vector<float> time;
    GetScalarValues(time, 1, *sampler.input);

    std::vector<float> val;
    GetScalarValues(val, N, *sampler.output);

    size_t num_frames = sampler.input->count;
    size_t comp_count = val.size() / time.size();
    result.Resize(num_frames);

    for(size_t i = 0; i < num_frames; ++i)
    {
        int       base_index = static_cast<int>(1 * comp_count);
        Frame<N>& frame      = result[i];
        int       offset     = 0;

        frame.m_time = time[i];

        for(int comp = 0; comp < N; ++comp)
        {
            frame.m_in_tangents[comp] = is_sampler_cubic ? val[base_index + offset++] : 0.0f;
        }

        for(int comp = 0; comp < N; ++comp)
        {
            frame.m_value[comp] = val[base_index + offset++];
        }

        for(int comp = 0; comp < N; ++comp)
        {
            frame.m_out_tangents[comp] = is_sampler_cubic ? val[base_index + offset++] : 0.0f;
        }
    }
}

void LoadAnimationClips(Data* data, std::vector<AnimationClip>& out_animation_clips)
{
    size_t       num_clips = data->animations_count;
    size_t num_nodes = data->nodes_count;

    out_animation_clips.resize(num_clips);

    for(size_t i = 0; i < num_clips; ++i)
    {
        out_animation_clips[i].SetName(data->animations[i].name);
        size_t num_channels = data->animations[i].channels_count;
        for(size_t j = 0; j < num_channels; ++j)
        {
            cgltf_animation_channel& channel = data->animations[i].channels[j];
            cgltf_node*              target  = channel.target_node;
            int                      node_id = GetNodeIndex(target, data->nodes, num_nodes);
            if(channel.target_path == cgltf_animation_path_type_translation)
            {
                VectorTrack& track = out_animation_clips[i][node_id].GetPositionTrack();
                CreateTrackFromChannel<Vector3, 3>(track, channel);
            }
            else if(channel.target_path == cgltf_animation_path_type_scale)
            {
                VectorTrack& track = out_animation_clips[i][node_id].GetScaleTrack();
                CreateTrackFromChannel<Vector3, 3>(track, channel);
            }
            else if(channel.target_path == cgltf_animation_path_type_rotation)
            {
                QuaternionTrack& track = out_animation_clips[i][node_id].GetRotationTrack();
                CreateTrackFromChannel<Quaternion, 4>(track, channel);
            }
        } // channel loop ends

        out_animation_clips[i].RecalculateDuration();
    } // clip loop ends
} // LoadAnimationClips() ends

} // namespace helper

Data* GltfLoader::LoadFile(const char* path)
{
    cgltf_options options;
    memset(&options, 0, sizeof(cgltf_options));
    Data* data = NULL;

    cgltf_result result = cgltf_parse_file(&options, path, &data); // load the glTF data from file.
    if(result != cgltf_result_success)
    {
        // cout << "Could not load: " << path << "\n";
        return 0;
    }

    result = cgltf_load_buffers(&options, data, path); // load any external buffer data
    if(result != cgltf_result_success)
    {
        cgltf_free(data);
        // cout << "Could not load: " << path << "\n";
        return 0;
    }

    result = cgltf_validate(data); // validate the gltf file just loaded
    if(result != cgltf_result_success)
    {
        cgltf_free(data);
        // assert() // "Invalid file"
        return 0;
    }
    return data;
}

void GltfLoader::FreeFile(Data* data)
{
    if(data == 0)
    {
        // assert(); // cannot free null data
    }
    else
    {
        cgltf_free(data);
    }
}

} // namespace gltf

} // namespace sputnik