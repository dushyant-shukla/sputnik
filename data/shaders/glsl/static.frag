#version 460 core

layout(location = 0) out vec4 frag_color;

layout(location = 0) in VS_OUT {
    vec3 normal;
    vec2 uv;
} fs_in;

// layout(std140, binding = 1) uniform LightData {
//     vec3 position;
//     float constant;
//     float linear;
//     float quadratic;
//     vec3 ambient;
//     vec3 diffuse;
//     vec3 specular;
// } light_data;

#include <blinn_phong_lighting.glsl>

void main() {
    frag_color = vec4(0.24, 0.88, 0.51, 0.8);
}