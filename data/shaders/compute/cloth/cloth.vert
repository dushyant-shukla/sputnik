#version 460 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 uv;

out vec2 out_uv;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    out_uv = uv;
    gl_Position = projection * view * model * position;
}