#version 460 core

#include <grid_parameters.glsl>
#include <grid_calculations.glsl>


layout (location=0) in vec2 uv;
layout (location=0) out vec4 out_FragColor;

void main()
{
	out_FragColor = gridColor(uv);
};
