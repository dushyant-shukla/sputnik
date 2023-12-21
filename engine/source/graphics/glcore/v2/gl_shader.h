#pragma once

#include "core/core.h"

#include <vector.hpp>
#include <matrix.hpp>

namespace sputnik::graphics::gl
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

cstring shaderStageTypeToString(const ShaderStageType& type);

class ShaderStageBits
{
public:
    NON_INSTATIABLE(ShaderStageBits)

    static const u32 kVertexShaderBit;
    static const u32 kFragmentShaderBit;
    static const u32 kGeometryShaderBit;
    static const u32 kTessellationControlShaderBit;
    static const u32 kTessellationEvaluationShaderBit;
    static const u32 kComputeShaderBit;
};

////////////////////////////////////////////////////// EXPERIMENTAL
/////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class OglShaderProgramStage
{
public:
    // NON_COPYABLE(GlShaderModule)

    // Need to be moveable, but not copyable

    OglShaderProgramStage(cstring shader_file_path);
    OglShaderProgramStage(const ShaderStageType& stage_type, cstring shader_source);
    virtual ~OglShaderProgramStage();

    const u32& getId() const;
    const u32& getStageType() const;

protected:
    u32 m_id;
    u32 m_stage_type;
};

// Todo:: This is experimental, still figuring out how it works and how to best implement this
class OglShaderProgramPipeline
{
public:
    NON_COPYABLE(OglShaderProgramPipeline)

    OglShaderProgramPipeline();
    virtual ~OglShaderProgramPipeline();

    void addShaderStage(const OglShaderProgramStage& shader_stage);
    void addShaderStage(const OglShaderProgramStage& shader_stage, const u32& shader_stage_bits);
    void configure();

    void bind();
    void unbind();

    const u32& getId() const;

protected:
    void deleteShaderModules();

protected:
    u32                                  m_id;
    std::vector<OglShaderProgramStage>   m_shader_modules;
    std::unordered_map<std::string, u32> m_attributes; // maps attribute name -> index in the shader
    std::unordered_map<std::string, u32> m_uniforms;   // maps uniform name -> index in the shader
};

/////////////////////////////////////////// EXPERIMENTAL ENDS ////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

class OglShaderStage
{
public:
    OglShaderStage(cstring shader_file_path);
    OglShaderStage(const ShaderStageType& stage_type, cstring shader_source);
    virtual ~OglShaderStage();

    OglShaderStage(OglShaderStage&& other) noexcept;
    OglShaderStage& operator=(OglShaderStage&& other) noexcept;

    const u32& getId() const;
    const u32& getStageType() const;

protected:
    OglShaderStage(const OglShaderStage& other)      = delete;
    OglShaderStage& operator=(OglShaderStage& other) = delete;

protected:
    u32 m_id;
    u32 m_stage_type;
};

class OglShaderProgram
{
public:
    OglShaderProgram();
    virtual ~OglShaderProgram();

    OglShaderProgram(OglShaderProgram&& other) noexcept;
    OglShaderProgram& operator=(OglShaderProgram&& other) noexcept;

    void       addShaderStage(cstring shader_filepath);
    void       addShaderStage(const ShaderStageType& stage_type, cstring shader_source);
    void       configure();
    const u32& getId() const;
    const u32& getAttributeId(const std::string& name) const;
    const u32& getUniformId(const std::string& name) const;

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

protected:
    OglShaderProgram(const OglShaderProgram& other)      = delete;
    OglShaderProgram& operator=(OglShaderProgram& other) = delete;

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

    void clear();

private:
    u32                                  m_id;
    std::unordered_map<std::string, u32> m_attributes; // maps attribute name -> index in the shader
    std::unordered_map<std::string, u32> m_uniforms;   // maps uniform name -> index in the shader
    std::vector<OglShaderStage>          m_shader_stages;
};

} // namespace sputnik::graphics::gl
