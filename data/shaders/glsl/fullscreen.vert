// Application code must invoke the shader like the following:
// m_fs_quad_shader->Bind();                                  // Bind the fullscreen quad shader
// glActiveTexture(GL_TEXTURE0);                              // Activate texture unit 0
// glBindTexture(GL_TEXTURE_2D, m_texture_id);                // Bind the texture
// glUniform1i(m_fs_quad_shader->GetUniform("s_texture"), 0); // Set the sampler uniform to texture unit 0
// glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);                     // Draw the fullscreen quad
// m_fs_quad_shader->Unbind();                                // Unbind the fullscreen quad shader

#version 460 core

layout(location = 0) out vec2 uv;

void main()
{
    const vec2 positions[4] = vec2[](
        vec2(-1, -1),
        vec2(+1, -1),
        vec2(-1, +1),
        vec2(+1, +1)
    );
    const vec2 coords[4] = vec2[](
        vec2(0, 0),
        vec2(1, 0),
        vec2(0, 1),
        vec2(1, 1)
    );

    uv = coords[gl_VertexID];
    gl_Position = vec4(positions[gl_VertexID], 0.0, 1.0);
}
