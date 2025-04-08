// Shadow pass vertex shader

#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 uv;

layout(std140, binding = 2) uniform ShadowPassBuffer
{
    uniform mat4 light_projection;
    uniform mat4 light_view;
    // uniform vec3 light_position;
    // uniform vec3 light_direction;
};

uniform mat4 model;

void main()
{
    gl_Position = light_projection * light_view * model * vec4(position, 1.0);
    // gl_Position = light_projection * light_view * vec4(position, 1.0);
}