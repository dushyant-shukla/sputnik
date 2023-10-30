#version 460 core

layout(location = 0) out vec2 uv;
layout(location = 0) out vec4 frag_color;

uniform sampler2D s_texture;

void main()
{
	frag_color = vec4(texture(s_texture, uv).rgb, 1.0);
}
