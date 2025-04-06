#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

layout(std140, binding = 0) uniform GlobalUbo
{
	mat4 view;
	mat4 projection;
	mat4 inv_view;
	mat4 inv_projection;
	mat4 inv_view_projection;
	vec4 view_pos;
};

layout(std140, binding = 1) uniform ObjectUBO
{
	mat4 model_matrix;
	mat3 normal_matrix;
};

out VS_OUT
{
	vec2 uv;
	vec3 normal;
} vs_out;


void main()
{
	vs_out.uv = uv;
	mat3 norma_m = mat3(model_matrix);
	vs_out.normal = normalize(norma_m * normal); // normal_matrix gives incorrect result even when I am sending same data from cpu
	gl_Position = projection * view * model_matrix * vec4(position, 1.0);
}