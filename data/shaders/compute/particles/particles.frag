#version 460 core

//layout(location = 0) in vec3 out_position;

uniform vec4 color;

layout(location = 0) out vec4 frag_color;

void main() {
    frag_color = color;
}