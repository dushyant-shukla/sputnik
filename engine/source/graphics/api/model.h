#pragma once

#include "graphics/core/geometry/mesh.h"
//#include <graphics/glcore/shader.h>

#include <string>
#include <vector>
#include <memory>

namespace sputnik::graphics::api
{

using namespace sputnik::graphics::core;
//using namespace sputnik::graphics::glcore;

class Model
{
public:
    Model();
    virtual ~Model();

    //void Draw(std::shared_ptr<Shader> shader_program,
    //          const bool&             uv      = true,
    //          const bool&             joints  = true,
    //          const bool&             weights = true);

    void Draw();

    static std::shared_ptr<Model> LoadModel(const std::string& path);

protected:
protected:
    std::vector<Mesh> m_meshes;
};
} // namespace sputnik::graphics::api
