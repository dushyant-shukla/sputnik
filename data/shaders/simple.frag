#version 460 core

layout (location = 0) in VS_OUT
{
    vec3 frag_position;
    vec2 uv;
    vec3 normal;
} fs_in;

uniform vec3 light;
uniform sampler2D diffuse;

layout(location = 0) out vec4 frag_color;

vec4 CalculateDiffuseComponent(const in vec3 L, const in vec3 N, const in vec4 Kd)
{
	float diffuse_strength = max(dot(N, L), 0.0f);
	return diffuse_strength * Kd;
}

void main()
{
    vec4 diffuse_color = texture(diffuse, fs_in.uv);
    vec3 n = normalize(fs_in.normal);
    vec3 l = normalize(light);
    //float diffuse_intensity = clamp(dot(n, l), 0, 1);
    float diffuse_intensity = clamp(dot(n, l) + 0.1, 0, 1);
    frag_color = diffuse_color * diffuse_intensity;
}

//void main()
//{
//    vec3 L = normalize(light - fs_in.frag_position);
//    vec3 N = normalize(fs_in.normal);
//    vec4 Kd = texture(diffuse, fs_in.uv);
//    vec4 diffuse_color = CalculateDiffuseComponent(L, N, Kd);
//    frag_color = diffuse_color;
//}