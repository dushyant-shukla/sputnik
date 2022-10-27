#pragma once

#include <platform/win32/win32_window.h>
#include <main/application.h>
#include <graphics/glcore/shader.h>
#include <graphics/glcore/texture.h>
#include <graphics/glcore/index_buffer.h>
#include <graphics/glcore/vertex_attribute.h>
#include <vector2.h>
#include <vector3.h>

#include <memory>

namespace sputnik::demos
{

class Sandbox : public sputnik::main::Application
{

public:
    Sandbox();
    ~Sandbox();

    virtual void Initialize() override;
    virtual void Update(float delta_time) override;
    virtual void Render(float aspect_ratio) override;
    virtual void Shutdown() override;

private:
    std::shared_ptr<sputnik::glcore::Shader>                              m_shader;
    std::shared_ptr<sputnik::glcore::VertexAttribute<ramanujan::Vector3>> m_vertex_positions;
    std::shared_ptr<sputnik::glcore::VertexAttribute<ramanujan::Vector3>> m_vertex_normals;
    std::shared_ptr<sputnik::glcore::VertexAttribute<ramanujan::Vector2>> m_vertex_uv;
    std::shared_ptr<sputnik::glcore::IndexBuffer>                         m_index_buffer;
    std::shared_ptr<sputnik::glcore::Texture>                             m_texture;
    float                                                                 m_rotation;
};

} // namespace sputnik::demos