#version 460 core

layout(location = 0) out vec4 frag_color;
//out vec4 frag_color;

in VS_OUT
{
	vec2 uv;
	vec3 normal;
} fs_in;

struct Material {
	sampler2D texture_diffuse;
};
uniform Material material;

uniform vec3 direction;

void main()
{
	vec3 diffuse = texture(material.texture_diffuse, fs_in.uv * 10).rgb;
	//vec3 color = diffuse;
	vec3 color = dot(fs_in.normal, -direction) * diffuse;
	frag_color = vec4(color, 1.0);
	//frag_color = vec4(1.0, 0.0, 0.0, 1.0);
}