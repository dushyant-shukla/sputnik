#version 460 core

struct DebugData
{
    vec4 position;
};

layout(std430, binding = 0) restrict readonly buffer DebugDataBuffer
{
    DebugData positions[];
};

layout(std140, binding = 0) uniform PerFrameData {
    uniform mat4 projection;
    uniform mat4 view;
    vec3 camera_position;
};

void main()
{
    gl_Position = projection * view * vec4(positions[gl_VertexID].position);
}