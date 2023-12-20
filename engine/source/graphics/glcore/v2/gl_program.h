#pragma once

#include "core/core.h"

#include <vector.hpp>
#include <matrix.hpp>

namespace sputnik::graphics::glcore::v2
{

using namespace ramanujan::experimental;

enum class ShaderStageType : u32
{
    Vertex,
    Fragment,
    Geometry,
    TessellationControl,
    TessellationEvaluation,
    Compute
};

class GlShaderModule
{
public:
    // NON_COPYABLE(GlShaderModule)

    // Need to be moveable, but not copyable

    GlShaderModule(const ShaderStageType& stage_type);
    virtual ~GlShaderModule();

    bool loadFromFile(cstring shader_file);
    bool loadFromSource(cstring shader_source);

    const u32& getId() const;

protected:
    u32 m_id;
};

class GlProgram
{
public:
    NON_COPYABLE(GlProgram)

    GlProgram();
    virtual ~GlProgram();

    GlProgram& addShaderModule(cstring shader_file);
    GlProgram& addShaderModuleFromSource(cstring shader_source, const ShaderStageType& stage_type);
    void       configure();

    void bind();
    void unbind();

    virtual void setInt(const std::string& name, const int value);
    virtual void setIntArray(const std::string& name, int const* values, uint32_t count);
    virtual void setFloat(const std::string& name, const float& value);
    virtual void setFloat2(const std::string& name, const vec2& value);
    virtual void setFloat3(const std::string& name, const vec3& value);
    virtual void setFloat4(const std::string& name, const vec4& value);
    virtual void setMat4(const std::string& name, const mat4& value);
    virtual void setMat3(const std::string& name, const mat3& value);

    const u32& getId() const;
    u32 getAttributeId(const std::string& name) const;
    u32 getUniformId(const std::string& name) const;

protected:
    void link();

    /**
     * This method enumerates over all the attributes stored in the shader program, and stores them as key-value pairs
     * where
     * key: attribute name
     * value: attributes location in the shader
     */
    void populateAttributes();

    /**
     * This method enumerates over all the attributes stored in the shader program, and stores them as key-value pairs
     * where
     * key: uniform name
     * value: uniform location in the shader
     */
    void populateUniforms();

    void deleteShaderModules();

protected:
    u32                                           m_id;
    std::vector<GlShaderModule>                   m_shader_modules;
    std::unordered_map<std::string, unsigned int> m_attributes; // maps attribute name -> index in the shader
    std::unordered_map<std::string, unsigned int> m_uniforms;   // maps uniform name -> index in the shader
};

} // namespace sputnik::graphics::glcore::v2
