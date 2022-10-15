#version 460 core

layout (location = 0) in VS_OUT
{
    vec3 frag_pos;
    vec2 uv;
    vec3 normal;
} fs_in;

uniform vec3 light;
uniform sampler2D diffuse;

layout(location = 0) out vec4 frag_color;

void main()
{
    vec4 diffuse_color = texture(diffuse, fs_in.uv);
    vec3 n = normalize(fs_in.normal);
    vec3 l = normalize(light);
    float diffuse_intensity = clamp(dot(n, l), 0, 1);
    frag_color = diffuse_color * diffuse_intensity;
}