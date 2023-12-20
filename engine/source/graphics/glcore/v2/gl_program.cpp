#include "pch.h"
#include "gl_program.h"

#include <glad/glad.h>

namespace sputnik::graphics::glcore::v2
{

const std::unordered_map<ShaderStageType, u32> kShaderStageTypeToGlType = {
    {ShaderStageType::Vertex, GL_VERTEX_SHADER},
    {ShaderStageType::Fragment, GL_FRAGMENT_SHADER},
    {ShaderStageType::Geometry, GL_GEOMETRY_SHADER},
    {ShaderStageType::TessellationControl, GL_TESS_CONTROL_SHADER},
    {ShaderStageType::TessellationEvaluation, GL_TESS_EVALUATION_SHADER},
    {ShaderStageType::Compute, GL_COMPUTE_SHADER}};

const std::unordered_map<std::string, ShaderStageType> kShaderStageTypeStringToGlType = {
    {".vert", ShaderStageType::Vertex},
    {".frag", ShaderStageType::Fragment},
    {".geom", ShaderStageType::Geometry},
    {".tesc", ShaderStageType::TessellationControl},
    {".tese", ShaderStageType::TessellationEvaluation},
    {".comp", ShaderStageType::Compute}};

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
        const auto include_shader_file   = shader_source.substr(pos1 + 1, pos2 - pos1 - 1);
        const auto include_shader_source = readShaderSource(include_shader_file.c_str());
        shader_source.replace(pos, pos2 - pos + 1, include_shader_source);
    }
    return shader_source;
}

GlShaderModule::GlShaderModule(const ShaderStageType& stage_type) : m_id{0}
{
    auto itr = kShaderStageTypeToGlType.find(stage_type);
    if(itr == kShaderStageTypeToGlType.end())
    {
        SPUTNIK_ASSERT_MESSAGE(false, "Unknown shader type: {}", static_cast<u32>(stage_type));
    }
    m_id = glCreateShader(itr->second);
}

GlShaderModule::~GlShaderModule()
{
    glDeleteShader(m_id);
}

bool GlShaderModule::loadFromFile(cstring shader_file)
{
    cstring shader_source = readShaderSource(shader_file).c_str();
    return loadFromSource(shader_source);
}

bool GlShaderModule::loadFromSource(cstring shader_source)
{
    glShaderSource(m_id, 1, &shader_source, nullptr);
    glCompileShader(m_id);
    int success = 0;
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        char info_log[512];
        glGetShaderInfoLog(m_id, 512, NULL, info_log);
        ENGINE_CRITICAL("Shader compilation failed: {}", info_log);
        glDeleteShader(m_id);
        return false;
    }
    return true;
}

const u32& GlShaderModule::getId() const
{
    return m_id;
}

GlProgram::GlProgram() : m_id{0}
{
    m_id = glCreateProgram();
}

GlProgram::~GlProgram()
{
    glDeleteProgram(m_id);
}

GlProgram& GlProgram::addShaderModule(cstring shader_file)
{
    auto itr = kShaderStageTypeStringToGlType.find(getShaderStageTypeString(shader_file));
    if(itr != kShaderStageTypeStringToGlType.end())
    {
        GlShaderModule shader_module(itr->second);
        if(shader_module.loadFromFile(shader_file))
        {
            m_shader_modules.push_back(shader_module);
            return *this;
        }
        SPUTNIK_ASSERT(false, "Invalid shader source.");
    }

    SPUTNIK_ASSERT_MESSAGE(false, "Unknown shader type: {}", shader_file);
    return *this;
}

GlProgram& GlProgram::addShaderModuleFromSource(cstring shader_source, const ShaderStageType& stage_type)
{
    GlShaderModule shader_module(stage_type);
    if(shader_module.loadFromSource(shader_source))
    {
        m_shader_modules.push_back(shader_module);
        return *this;
    }

    SPUTNIK_ASSERT(false, "Invalid shader source.");
    return *this;
}

void GlProgram::configure()
{
    link();
    populateAttributes();
    populateUniforms();
}

void GlProgram::bind()
{
    glUseProgram(m_id);
}

void GlProgram::unbind()
{
    glUseProgram(0);
}

void GlProgram::setInt(const std::string& name, const int value)
{
    u32 uniform_id = getUniformId(name);
    glUniform1i(uniform_id, value);
}

void GlProgram::setIntArray(const std::string& name, int const* values, uint32_t count)
{
    u32 uniform_id = getUniformId(name);
    glUniform1iv(uniform_id, count, values);
}

void GlProgram::setFloat(const std::string& name, const float& value)
{
    u32 uniform_id = getUniformId(name);
    glUniform1f(uniform_id, value);
}

void GlProgram::setFloat2(const std::string& name, const vec2& value)
{
    u32 uniform_id = getUniformId(name);
    glUniform2f(uniform_id, value.x, value.y); // or glUniform2fv(location, 1, &value[0]);
}

void GlProgram::setFloat3(const std::string& name, const vec3& value)
{
    u32 uniform_id = getUniformId(name);
    glUniform3f(uniform_id, value.x, value.y, value.z); // or glUniform3fv(location, 1, &value[0]);
}

void GlProgram::setFloat4(const std::string& name, const vec4& value)
{
    u32 uniform_id = getUniformId(name);
    glUniform4f(uniform_id, value.x, value.y, value.z, value.w); // or glUniform4fv(location, 1, &value[0]);
}

void GlProgram::setMat4(const std::string& name, const mat4& value)
{
    u32 uniform_id = getUniformId(name);
    glUniformMatrix4fv(uniform_id, 1, GL_FALSE, (float*)&value.m[0]);
}

void GlProgram::setMat3(const std::string& name, const mat3& value)
{
    u32 uniform_id = getUniformId(name);
    glUniformMatrix3fv(uniform_id, 1, GL_FALSE, (float*)&value.m[0]);
}

const u32& GlProgram::getId() const
{
    return m_id;
}

u32 GlProgram::getAttributeId(const std::string& name) const
{
    auto itr = m_attributes.find(name);
    if(itr == m_attributes.end())
    {
        SPUTNIK_ASSERT_MESSAGE(false, "Bad attribute: {}", name);
    }
    return itr->second;
}

u32 GlProgram::getUniformId(const std::string& name) const
{
    auto itr = m_uniforms.find(name);
    if(itr == m_uniforms.end())
    {
        SPUTNIK_ASSERT_MESSAGE(false, "Bad uniform: {}", name);
    }
    return itr->second;
}

void GlProgram::link()
{
    for(auto& shader_module : m_shader_modules)
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
        deleteShaderModules();
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
        deleteShaderModules();
    }

    // delete shaders here
}

void GlProgram::populateAttributes()
{
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

void GlProgram::populateUniforms()
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

void GlProgram::deleteShaderModules()
{
    for(auto& shader_module : m_shader_modules)
    {
        glDeleteShader(shader_module.getId());
    }
}

} // namespace sputnik::graphics::glcore::v2
