#version 460 core

in vec2 out_uv;

uniform sampler2D color_texture;
uniform sampler2D opacity_texture;

layout(location = 0) out vec4 frag_color;

void main(){
    // frag_color = color;
    vec3 cloth_color = texture(color_texture, out_uv).xyz;
    vec4 opacity_color = texture(opacity_texture, out_uv);
    frag_color = vec4(cloth_color, opacity_color.y);
}