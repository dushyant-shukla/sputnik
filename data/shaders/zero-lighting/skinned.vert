#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec4 weights;
layout(location = 3) in ivec4 joints;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 skin_transforms[120];

layout (location = 0) out VS_OUT
{
    vec3 frag_position;
    vec2 uv;
} vs_out;

void main()
{
    mat4 skin = skin_transforms[joints.x] * weights.x;
    skin += skin_transforms[joints.y] * weights.y;
    skin += skin_transforms[joints.z] * weights.z;
    skin += skin_transforms[joints.w] * weights.w;

    gl_Position = projection * view * model * skin * vec4(position, 1.0);

    vs_out.frag_position = vec3(model * skin * vec4(position, 1.0));
    vs_out.uv = uv;
}