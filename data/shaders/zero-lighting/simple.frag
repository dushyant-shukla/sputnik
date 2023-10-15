#version 460 core

layout (location = 0) in VS_OUT
{
    vec3 frag_position;
    vec2 uv;
} fs_in;

uniform sampler2D diffuse;

layout(location = 0) out vec4 frag_color;

void main()
{
    vec4 Kd = texture(diffuse, fs_in.uv);
    frag_color = Kd;
}