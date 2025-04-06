#pragma once

#include "graphics/core/geometry/mesh.h"
#include "graphics/api/color_material.h"
#include "graphics/glcore/gl_vertex_array.h"
// #include <graphics/glcore/shader.h>

#include <string>
#include <vector>
#include <memory>

namespace sputnik::graphics::api
{

using namespace sputnik::graphics::core;
// using namespace sputnik::graphics::glcore;

class Model
{
public:
    Model();
    virtual ~Model();

    // void Draw(std::shared_ptr<Shader> shader_program,
    //           const bool&             uv      = true,
    //           const bool&             joints  = true,
    //           const bool&             weights = true);

    void Draw();
    void draw(const Material& material, const mat4& model = {});
    void draw(const Material& material, const mat4& model, const std::vector<Matrix4>& skin_transformations);
    void drawInstanced(const Material& material, const u32& num_instances);

    void update(const std::vector<ramanujan::Matrix4>& pose_palette, const bool& cpu_skin = false);

    static std::shared_ptr<Model> LoadModel(const std::string& path);

    std::shared_ptr<OglVertexArray> getVertexArray() const;

    const std::vector<ramanujan::Vector3>& getPositions();
    const std::vector<ramanujan::Vector3>& getNormals();
    const std::vector<unsigned int>&       getIndices();

    void updatePositionBuffer(void* data, const u64& byte);
    void updateNormalBuffer(void* data, const u64& byte);

protected:
    // Write now we expect a single mesh per model
    std::vector<Mesh> m_meshes;
};
} // namespace sputnik::graphics::api
