#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

//layout(std140, binding = 0) uniform PerFrameData {
//    uniform mat4 projection;
//    uniform mat4 view;
//    uniform mat4 model;
//    uniform mat4 normal_matrix;
//} per_frame_data;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 normal_matrix;

layout(location = 0) out VS_OUT {
    vec3 normal;
    vec2 uv;
} vs_out;

void main() {
    // vs_out.normal = vec3(per_frame_data.normal_matrix * vec4(normal, 1.0));
    vs_out.normal = vec3(normal_matrix * vec4(normal, 1.0));
    vs_out.uv = uv;
    // gl_Position = per_frame_data.projection * per_frame_data.view * per_frame_data.model * vec4(position, 1.0);
    gl_Position = projection * view * model * vec4(position, 1.0);
}
