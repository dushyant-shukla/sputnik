#pragma once

#include "pch.h"
#include "core/core.h"

namespace sputnik::graphics::glcore
{

class Shader
{

public:
    // public interface

    /**
     * This constructor creates an empty shader object with no shader programs attached.
     *
     */
    Shader();

    /*!
     * @brief .
     *
     * @param compute_shader_path
     */
    Shader(const std::string& compute_shader_path);

    /**
     * This constructor accepts either the paths to files containing the shader source or inline defined shader
     * definition.
     *
     * \param vertex_shader Path to the file containing vertex shader source
     * \param fragment_shader Path to the file containing fragment shader source
     */
    Shader(std::string vertex_shader, std::string fragment_shader);
    ~Shader();

    // copy constructor and assignment operators are deleted because this class is not intended to be copied since it
    // holds a handle to a GPU resource
    NON_COPYABLE(Shader);

    void Bind();
    void Unbind();

    unsigned int GetId();

    /**
     * This method performs a lookup for a shader attribute.
     *
     * \param name Attribute name
     * \return The index of the shader attribute.
     */
    unsigned int GetAttribute(const std::string name);

    /**
     * This method performs a lookup for a shader uniform.
     *
     * \param name Uniform name
     * \return The index of the shader uniform
     */
    unsigned int GetUniform(const std::string name);

protected:
    // protected methods

    /**
     * This method loads the individual shader components of the shader program. It takes two strings, which can be
     * either filepaths or inline defined shader source.
     *
     * \param vertex_shader
     * \param fragment_shader
     */
    void Load(const std::string& vertex_shader, const std::string& fragment_shader);

    /*!
     * @brief This method loads a compute shader. It takes a string, which can be either a filepath or inline defined
     * shader source.
     *
     * @param compute_shader Path to the file containing compute shader source, or the source itself.
     */
    void LoadComputeShader(const std::string& compute_shader);

    /**
     * This method reads shader source given a path to the shader file.
     *
     * \param path Path to a file containing shader source
     * \return Shader source
     */
    std::string ReadShaderSource(const std::string& path);

    /**
     * This method prepares a vertex shader
     *
     * \param _vertex_shader_source Source of a vertex shader
     * \return An id associated with the shader on successful compilation, zero otherwise.
     */
    unsigned int CompileVertexShader(const std::string& vertex_shader_source);

    /**
     * This method prepares a fragment shader
     *
     * \param fragment_shader_source Source of a fragment shader
     * \return An id associated with the shader on successful compilation, zero otherwise.
     */
    unsigned int CompileFragmentShader(const std::string& fragment_shader_source);

    /*!
     * @brief This method prepares a compute shader.
     *
     * @param compute_shader_source Source of a compute shader
     * @return  An id associated with the shader on sussecful compilation, zero otherwise.
     */
    unsigned int CompileComputeShader(const std::string& compute_shader_source);

    /**
     * This method attaches the shaders to the shader program.
     *
     * \param vertex_shader_id
     * \param fragment_shader_id
     * \return true if shader linked successfully, false otherwise
     */
    bool LinkShaders(unsigned int vertex_shader_id, unsigned int fragment_shader_id);

    /*!
     * @brief This method attaches the given compute shader to the shader program.
     *
     * @param compute_shader_id The compute shader id
     * @return true if shader linked successfully, false otherwise
     */
    bool LinkComputeShader(unsigned int compute_shader_id);

    /**
     * This method enumerates over all the attributes stored in the shader program, and stores them as key-value pairs
     * where
     * key: attribute name
     * value: attributes location in the shader
     */
    void PopulateAttributes();

    /**
     * This method enumerates over all the attributes stored in the shader program, and stores them as key-value pairs
     * where
     * key: uniform name
     * value: uniform location in the shader
     */
    void PopulateUniforms();

private:
    // private methods

private:
    // private data

    unsigned int                                  m_id;
    std::unordered_map<std::string, unsigned int> m_attributes; // maps attribute name -> index in the shader
    std::unordered_map<std::string, unsigned int> m_uniforms;   // maps uniform name -> index in the shader
};

} // namespace sputnik::graphics::glcore