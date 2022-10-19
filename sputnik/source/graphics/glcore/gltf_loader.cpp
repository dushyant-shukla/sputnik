#include "pch.h"
#include "gltf_loader.h"
#include "graphics/api/animation/track.h"

#include <transform.h>

namespace sputnik
{

using namespace api::animation;
using namespace ramanujan;

namespace gltf
{

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
        // local_transform.position = ramanujan::Vector3(&node.translation[0]);
        local_transform.position = ramanujan::Vector3(node.translation[0], node.translation[1], node.translation[2]);
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

void GetScalarValues(std::vector<float>& out, unsigned int component_count, const cgltf_accessor& in_accessor)
{
    out.resize(in_accessor.count * component_count);
    for(cgltf_size i = 0; i < in_accessor.count; ++i)
    {
        cgltf_accessor_read_float(&in_accessor, i, &out[i * component_count], component_count);
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
        int       base_index = static_cast<int>(i * comp_count);
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

void MeshFromAttribute(sputnik::api::Mesh& out_mesh,
                       cgltf_attribute&    attribute,
                       Skin*               skin,
                       Node*               nodes,
                       size_t        node_count)
{
    cgltf_attribute_type attrib_type = attribute.type;
    cgltf_accessor&      accessor    = *attribute.data;

    unsigned int component_count = 0;
    if(accessor.type == cgltf_type_vec2)
    {
        component_count = 2;
    }
    else if(accessor.type == cgltf_type_vec3)
    {
        component_count = 3;
    }
    else if(accessor.type == cgltf_type_vec4)
    {
        component_count = 4;
    }

    std::vector<float> values;
    GetScalarValues(values, component_count, accessor);
    size_t accessor_count = accessor.count;

    std::vector<ramanujan::Vector3>&  positions  = out_mesh.GetPosition();
    std::vector<ramanujan::Vector3>&  normals    = out_mesh.GetNormal();
    std::vector<ramanujan::Vector2>&  uv         = out_mesh.GetTexCoord();
    std::vector<ramanujan::IVector4>& influences = out_mesh.GetInfluences();
    std::vector<ramanujan::Vector4>&  weights    = out_mesh.GetWeights();

    for(size_t i = 0; i < accessor_count; ++i)
    {
        size_t index = i * component_count;
        switch(attrib_type)
        {
        case cgltf_attribute_type_position:
            positions.push_back(ramanujan::Vector3(values[index + 0], values[index + 1], values[index + 2]));
            break;
        case cgltf_attribute_type_texcoord:
            uv.push_back(ramanujan::Vector2(values[index + 0], values[index + 1]));
            break;
        case cgltf_attribute_type_weights:
            weights.push_back(
                ramanujan::Vector4(values[index + 0], values[index + 1], values[index + 2], values[index + 3]));
            break;
        case cgltf_attribute_type_normal:
        {
            ramanujan::Vector3 normal = ramanujan::Vector3(values[index + 0], values[index + 1], values[index + 2]);
            if(ramanujan::LengthSq(normal) < ramanujan::constants::EPSILON)
            {
                normal = ramanujan::Vector3(0.0f, 1.0f, 0.0f);
            }
            normals.push_back(ramanujan::Normalized(normal));
            break;
        }
        case cgltf_attribute_type_joints:
        {
            ramanujan::IVector4 joints(static_cast<int>(values[index + 0] + 0.5f),
                                       static_cast<int>(values[index + 1] + 0.5f),
                                       static_cast<int>(values[index + 2] + 0.5f),
                                       static_cast<int>(values[index + 3] + 0.5f));

            joints.x = GetNodeIndex(skin->joints[joints.x], nodes, node_count);
            joints.y = GetNodeIndex(skin->joints[joints.y], nodes, node_count);
            joints.z = GetNodeIndex(skin->joints[joints.z], nodes, node_count);
            joints.w = GetNodeIndex(skin->joints[joints.w], nodes, node_count);

            joints.x = std::max(0, joints.x);
            joints.y = std::max(0, joints.y);
            joints.z = std::max(0, joints.z);
            joints.w = std::max(0, joints.w);

            influences.push_back(joints);
            break;
        }
        }
    }
} // End of MeshFromAttribute()

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

void GltfLoader::LoadAnimationClips(Data*                                                data,
                                    std::vector<sputnik::api::animation::AnimationClip>& out_animation_clips)
{
    size_t num_clips = data->animations_count;
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
            int                      node_id = helper::GetNodeIndex(target, data->nodes, num_nodes);
            if(channel.target_path == cgltf_animation_path_type_translation)
            {
                VectorTrack& track = out_animation_clips[i][node_id].GetPositionTrack();
                helper::CreateTrackFromChannel<Vector3, 3>(track, channel);
            }
            else if(channel.target_path == cgltf_animation_path_type_scale)
            {
                VectorTrack& track = out_animation_clips[i][node_id].GetScaleTrack();
                helper::CreateTrackFromChannel<Vector3, 3>(track, channel);
            }
            else if(channel.target_path == cgltf_animation_path_type_rotation)
            {
                QuaternionTrack& track = out_animation_clips[i][node_id].GetRotationTrack();
                helper::CreateTrackFromChannel<Quaternion, 4>(track, channel);
            }
        } // channel loop ends

        out_animation_clips[i].RecalculateDuration();
    } // clip loop ends
} // LoadAnimationClips() ends

std::vector<std::string> GltfLoader::LoadJointNanes(Data* data)
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

/**
 * .
 */
sputnik::api::animation::Pose GltfLoader::LoadRestPose(Data* data) // Todo: LoadJointNames can be done here
{
    size_t                        joint_count = data->nodes_count;
    sputnik::api::animation::Pose rest_pose(joint_count);

    for(size_t joint_index = 0; joint_index < joint_count; ++joint_index)
    {
        Node*                node            = &(data->nodes[joint_index]);
        ramanujan::Transform local_transform = helper::GetNodeLocalTransform(*node);
        rest_pose.SetLocalTransform(joint_index, local_transform);
        int parent_node_index = helper::GetNodeIndex(node->parent, data->nodes, joint_count);
        rest_pose.SetParent(joint_index, parent_node_index);
    }
    return rest_pose;
}

// Todo:: Double check the work
sputnik::api::animation::Pose GltfLoader::LoadBindPose(Data* data)
{
    // Phase#1: Loading rest pose, and default values for world space bind pose for joints
    sputnik::api::animation::Pose rest_pose            = LoadRestPose(data);
    size_t                        num_joints_rest_pose = rest_pose.GetNumJoints();

    // Convert each transform in the rest pose into a world space transform. This ensures that is a skin didn't provide
    // an inverse bind pose matrix of a joint, a good default value is available.
    std::vector<ramanujan::Transform> world_bind_pose(num_joints_rest_pose);
    for(size_t rest_pose_joint_index = 0; rest_pose_joint_index < num_joints_rest_pose; ++rest_pose_joint_index)
    {
        world_bind_pose[rest_pose_joint_index] = rest_pose.GetGlobalTransform(rest_pose_joint_index);
    }

    // Phase#2:
    // Loop through each skinned mesh in the gltf file.
    size_t num_skins = data->skins_count;
    for(size_t skin_index = 0; skin_index < num_skins; ++skin_index)
    {
        Skin* current_skin = &(data->skins[skin_index]);

        // Read the inverse bind matrices accessor into a large vector of floating point variables. This vector will
        // contain 16 * NUM_JOINTS elements since each matrix is 4x4 matrix.
        std::vector<float> inv_bind_accessor;
        helper::GetScalarValues(inv_bind_accessor, 16, *current_skin->inverse_bind_matrices);

        // For every joint in the skinned mesh, invert the inverse bind pose matrix of each joint. Inverting the inverse
        // bind pose matrix results in the bind pose matrix.
        size_t num_joints_in_current_skin = current_skin->joints_count;
        for(size_t joint_index_current_skin = 0; joint_index_current_skin < num_joints_in_current_skin;
            ++joint_index_current_skin)
        {
            float* matrix =
                &(inv_bind_accessor[joint_index_current_skin * 16]); // Read the inverse bind matrix of the joint
            ramanujan::Matrix4   inv_bind_matrix = ramanujan::Matrix4(matrix);
            ramanujan::Matrix4   bind_matrix     = ramanujan::Inverse(inv_bind_matrix);
            ramanujan::Transform bind_transform  = ToTransform(bind_matrix); // This is a world space transform

            // Set the world space bind transform in the world_bind_pose
            Node* joint_node             = current_skin->joints[joint_index_current_skin];
            int   joint_index            = helper::GetNodeIndex(joint_node, data->nodes, num_joints_rest_pose);
            world_bind_pose[joint_index] = bind_transform;
        }
    }

    // Phase#3: Convert the world bind pose to a regular bind pose
    sputnik::api::animation::Pose bind_pose = rest_pose;
    for(size_t i = 0; i < num_joints_rest_pose; ++i)
    {
        ramanujan::Transform current_transform = world_bind_pose[i];
        int                  parent_node_index = bind_pose.GetParent(i);
        if(parent_node_index >= 0)
        {
            // Bringing the world space transform into local space transform relative to the parent node.
            ramanujan::Transform parent_transform = world_bind_pose[parent_node_index];
            current_transform = ramanujan::Combine(ramanujan::Inverse(parent_transform), current_transform);
        }
        bind_pose.SetLocalTransform(i, current_transform);
    }
    return bind_pose;
} // End of LoadBindPose()

sputnik::api::animation::Skeleton GltfLoader::LoadSkeleton(Data* data)
{
    return sputnik::api::animation::Skeleton(LoadRestPose(data), LoadBindPose(data), LoadJointNanes(data));
}

std::vector<sputnik::api::Mesh> GltfLoader::LoadMeshes(Data* data)
{
    std::vector<sputnik::api::Mesh> result;
    Node*                           nodes      = data->nodes;
    size_t                          node_count = data->nodes_count;

    for(size_t i = 0; i < node_count; ++i)
    {
        Node* node = &nodes[i];
        if(node->mesh == 0 || node->skin == 0)
        {
            continue;
        }

        size_t num_primitives = node->mesh->primitives_count;
        for(size_t j = 0; j < num_primitives; ++j)
        {
            result.push_back(sputnik::api::Mesh());
            sputnik::api::Mesh& mesh = result[result.size() - 1];

            cgltf_primitive* primitive = &node->mesh->primitives[j];

            size_t attribute_count = primitive->attributes_count;
            for(size_t k = 0; k < attribute_count; ++k)
            {
                cgltf_attribute* attribute = &primitive->attributes[k];
                helper::MeshFromAttribute(mesh, *attribute, node->skin, nodes, node_count);
            }

            // Check for indices, update index buffer for the mesh
            if(primitive->indices != 0)
            {
                size_t                     ic      = primitive->indices->count;
                std::vector<unsigned int>& indices = mesh.GetIndices();
                indices.resize(ic);

                for(size_t k = 0; k < ic; ++k)
                {
                    indices[k] = cgltf_accessor_read_index(primitive->indices, k);
                }
            }

            mesh.UpdateOpenglBuffers();
        }
    }

    return result;
}

} // namespace gltf

} // namespace sputnik