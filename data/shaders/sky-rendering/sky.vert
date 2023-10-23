#version 460 core

uniform mat4 inv_view_projection;

out vec3 ps_in_tex_coord;

void main()
{
	const vec3 vertices[4] = vec3[4](vec3(-1.0f, -1.0f, 1.0f),
										vec3( 1.0f, -1.0f, 1.0f),
										vec3(-1.0f,  1.0f, 1.0f),
										vec3( 1.0f,  1.0f, 1.0f));

	vec4 clip_pos = vec4(vertices[gl_VertexID].xy, -1.0, 1.0);
	vec4 view_pos  = inv_view_projection * clip_pos;

	vec3 dir = vec3(view_pos);//vec3(inv_view * vec4(view_pos.x, view_pos.y, -1.0, 0.0));
	dir = normalize(dir);

	ps_in_tex_coord = dir;

	gl_Position = vec4(vertices[gl_VertexID], 1.0f);
}