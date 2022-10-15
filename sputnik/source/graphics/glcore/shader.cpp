#include "pch.h"
#include "shader.h"

#include <glad/glad.h>

namespace sputnik::glcore
{

Shader::Shader()
{
    m_id = glCreateProgram();
}

Shader::Shader(std::string vertex_shader, std::string fragment_shader)
{
    m_id = glCreateProgram();
    Load(vertex_shader, fragment_shader);
}

Shader::~Shader()
{
    glDeleteProgram(m_id);
}

void Shader::Bind()
{
    glUseProgram(m_id);
}

void Shader::Unbind()
{
    glUseProgram(0);
}

unsigned int Shader::GetId()
{
    return m_id;
}

unsigned int Shader::GetAttribute(const std::string name)
{
    std::unordered_map<std::string, unsigned int>::const_iterator itr = m_attributes.find(name);
    if(itr == m_attributes.end())
    {
        std::cout << "Bad attribute index: " << name << "\n";
    }
    return itr->second;
}

unsigned int Shader::GetUniform(const std::string name)
{
    std::unordered_map<std::string, unsigned int>::const_iterator itr = m_uniforms.find(name);
    if(itr == m_uniforms.end())
    {
        std::cout << "Bad uniform index: " << name << "\n";
    }
    return itr->second;
}

void Shader::Load(const std::string& vertex_shader, const std::string& fragment_shader)
{
    std::ifstream f(vertex_shader.c_str());
    bool          vertFile = f.good();
    f.close();

    f             = std::ifstream(fragment_shader.c_str());
    bool fragFile = f.good();
    f.close();

    std::string v_source = vertex_shader;
    if(vertFile)
    {
        v_source = ReadShaderSource(vertex_shader);
    }

    std::string f_source = fragment_shader;
    if(fragFile)
    {
        f_source = ReadShaderSource(fragment_shader);
    }

    unsigned int v_shader = CompileVertexShader(v_source);
    unsigned int f_shader = CompileFragmentShader(f_source);
    if(LinkShaders(v_shader, f_shader))
    {
        PopulateAttributes();
        PopulateUniforms();
    }
}

std::string Shader::ReadShaderSource(const std::string& filepath)
{
    std::ifstream _file;
    _file.open(filepath);
    std::stringstream _shader_source;
    _shader_source << _file.rdbuf();
    _file.close();
    return _shader_source.str();
}

unsigned int Shader::CompileVertexShader(const std::string& vertex_shader_source)
{
    unsigned int vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    const char*  shader_source    = vertex_shader_source.c_str();
    glShaderSource(vertex_shader_id, 1, &shader_source, NULL);
    glCompileShader(vertex_shader_id);
    int success = 0;
    glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        char info_log[512];
        glGetShaderInfoLog(vertex_shader_id, 512, NULL, info_log);
        std::cout << "Vertex shader compilation failed.\n";
        std::cout << "\t" << info_log << "\n";
        glDeleteShader(vertex_shader_id);
        return 0;
    }
    return vertex_shader_id;
}

unsigned int Shader::CompileFragmentShader(const std::string& fragment_shader_source)
{
    unsigned int fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    const char*  shader_source      = fragment_shader_source.c_str();
    glShaderSource(fragment_shader_id, 1, &shader_source, NULL);
    glCompileShader(fragment_shader_id);
    int success = 0;
    glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        char info_log[512];
        glGetShaderInfoLog(fragment_shader_id, 512, NULL, info_log);
        std::cout << "Fragmentation shader compilation failed.\n";
        std::cout << "\t" << info_log << "\n";
        glDeleteShader(fragment_shader_id);
        return 0;
    }
    return fragment_shader_id;
}

bool Shader::LinkShaders(unsigned int vertex_shader_id, unsigned int fragment_shader_id)
{
    glAttachShader(m_id, vertex_shader_id);
    glAttachShader(m_id, fragment_shader_id);
    glLinkProgram(m_id);
    int success = 0;
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if(!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(m_id, 512, NULL, infoLog);
        std::cout << "ERROR: Shader linking failed.\n";
        std::cout << "\t" << infoLog << "\n";
        glDeleteShader(vertex_shader_id);
        glDeleteShader(fragment_shader_id);
        return false;
    }

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    return true;
}

void Shader::PopulateAttributes()
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

void Shader::PopulateUniforms()
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

} // namespace sputnik::glcore