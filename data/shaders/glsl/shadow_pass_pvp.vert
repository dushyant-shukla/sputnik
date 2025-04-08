// Shadow pass vertex shader

#version 460 core

struct VertexData
{
    vec3 position;
    vec3 normal;
    vec2 uv;
};

layout(std430, binding = 0) restrict readonly buffer VertexBuffer
{
    VertexData vertices[];
};

layout(std140, binding = 2) uniform ShadowPassBuffer
{
    uniform mat4 light_projection;
    uniform mat4 light_view;
   // uniform vec3 light_position;
    //uniform vec3 light_direction;
};

vec3 getPosition(int vertex_index)
{
    return vertices[vertex_index].position;
}

uniform mat4 model;

void main()
{
    vec3 position = getPosition(gl_VertexID);
    gl_Position = light_projection * light_view * model * vec4(position, 1.0);
    // gl_Position = light_projection * light_view * vec4(position, 1.0);
}