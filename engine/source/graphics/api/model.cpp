#include "pch.h"

#include "model.h"
#include "graphics/glcore/gltf_loader.h"

namespace sputnik::graphics::api
{

Model::Model() {}

Model::~Model() {}

void Model::Draw(std::shared_ptr<Shader> shader_program, const bool& uv, const bool& joints, const bool& weights)
{
    int position_slot = shader_program->GetAttribute("position");
    int normal_slot   = shader_program->GetAttribute("normal");
    int uv_slot       = uv ? shader_program->GetAttribute("uv") : -1;
    int weight_slot   = weights ? shader_program->GetAttribute("weights") : -1;
    int joints_slot   = joints ? shader_program->GetAttribute("joints") : -1;
    // int uv_slot     = shader_program->GetAttribute("uv");
    // int weight_slot = shader_program->GetAttribute("weights");
    // int joints_slot = shader_program->GetAttribute("joints");
    for(auto& mesh : m_meshes)
    {
        mesh.Bind(position_slot, normal_slot, uv_slot, weight_slot, joints_slot);
        mesh.Draw();
        mesh.Unbind(position_slot, normal_slot, uv_slot, weight_slot, joints_slot);
    }
}

std::shared_ptr<Model> Model::LoadModel(const std::string& path)
{
    auto        model     = std::make_shared<Model>();
    cgltf_data* gltf_data = sputnik::gltf::GltfLoader::LoadFile(path.c_str());
    model->m_meshes       = sputnik::gltf::GltfLoader::LoadMeshes(gltf_data);
    return model;
}

} // namespace sputnik::graphics::api
