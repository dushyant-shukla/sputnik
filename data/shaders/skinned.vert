#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 weights;
layout(location = 4) in ivec4 joints;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 pose[120];
uniform mat4 inv_bind_pose[120];

layout (location = 0) out VS_OUT
{
    vec3 frag_position;
    vec2 uv;
    vec3 normal;
} vs_out;

void main()
{
    mat4 skin = (pose[joints.x] * inv_bind_pose[joints.x]) * weights.x;
    skin += (pose[joints.y] * inv_bind_pose[joints.y]) * weights.y;
    skin += (pose[joints.z] * inv_bind_pose[joints.z]) * weights.z;
    skin += (pose[joints.w] * inv_bind_pose[joints.w]) * weights.w;

    gl_Position = projection * view * model * skin * vec4(position, 1.0);
    vs_out.frag_position = vec3(model * skin * vec4(position, 1.0));

    //mat3 normal_matrix = mat3(transpose(inverse(model)));
    //vs_out.normal = normal_matrix * normal;

    vs_out.normal = vec3(model * skin * vec4(normal, 1.0));
    vs_out.uv = uv;
}