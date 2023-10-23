#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
// layout(location = 2) in vec2 uv;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 normal_matrix;

layout(location = 0) out vec3 fs_normal;
layout(location = 1) out vec3 fs_frag_position;
layout(location = 2) out vec2 fs_uv;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    fs_frag_position = vec3(model * vec4(position, 1.0)); // vertex position in world space
    fs_normal = vec3(normal_matrix * vec4(normal, 1.0));
    // fs_uv = uv;
}