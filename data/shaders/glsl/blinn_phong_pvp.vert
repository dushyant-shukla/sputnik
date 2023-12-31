#version 460 core

struct VertexData
{
    vec3 position;
    vec3 normal;
    vec2 uv;
};

layout(std140, binding = 0) uniform PerFrameData {
    uniform mat4 projection;
    uniform mat4 view;
    vec3 camera_position;
};

layout(std430, binding = 2) restrict readonly buffer VertexBuffer
{
    VertexData vertices[];
};

uniform mat4 model;
uniform mat4 normal_matrix;

layout(location = 0) out VS_OUT {
    vec3 normal;
    vec2 uv;
    vec3 eye_position;
    vec3 frag_position;
} vs_out;

vec3 getPosition(int vertex_index)
{
    return vertices[vertex_index].position;
}

vec3 getNormal(int vertex_index)
{
    return vertices[vertex_index].normal;
}

vec2 getUV(int vertex_index)
{
    return vertices[vertex_index].uv;
}

void main() {
    vec3 position = getPosition(gl_VertexID);
    vec3 normal = getNormal(gl_VertexID);
    vec2 uv = getUV(gl_VertexID);
    vs_out.normal = vec3(normal_matrix * vec4(normal, 1.0));
    vs_out.uv = uv;
    vs_out.eye_position = camera_position;
    vs_out.frag_position = vec3(model * vec4(position, 1.0));
    gl_Position = projection * view * model * vec4(position, 1.0);
}
