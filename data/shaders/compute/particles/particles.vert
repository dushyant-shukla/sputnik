#version 460 core

layout(location = 0) in vec4 position;

// layout(location = 0) out vec3 out_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // mat4 view_model = view * model;
    // out_position = (view_model * position).xyz;
    gl_Position = projection * view * model * position;
}