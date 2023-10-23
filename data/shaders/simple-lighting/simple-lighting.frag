#version 460 core

layout(location = 0) in vec3 fs_normal;
layout(location = 1) in vec3 fs_frag_position;
// layout(location = 2) in vec2 fs_uv;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

uniform vec3 eye_position;

layout(location = 0) out vec4 frag_color;

void main() {
    vec3 ambient_color = light.ambient * material.ambient;

    vec3 to_light = normalize(light.position - fs_frag_position);
    vec3 normal = normalize(fs_normal);
    float diffuse_strength = max(dot(normal, to_light), 0.0);
    // float diffuse_strength = max(dot(normal, -light.position), 0.0);  // for sun's direction
    vec3 diffuse_color = light.diffuse * (diffuse_strength * material.diffuse);

    vec3 view_direction = normalize(eye_position - fs_frag_position);
    vec3 light_reflection_direction = reflect(-to_light, normal);
    // vec3 light_reflection_direction = reflect(light.position, normal); // for sun's direction
    float specular_strength = pow(max(dot(view_direction, light_reflection_direction), 0.0), material.shininess * 128.0);
    vec3 specular_color = light.specular * (specular_strength * material.specular);

    frag_color = vec4(ambient_color + diffuse_color + specular_color, 1.0);
}