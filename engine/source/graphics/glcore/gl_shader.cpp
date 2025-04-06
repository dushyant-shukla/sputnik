#include "pch.h"
#include "gl_shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace sputnik::graphics::gl
{

cstring shaderStageTypeToString(const ShaderStageType& type)
{
    switch(type)
    {
        SPUTNIK_CASE_TO_STRING(ShaderStageType::Vertex);
        SPUTNIK_CASE_TO_STRING(ShaderStageType::Fragment);
        SPUTNIK_CASE_TO_STRING(ShaderStageType::Geometry);
        SPUTNIK_CASE_TO_STRING(ShaderStageType::TessellationControl);
        SPUTNIK_CASE_TO_STRING(ShaderStageType::TessellationEvaluation);
        SPUTNIK_CASE_TO_STRING(ShaderStageType::Compute);
    }
    return "Unknown";
}

const std::unordered_map<ShaderStageType, u32> kShaderStageTypeToGlType = {
    {ShaderStageType::Vertex, GL_VERTEX_SHADER},
    {ShaderStageType::Fragment, GL_FRAGMENT_SHADER},
    {ShaderStageType::Geometry, GL_GEOMETRY_SHADER},
    {ShaderStageType::TessellationControl, GL_TESS_CONTROL_SHADER},
    {ShaderStageType::TessellationEvaluation, GL_TESS_EVALUATION_SHADER},
    {ShaderStageType::Compute, GL_COMPUTE_SHADER}};

const std::unordered_map<std::string, u32> kStringToGlShaderType = {{"vert", GL_VERTEX_SHADER},
                                                                    {"frag", GL_FRAGMENT_SHADER},
                                                                    {"geom", GL_GEOMETRY_SHADER},
                                                                    {"tesc", GL_TESS_CONTROL_SHADER},
                                                                    {"tese", GL_TESS_EVALUATION_SHADER},
                                                                    {"comp", GL_COMPUTE_SHADER}};

const std::unordered_map<std::string, ShaderStageType> kStringToShaderStageType = {
    {"vert", ShaderStageType::Vertex},
    {"frag", ShaderStageType::Fragment},
    {"geom", ShaderStageType::Geometry},
    {"tesc", ShaderStageType::TessellationControl},
    {"tese", ShaderStageType::TessellationEvaluation},
    {"comp", ShaderStageType::Compute}};

// const u32 ShaderStageBits::kVertexShaderBit                 = GL_VERTEX_SHADER_BIT;
// const u32 ShaderStageBits::kFragmentShaderBit               = GL_FRAGMENT_SHADER_BIT;
// const u32 ShaderStageBits::kGeometryShaderBit               = GL_GEOMETRY_SHADER_BIT;
// const u32 ShaderStageBits::kTessellationControlShaderBit    = GL_TESS_CONTROL_SHADER_BIT;
// const u32 ShaderStageBits::kTessellationEvaluationShaderBit = GL_TESS_EVALUATION_SHADER_BIT;
// const u32 ShaderStageBits::kComputeShaderBit                = GL_COMPUTE_SHADER_BIT;

const std::unordered_map<u32, u32> kGlShaderTypeToGlShaderBit = {
    {GL_VERTEX_SHADER, GL_VERTEX_SHADER_BIT},
    {GL_FRAGMENT_SHADER, GL_FRAGMENT_SHADER_BIT},
    {GL_GEOMETRY_SHADER, GL_GEOMETRY_SHADER_BIT},
    {GL_TESS_CONTROL_SHADER, GL_TESS_CONTROL_SHADER_BIT},
    {GL_TESS_EVALUATION_SHADER, GL_TESS_EVALUATION_SHADER_BIT},
    {GL_COMPUTE_SHADER, GL_COMPUTE_SHADER_BIT}};

const char* getShaderStageTypeString(cstring shader_file)
{
    size_t length = strlen(shader_file);
    return shader_file + length - 4;
}

std::string readShaderSource(cstring shader_file)
{
    std::ifstream     shader_file_stream(shader_file);
    std::stringstream shader_source_stream;
    shader_source_stream << shader_file_stream.rdbuf();
    shader_file_stream.close();
    std::string shader_source = shader_source_stream.str();

    while(shader_source.find("#include ") != shader_source.npos)
    {
        const auto pos  = shader_source.find("#include ");
        const auto pos1 = shader_source.find('<', pos);
        const auto pos2 = shader_source.find('>', pos);
        if(pos1 == shader_source.npos || pos2 == shader_source.npos || pos2 <= pos1)
        {
            SPUTNIK_ASSERT_MESSAGE(false, "Invalid include statement in shader: {}", shader_file);
            return std::string();
        }
        const auto include_shader_file = shader_source.substr(pos1 + 1, pos2 - pos1 - 1);
        // const auto include_shader_source = readShaderSource(include_shader_file.c_str());
        const auto include_shader_source = readShaderSource(("../../data/shaders/glsl/" + include_shader_file).c_str());
        // const auto include_shader_source = readShaderSource(("../data/shaders/glsl/" + include_shader_file).c_str());
        shader_source.replace(pos, pos2 - pos + 1, include_shader_source);
    }
    return shader_source;
}

//////////////////////////////////////// EXPERIMENTAL STARTS HERE ////////////////////////////////////////

#ifdef EXPERIMENTAL_SHADER

OglShaderProgramStage::OglShaderProgramStage(cstring shader_file_path) : m_id{0}, m_stage_type{0}
{
    // Reference: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glCreateShaderProgram.xhtml
    auto itr = kStringToGlShaderType.find(getShaderStageTypeString(shader_file_path));
    if(itr == kStringToGlShaderType.end())
    {
        SPUTNIK_ASSERT_MESSAGE(false, "Unknown shader type: {}", shader_file_path);
    }
    std::string shader_source      = readShaderSource(shader_file_path);
    cstring     shader_source_cstr = shader_source.c_str();
    m_id                           = glCreateShaderProgramv(itr->second, 1, &shader_source_cstr);
    m_stage_type                   = itr->second;
    ENGINE_INFO("Shader loaded: {}, source: {}", shader_file_path, shader_source);
}

OglShaderProgramStage::OglShaderProgramStage(const ShaderStageType& stage_type, cstring shader_source)
    : m_id{0}
    , m_stage_type{0}
{
    // Reference: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glCreateShaderProgram.xhtml
    auto itr = kShaderStageTypeToGlType.find(stage_type);
    if(itr == kShaderStageTypeToGlType.end())
    {
        SPUTNIK_ASSERT_MESSAGE(false, "Unknown shader type: {}", static_cast<u32>(stage_type));
    }
    m_id         = glCreateShaderProgramv(itr->second, 1, &shader_source);
    m_stage_type = itr->second;
}

OglShaderProgramStage::~OglShaderProgramStage()
{
    glDeleteShader(m_id);
}

const u32& OglShaderProgramStage::getId() const
{
    return m_id;
}

const u32& OglShaderProgramStage::getStageType() const
{
    return m_stage_type;
}

OglShaderProgramPipeline::OglShaderProgramPipeline() : m_id{0}
{
    // m_id = glCreateProgram();

    // References:
    // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glCreateProgramPipelines.xhtml
    // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGenProgramPipelines.xhtml
    glCreateProgramPipelines(1, &m_id);
}

OglShaderProgramPipeline::~OglShaderProgramPipeline()
{
    // glDeleteProgram(m_id);
    glDeleteProgramPipelines(1, &m_id);
}

void OglShaderProgramPipeline::addShaderStage(const OglShaderProgramStage& shader_stage)
{
    // Reference: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glUseProgramStages.xhtml
    glUseProgramStages(m_id, kGlShaderTypeToGlShaderBit.at(shader_stage.getStageType()), shader_stage.getId());
}

void OglShaderProgramPipeline::addShaderStage(const OglShaderProgramStage& shader_stage, const u32& shader_stage_bits)
{
    // Reference: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glUseProgramStages.xhtml
    glUseProgramStages(m_id, shader_stage_bits, shader_stage.getId());
}

void OglShaderProgramPipeline::configure() {}

void OglShaderProgramPipeline::bind()
{
    // glUseProgram(m_id);
    glBindProgramPipeline(m_id);
}

void OglShaderProgramPipeline::unbind()
{
    // glUseProgram(0);
    glBindProgramPipeline(0);
}

const u32& OglShaderProgramPipeline::getId() const
{
    return m_id;
}

void OglShaderProgramPipeline::deleteShaderModules()
{
    for(auto& shader_module : m_shader_modules)
    {
        glDeleteShader(shader_module.getId());
    }
}

#endif // 0

/////////////////////////// EXPERIMENTAL ENDS HERE ////////////////////////////////

OglShaderProgram::OglShaderProgram() : m_id{0}
{
    m_id = glCreateProgram();
    SPUTNIK_ASSERT(m_id != 0, "Failed to create shader program.");
}

OglShaderProgram::~OglShaderProgram()
{
    glDeleteProgram(m_id);
}

OglShaderProgram::OglShaderProgram(OglShaderProgram&& other) noexcept
{
    *this = std::move(other);
}

OglShaderProgram& OglShaderProgram::operator=(OglShaderProgram&& other) noexcept
{
    if(this != &other)
    {
        m_id            = other.m_id;
        m_shader_stages = std::move(other.m_shader_stages);
        m_attributes    = std::move(other.m_attributes);
        m_uniforms      = std::move(other.m_uniforms);
        other.m_id      = 0;
    }
    return *this;
}

void OglShaderProgram::addShaderStage(cstring shader_filepath)
{
    OglShaderStage shader_stage(shader_filepath);
    m_shader_stages.push_back(std::move(shader_stage));
}

void OglShaderProgram::addShaderStage(const ShaderStageType& stage_type, cstring shader_source)
{
    OglShaderStage shader_stage(stage_type, shader_source);
    m_shader_stages.push_back(std::move(shader_stage));
}

void OglShaderProgram::configure()
{
    link();
    populateAttributes();
    populateUniforms();
}

const u32& OglShaderProgram::getId() const
{
    return m_id;
}

void OglShaderProgram::link()
{
    for(auto& shader_module : m_shader_stages)
    {
        glAttachShader(m_id, shader_module.getId());
    }
    glLinkProgram(m_id);

    int success = 0;
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if(!success)
    {
        int length;
        glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(length * sizeof(char));
        glGetProgramInfoLog(m_id, length, &length, message);
        SPUTNIK_ASSERT_MESSAGE(false, "Shader linking failed: {}", message);
        clear();
    }

    success = -1;
    glValidateProgram(m_id);
    glGetProgramiv(m_id, GL_VALIDATE_STATUS, &success);
    if(!success)
    {
        int length;
        glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(length * sizeof(char));
        glGetProgramInfoLog(m_id, length, &length, message);
        SPUTNIK_ASSERT_MESSAGE(false, "Shader validation failed: {}", message);
        clear();
    }

    // delete shaders here
    clear();
}

void OglShaderProgram::populateAttributes()
{
    // Reference: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGetAttribLocation.xhtml

    int    count = -1;
    int    length;
    char   name[128];
    int    size;
    GLenum type;
    glUseProgram(m_id);
    glGetProgramiv(m_id, GL_ACTIVE_ATTRIBUTES, &count); // Get total number of active attributes in the shader program
    for(int i = 0; i < count; ++i)
    {
        memset(name, 0, sizeof(char) * 128);
        glGetActiveAttrib(m_id, (GLuint)i, 128, &length, &size, &type, name); // Get the name of the active attribute
        int attrib = glGetAttribLocation(m_id, name); // Get the location of the active attribute in the shader program
        if(attrib >= 0)                               // if the attribute is valid
        {
            m_attributes[name] = attrib;
        }
    }
    glUseProgram(0);
}

void OglShaderProgram::populateUniforms()
{
    int    count = -1;
    int    length;
    char   name[128];
    int    size;
    GLenum type;
    char   subscripted_uniform_name[256];

    glUseProgram(m_id);
    glGetProgramiv(m_id, GL_ACTIVE_UNIFORMS, &count); // Get total number of active uniforms in the shader program

    for(int i = 0; i < count; ++i)
    {
        memset(name, 0, sizeof(char) * 128);
        glGetActiveUniform(m_id, (GLuint)i, 128, &length, &size, &type, name); // Get the name of the active uniform

        int uniform = glGetUniformLocation(m_id, name);
        if(uniform >= 0) // if the uniform is valid
        {
            std::string uniform_name = name;
            std::size_t found        = uniform_name.find('['); // check if the uniform is an array
            if(found != std::string::npos)
            {
                // erase everything from the uniform array name starting with '[',
                // leaving us with a simple uniform name without array subscripts
                uniform_name.erase(uniform_name.begin() + found, uniform_name.end());

                // Populate subscripted names too
                unsigned int uniform_index = 0;
                while(true)
                {
                    // Retrieve every index from the uniform array by appending [ + index + ] to the uniform name
                    memset(subscripted_uniform_name, 0, sizeof(char) * 256);
                    sprintf_s(subscripted_uniform_name, "%s[%d]", uniform_name.c_str(), uniform_index++);
                    int uniform_location = glGetUniformLocation(m_id, subscripted_uniform_name);
                    if(uniform_location < 0)
                    {
                        break;
                    }
                    m_uniforms[subscripted_uniform_name] = uniform_location;
                }
            }
            // Now, we have the name of the uniform. If the uniform is an array, the [0] part of the name has been
            // removed.
            m_uniforms[uniform_name] = uniform;
        }
    }

    glUseProgram(0);
}

void OglShaderProgram::clear()
{
    m_shader_stages.clear();
}

const u32& OglShaderProgram::getAttributeId(const std::string& name) const
{
    auto itr = m_attributes.find(name);
    if(itr == m_attributes.end())
    {
        SPUTNIK_ASSERT_MESSAGE(false, "Bad attribute: {}", name);
    }
    return itr->second;
}

const u32& OglShaderProgram::getUniformId(const std::string& name) const
{
    auto itr = m_uniforms.find(name);
    if(itr == m_uniforms.end())
    {
        SPUTNIK_ASSERT_MESSAGE(false, "Bad uniform: {}", name);
    }
    return itr->second;
}

void OglShaderProgram::bind()
{
    glUseProgram(m_id);
}

void OglShaderProgram::unbind()
{
    glUseProgram(0);
}

void OglShaderProgram::setInt(const std::string& name, const int value)
{
    u32 uniform_id = getUniformId(name);
    glUniform1i(uniform_id, value);
}

void OglShaderProgram::setIntArray(const std::string& name, int const* values, uint32_t count)
{
    u32 uniform_id = getUniformId(name);
    glUniform1iv(uniform_id, count, values);
}

void OglShaderProgram::setFloat(const std::string& name, const float& value)
{
    u32 uniform_id = getUniformId(name);
    glUniform1f(uniform_id, value);
}

void OglShaderProgram::setFloat2(const std::string& name, const vec2& value)
{
    u32 uniform_id = getUniformId(name);
    glUniform2f(uniform_id, value.x, value.y); // or glUniform2fv(location, 1, &value[0]);
}

void OglShaderProgram::setFloat3(const std::string& name, const vec3& value)
{
    u32 uniform_id = getUniformId(name);
    glUniform3f(uniform_id, value.x, value.y, value.z); // or glUniform3fv(location, 1, &value[0]);
}

void OglShaderProgram::setFloat4(const std::string& name, const vec4& value)
{
    u32 uniform_id = getUniformId(name);
    glUniform4f(uniform_id, value.x, value.y, value.z, value.w); // or glUniform4fv(location, 1, &value[0]);
}

void OglShaderProgram::setMat4(const std::string& name, const mat4& value)
{
    u32 uniform_id = getUniformId(name);
    glUniformMatrix4fv(uniform_id, 1, GL_FALSE, (float*)&value.m[0]);
}

void OglShaderProgram::setMat4s(const std::string& name, const std::vector<Matrix4>& value)
{
    u32 uniform_id = getUniformId(name);
    // glUniformMatrix4fv(uniform_id, (GLsizei)value.size(), GL_FALSE, (float*)&value[0]);
    glUniformMatrix4fv(uniform_id, (GLsizei)value.size(), GL_FALSE, (float*)&value[0].v[0]);
}

void OglShaderProgram::setMat3(const std::string& name, const mat3& value)
{
    u32 uniform_id = getUniformId(name);
    glUniformMatrix3fv(uniform_id, 1, GL_FALSE, (float*)&value.m[0]);
}

void OglShaderProgram::setMat4(const std::string& name, const glm::mat4& value)
{
    u32 uniform_id = getUniformId(name);
    glUniformMatrix4fv(uniform_id, 1, GL_FALSE, glm::value_ptr(value[0]));
}

OglShaderStage::OglShaderStage(cstring shader_file_path)
{
    // Reference: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glCreateShader.xhtml
    auto itr = kStringToGlShaderType.find(getShaderStageTypeString(shader_file_path));
    if(itr == kStringToGlShaderType.end())
    {
        SPUTNIK_ASSERT_MESSAGE(false, "Unknown shader type: {}", shader_file_path);
    }
    std::string shader_source      = readShaderSource(shader_file_path);
    cstring     shader_source_cstr = shader_source.c_str();
    m_id                           = glCreateShader(itr->second);
    m_stage_type                   = itr->second;
    glShaderSource(m_id, 1, &shader_source_cstr, nullptr);
    glCompileShader(m_id);
    int success = 0;
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        int length;
        glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(length * sizeof(char));
        glGetShaderInfoLog(m_id, length, &length, message);
        SPUTNIK_ASSERT_MESSAGE(false, "Shader compilation failed: {}", message);
        glDeleteShader(m_id);
    }
    ENGINE_INFO("Shader loaded: {}, source: {}", shader_file_path, shader_source);
}

OglShaderStage::OglShaderStage(const ShaderStageType& stage_type, cstring shader_source)
{
    // Reference: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glCreateShader.xhtml
    auto itr = kShaderStageTypeToGlType.find(stage_type);
    if(itr == kShaderStageTypeToGlType.end())
    {
        SPUTNIK_ASSERT_MESSAGE(false, "Unknown shader type: {}", shaderStageTypeToString(stage_type));
    }

    m_id         = glCreateShader(itr->second);
    m_stage_type = itr->second;
    glShaderSource(m_id, 1, &shader_source, nullptr);
    glCompileShader(m_id);
    int success = 0;
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        int length;
        glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(length * sizeof(char));
        glGetShaderInfoLog(m_id, length, &length, message);
        SPUTNIK_ASSERT_MESSAGE(false, "Shader compilation failed: {}", message);
        glDeleteShader(m_id);
    }
    ENGINE_INFO("Shader loaded: source: {}", shader_source);
}

OglShaderStage::~OglShaderStage()
{
    glDeleteShader(m_id);
}

OglShaderStage::OglShaderStage(OglShaderStage&& other) noexcept
{
    *this = std::move(other);
}

OglShaderStage& OglShaderStage::operator=(OglShaderStage&& other) noexcept
{
    if(this != &other)
    {
        m_id               = other.m_id;
        m_stage_type       = other.m_stage_type;
        other.m_id         = 0;
        other.m_stage_type = 0;
    }
    return *this;
}

const u32& OglShaderStage::getId() const
{
    return m_id;
}

const u32& OglShaderStage::getStageType() const
{
    return m_stage_type;
}

} // namespace sputnik::graphics::gl
