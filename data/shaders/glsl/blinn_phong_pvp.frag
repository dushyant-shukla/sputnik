#version 460 core

layout(location = 0) out vec4 frag_color;

layout(location = 0) in VS_OUT {
    vec3 normal;
    vec2 uv;
    vec3 eye_position;
    vec3 frag_position;
} fs_in;

layout(std140, binding = 1) uniform LightData {
    vec3 light_position;
    vec3 light_ambient;
    vec3 light_diffuse;
    vec3 light_specular;
    float light_constant;
    float light_linear;
    float light_quadratic;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    float alpha;
    sampler2D diffuse_texture;
    sampler2D specular_texture;
};
uniform Material material;

#include <blinn_phong_lighting.glsl>

void main() {

    vec3 Kd = texture(material.diffuse_texture, fs_in.uv).rgb;
    vec3 Ks = texture(material.specular_texture, fs_in.uv).rgb;

    // vec3 ambient_color = light_ambient * material.ambient * Kd;
    vec3 ambient_color = light_ambient * 0.1 * Kd;

    vec3 to_light = normalize(light_position - fs_in.frag_position);
    vec3 normal = normalize(fs_in.normal);

    vec3 diffuse_color = light_diffuse * calculateDiffuseComponent(to_light, normal, material.diffuse * Kd);

    vec3 view_direction = normalize(fs_in.eye_position - fs_in.frag_position);
    vec3 specular_color = light_specular * calculateSpecularComponent(view_direction, to_light, normal, material.specular * Ks, material.shininess);

    float attenuation = calculateAttenuation(light_constant, light_linear, light_quadratic, length(light_position - fs_in.frag_position));

    vec4 light_intensity = vec4(ambient_color + diffuse_color + specular_color, 1.0) / attenuation;
    frag_color = light_intensity + vec4(1.0, -1.0, -1.0, 1.0);
    // frag_color.x = 1.0;
    // frag_color.y = 0.0;
    // frag_color.z = 0.0;
    // frag_color.a = 0.05;
}