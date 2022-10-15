#include "sandbox.h"

#include <quaternion.h>
#include <matrix4.h>
#include <camera_transforms.h>
#include <graphics/glcore/uniform.h>
#include <graphics/api/renderer.h>

namespace sputnik
{

sputnik::main::Application* sputnik::main::CreateApplication()
{
    return new demos::Sandbox();
}

namespace demos
{

Sandbox::Sandbox() : sputnik::main::Application::Application(), m_rotation(0.0f) {}

Sandbox::~Sandbox() {}

void Sandbox::Initialize()
{
    m_shader =
        std::make_shared<sputnik::glcore::Shader>("../../data/shaders/simple.vert", "../../data/shaders/simple.frag");
    m_texture = std::make_shared<sputnik::glcore::Texture>("../../data/assets/uv.png");

    m_vertex_positions = std::make_shared<sputnik::glcore::VertexAttribute<ramanujan::Vector3>>();
    m_vertex_normals   = std::make_shared<sputnik::glcore::VertexAttribute<ramanujan::Vector3>>();
    m_vertex_uv        = std::make_shared<sputnik::glcore::VertexAttribute<ramanujan::Vector2>>();
    m_index_buffer     = std::make_shared<sputnik::glcore::IndexBuffer>();

    // setting vertex positions counter clockwise
    std::vector<ramanujan::Vector3> positions;
    positions.push_back({-1.0f, -1.0f, 0});
    positions.push_back({1.0f, -1.0f, 0});
    positions.push_back({1.0f, 1.0f, 0});
    positions.push_back({-1.0f, 1.0f, 0});
    m_vertex_positions->Set(positions);

    std::vector<ramanujan::Vector3> normals;
    normals.resize(4, {0.0f, 0.0f, 1.0f});
    m_vertex_normals->Set(normals);

    std::vector<ramanujan::Vector2> uvs;
    uvs.push_back({0.0f, 0.0f});
    uvs.push_back({1.0f, 0.0f});
    uvs.push_back({1.0f, 1.0f});
    uvs.push_back({0.0f, 1.0f});
    m_vertex_uv->Set(uvs);

    std::vector<unsigned int> indices;
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(0);
    m_index_buffer->Set(indices);
}

void Sandbox::Update(float delta_time)
{
    m_rotation += delta_time * 45.0f;
    while(m_rotation > 360.0f)
    {
        m_rotation -= 360.0f;
    }
}

void Sandbox::Render(float aspect_ratio)
{
    ramanujan::Matrix4 projection = ramanujan::Perspective(60.0f, aspect_ratio, 0.01f, 1000.0f);
    ramanujan::Matrix4 view       = ramanujan::LookAt({0.0f, 0.0f, -5.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
    ramanujan::Matrix4 model =
        ramanujan::ToMatrix4(ramanujan::AngleAxis(m_rotation * ramanujan::constants::DEG_TO_RAD, {0.0f, 0.0f, 1.0f}));

    m_shader->Bind();

    m_vertex_positions->Bind(m_shader->GetAttribute("position"));
    m_vertex_normals->Bind(m_shader->GetAttribute("normal"));
    m_vertex_uv->Bind(m_shader->GetAttribute("uv"));

    sputnik::glcore::Uniform<ramanujan::Matrix4>::Set(m_shader->GetUniform("model"), model);
    sputnik::glcore::Uniform<ramanujan::Matrix4>::Set(m_shader->GetUniform("view"), view);
    sputnik::glcore::Uniform<ramanujan::Matrix4>::Set(m_shader->GetUniform("projection"), projection);
    sputnik::glcore::Uniform<ramanujan::Vector3>::Set(m_shader->GetUniform("light"), {0.0f, 0.0f, 1.0f});

    m_texture->Set(m_shader->GetUniform("diffuse"), 0);

    sputnik::glcore::Renderer::Draw((*m_index_buffer.get()), sputnik::glcore::DrawMode::TRIANGLES);

    m_texture->Unset(0);

    m_vertex_positions->Unbind(m_shader->GetAttribute("position"));
    m_vertex_normals->Unbind(m_shader->GetAttribute("normal"));
    m_vertex_uv->Unbind(m_shader->GetAttribute("uv"));

    m_shader->Unbind();
}

void Sandbox::Shutdown() {}

} // namespace demos

} // namespace sputnik