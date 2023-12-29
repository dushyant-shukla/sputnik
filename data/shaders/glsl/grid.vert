//
#version 460 core

#include <grid_parameters.glsl>

layout (location=0) out vec2 uv;

layout(std140, binding = 0) uniform PerFrameData {
    uniform mat4 projection;
    uniform mat4 view;
    vec3 camera_position;
};

// struct Vertex
// {
// 	float p[3];
// 	float n[3];
// 	float tc[2];
// };

// layout(std430, binding = 1) restrict readonly buffer Vertices
// {
// 	Vertex in_Vertices[];
// };

// layout(std430, binding = 2) restrict readonly buffer Matrices
// {
// 	mat4 in_ModelMatrices[];
// };

void main()
{
	int idx = indices[gl_VertexID];
	vec3 position = pos[idx] * gridSize;

	gl_Position = projection * view * vec4(position, 1.0);
	uv = position.xz;
}
