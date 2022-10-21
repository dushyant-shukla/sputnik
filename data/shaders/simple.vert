#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout (location = 0) out VS_OUT
{
    vec3 frag_position;
    vec2 uv;
    vec3 normal;
} vs_out;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    vs_out.frag_position = vec3(model * vec4(position, 1.0));

    //mat3 normal_matrix = mat3(transpose(inverse(model)));
    //vs_out.normal = normal_matrix * normal;

    vs_out.normal = vec3(model * vec4(normal, 1.0));

    vs_out.uv = uv;
}